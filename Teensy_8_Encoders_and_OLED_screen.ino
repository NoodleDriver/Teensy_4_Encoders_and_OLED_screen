/*
  Teensy 4.0 Experiments with 8 PDVC Rotary Encoders and OLED Display
  - Reads values from 8 rotary encoders (A, B, C, D, E, F, G, H)
  - Scrolls through a truncated set of musical notes (C2 to C4) based on encoder positions
  - Displays the musical note corresponding to each encoder on the OLED screen
  - Uses a global variable to set a default start note (C2 in this example)
  - Encoder values are mapped to note names and displayed on the screen
  
  Hardware:
  - Teensy 4.0 board
  - 8 rotary encoders (A, B, C, D, E, F, G, H)
  - OLED display (128x64)
  
  Libraries:
  - Adafruit_SSD1306 (For the OLED)
  - Encoder (For reading the rotary encoders)
  - SPI (For communication)
  - Wire (For I2C communication)

  Written by Nordl3, 2023
*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

// Global Settings
#define GLOBAL_FONT_SIZE 1
#define LINE_SPACING (8 * GLOBAL_FONT_SIZE)

int defaultNoteIndex = 36;  // C2 is at index 36

// Define all the encoders
Encoder knobA(2, 3);
Encoder knobB(4, 5);
Encoder knobC(6, 7);
Encoder knobD(8, 9);
Encoder knobE(24, 25);
Encoder knobF(26, 27);
Encoder knobG(28, 29);
Encoder knobH(30, 31);

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Truncated note array (C2 to C4)
const char *notes[] = {
  "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
  "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
  "C4"
};

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);  // Hang if OLED allocation fails
  }
  
  // Initialize encoders
  knobA.write(defaultNoteIndex * 4);
  knobB.write(defaultNoteIndex * 4);
  knobC.write(defaultNoteIndex * 4);
  knobD.write(defaultNoteIndex * 4);
  knobE.write(defaultNoteIndex * 4);
  knobF.write(defaultNoteIndex * 4);
  knobG.write(defaultNoteIndex * 4);
  knobH.write(defaultNoteIndex * 4);

  display.clearDisplay();
  display.setTextSize(GLOBAL_FONT_SIZE);
  display.setTextColor(SSD1306_WHITE);
  int textWidth = 6 * GLOBAL_FONT_SIZE * 6;
  display.setCursor((SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - 8 * GLOBAL_FONT_SIZE) / 2);
  display.println(F("Nordl3"));
  display.display();
  delay(1000);

  display.clearDisplay();
}

long positions[8] = {-999, -999, -999, -999, -999, -999, -999, -999};

void loop() {
  Encoder* knobs[8] = {&knobA, &knobB, &knobC, &knobD, &knobE, &knobF, &knobG, &knobH};
  char knobNames[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

  long newPos[8];
  bool anyChange = false;

  for (int i = 0; i < 8; i++) {
    newPos[i] = knobs[i]->read() / 4;
    if (newPos[i] != positions[i]) {
      anyChange = true;
    }
  }

  if (anyChange) {
    display.clearDisplay();
    display.setTextSize(GLOBAL_FONT_SIZE);
    display.setTextColor(SSD1306_WHITE);
    
    for (int i = 0; i < 8; i++) {
      display.setCursor(0, i * LINE_SPACING);
      display.print(knobNames[i]);
      display.print("=");
      display.println(notes[newPos[i] % (sizeof(notes) / sizeof(notes[0]))]);
      positions[i] = newPos[i];
    }
    
    display.display();
  }
}
