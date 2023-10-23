/*
 Description:
 This firmware is designed to interface with an OLED screen and rotary encoders
 to control a synth device named 'Nordl3'. 

  Features:
  1. Startup screen that displays the Nordl3 logo.
  2. 8 rotary encoders are read to select a note value which is then displayed 
      on the OLED screen.
  3. Each rotary encoder is paired with a button that serves as an indicator toggle.
  4. Indicators (small circles) on the OLED screen display the state of each button.

  Hardware Components:
  - Adafruit SSD1306 OLED display: Used for visual feedback.
  - Rotary Encoders: Used for note selection.
  - Buttons: Paired with rotary encoders for toggling states.

  Libraries:
  - Adafruit_SSD1306 (For the OLED)
  - Encoder (For reading the rotary encoders)
  - SPI (For DAC communication)
  - Wire (For I2C communication)

  Written by Nordl3, 2023
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

// STARTUP SCREEN SETTINGS
#define LOGO_X 10              // Horizontal position of the startup logo text
#define LOGO_Y 24              // Vertical position of the startup logo text
#define LOGO_FONT_SIZE 3       // Font size for the startup logo text
const char* startupLogoText = "Nordl3";

// GLOBAL DISPLAY SETTINGS
#define GLOBAL_FONT_SIZE 1
#define LINE_SPACING (8 * GLOBAL_FONT_SIZE)

// DEFAULT NOTE SETTINGS
int defaultNoteIndex = 48;  // Index of C4 in the truncated notes array

// ENCODER SETTINGS
Encoder encoders[] = {Encoder(2, 3), Encoder(4, 5), Encoder(6, 7), Encoder(8, 9), Encoder(24, 25), Encoder(26, 27), Encoder(28, 29), Encoder(30, 31)};
const int encoderButtons[] = {34, 35, 36, 37, 38, 39, 22, 23};
bool buttonStates[8] = {0};
unsigned long lastDebounceTime[8];
const unsigned long debounceDelay = 80;
const int numEncoders = 8;
boolean lastButtonStates[numEncoders];

// OLED DISPLAY SETTINGS
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NOTE ARRAY
const char *notes[] = {
  "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
  "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
  "C4"
};

void setup() {
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);  // Hang if OLED allocation fails
  }
  
  // Display the startup logo
  display.clearDisplay();
  display.setTextSize(LOGO_FONT_SIZE);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(LOGO_X, LOGO_Y);
  display.print(startupLogoText);
  display.display();
  delay(1000);  // Display for 1 second
  display.clearDisplay();
  display.display();

  // Initialize encoders and buttons
  for (int i = 0; i < numEncoders; i++) {
    encoders[i].write(defaultNoteIndex * 4);
    pinMode(encoderButtons[i], INPUT_PULLUP);   // Set up the encoder buttons using internal pull-ups
    lastDebounceTime[i] = 0;
    lastButtonStates[i] = HIGH;                 // Default to HIGH due to pull-ups
    buttonStates[i] = true;                     // Default state of each indicator to active
  }
}

long encoderPositions[8] = {-999, -999, -999, -999, -999, -999, -999, -999};

void loop() {
  // Clear display at the start of the loop
  display.clearDisplay();
  display.setTextSize(GLOBAL_FONT_SIZE);
  display.setTextColor(SSD1306_WHITE);

  for (int i = 0; i < 8; i++) {
    int x = 32 * (i % 4) + 8;
    int y = i < 4 ? 0 : 4 * LINE_SPACING;
    y += 4;

    // Read and display encoder values
    long newPosition = encoders[i].read() / 4;
    if (newPosition != encoderPositions[i]) {
      encoderPositions[i] = newPosition;
    }
    display.setCursor(x, y);
    display.print(notes[encoderPositions[i] % (sizeof(notes) / sizeof(notes[0]))]);

    // Read and manage button states for indicator toggle
    int reading = digitalRead(encoderButtons[i]);
    if (reading == LOW && lastButtonStates[i] == HIGH && (millis() - lastDebounceTime[i]) > debounceDelay) {
      lastDebounceTime[i] = millis();
      buttonStates[i] = !buttonStates[i];  // Toggle the button state
    }
    lastButtonStates[i] = reading;

    // Display indicator based on the button state
    int indicatorX = x + 6;
    int indicatorY = y + LINE_SPACING + 8;
    if (buttonStates[i]) {
      display.fillCircle(indicatorX, indicatorY, 3, SSD1306_WHITE);
    }
  }
  
  display.display();  // Update the display at the end of the loop
}
