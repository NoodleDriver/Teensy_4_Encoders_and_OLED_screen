/*
# Teesy Groovebox V1

## Current Progress

### Date: October 24, 2023

#### Features:
- Utilizes an OLED display with Adafruit's SSD1306 library.
- Configured for 10 rotary encoders with push buttons for various functions.
- Implements basic debouncing for button inputs.
- Cycle between modes ("Pitch", "Decay", "Velocity", "Cutoff") with a dedicated mode button.
- Display mode change on OLED.
- Display the notes based on encoder rotations.

#### Known Issues:
- None at the moment.

#### To-Do:
- Implement MIDI input functionalities.
- Add more modes and corresponding functionalities.
- Optimize and refactor code for better readability and maintenance.

#### Dependencies:
- SPI library
- Wire library
- Adafruit_SSD1306 library
- Encoder library

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

int currentMode = 1;  // To keep track of the current mode
const int modeButton = 32;  // Pin for Mode button
unsigned long lastModeDebounceTime = 0;  // Debounce timer
boolean lastModeButtonState = HIGH;

// New mode name array
const char *modeNames[] = {"Pitch", "Decay", "Velocity", "Cutoff"};

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
  pinMode(modeButton, INPUT_PULLUP);  // Setup the Mode button

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

// Mode button check
  int modeReading = digitalRead(modeButton);
  if (modeReading == LOW && lastModeButtonState == HIGH && (millis() - lastModeDebounceTime) > debounceDelay) {
    lastModeDebounceTime = millis();
    currentMode = (currentMode + 1) % 4;  // Cycle through 0-3

    // Briefly display the current mode
    display.clearDisplay();
    display.setTextSize(2);  // Assuming font size of 2
    int charWidth = 10;  // 10 pixels per character at font size of 2
    
    // Calculate the x position for the mode name
    int modeNameLength = strlen(modeNames[currentMode]);
    int modeNamePixelWidth = modeNameLength * charWidth;
    int modeNameX = (SCREEN_WIDTH - modeNamePixelWidth) / 2;

    // Calculate the x position for the word "Mode"
    int modeWordLength = 4;  // "Mode" has 4 characters
    int modeWordPixelWidth = modeWordLength * charWidth;
    int modeWordX = (SCREEN_WIDTH - modeWordPixelWidth) / 2;

    display.setCursor(modeNameX, LOGO_Y);
    display.println(modeNames[currentMode]);
    
    display.setCursor(modeWordX, LOGO_Y + 16);  // 16 pixels below the first line
    display.println("Mode");
    
    display.display();
    delay(600);
  }
  display.display();
}