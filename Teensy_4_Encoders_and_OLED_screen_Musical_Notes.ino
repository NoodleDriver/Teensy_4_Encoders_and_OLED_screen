/*
  Teensy 4.0 Experiments with Rotary Encoders and OLED display
  - Reads values from 4 rotary encoders
  - Scrolls through musical notes based on encoder positions
  - Centers startup text on OLED display
  - Global variable for default start note (C3 in this example)
  Written by Nordl3, 2023
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

// Global Settings
#define GLOBAL_FONT_SIZE 2
#define LINE_SPACING (8 * GLOBAL_FONT_SIZE)
//#define COARSE_STEP  1  // Set this to higher values like 2, 3, 4... for coarser steps

int defaultNoteIndex = 60;  // Index of C3 in the notes array

Encoder knobA(2, 3);
Encoder knobB(4, 5);
Encoder knobC(6, 7);
Encoder knobD(8, 9);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *notes[] = {
  "C-2", "C#-2", "D-2", "D#-2", "E-2", "F-2", "F#-2", "G-2", "G#-2", "A-2", "A#-2", "B-2",
  "C-1", "C#-1", "D-1", "D#-1", "E-1", "F-1", "F#-1", "G-1", "G#-1", "A-1", "A#-1", "B-1",
  "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
  "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
  "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
  "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
  "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
  "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
  "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
  "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",
  "C8"
};


void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;)
      ;  // Hang if OLED allocation fails
  }

  knobA.write(defaultNoteIndex * 4);
  knobB.write(defaultNoteIndex * 4);
  knobC.write(defaultNoteIndex * 4);
  knobD.write(defaultNoteIndex * 4);

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

long positionA = -999;
long positionB = -999;
long positionC = -999;
long positionD = -999;

void loop() {
  long newA, newB, newC, newD;
  newA = knobA.read() / 4;
  newB = knobB.read() / 4;
  newC = knobC.read() / 4;
  newD = knobD.read() / 4;

  // COARSE_STEP is not used anymore, so we can remove the following lines:
  // newA *= COARSE_STEP;
  // newB *= COARSE_STEP;
  // newC *= COARSE_STEP;
  // newD *= COARSE_STEP;

  if (newA != positionA || newB != positionB || newC != positionC || newD != positionD) {
    display.clearDisplay();
    display.setTextSize(GLOBAL_FONT_SIZE);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.print("A=");
    display.println(notes[newA % (sizeof(notes) / sizeof(notes[0]))]);

    display.setCursor(0, LINE_SPACING);
    display.print("B=");
    display.println(notes[newB % (sizeof(notes) / sizeof(notes[0]))]);

    display.setCursor(0, 2 * LINE_SPACING);
    display.print("C=");
    display.println(notes[newC % (sizeof(notes) / sizeof(notes[0]))]);

    display.setCursor(0, 3 * LINE_SPACING);
    display.print("D=");
    display.println(notes[newD % (sizeof(notes) / sizeof(notes[0]))]);

    display.display();

    positionA = newA;
    positionB = newB;
    positionC = newC;
    positionD = newD;
  }
}

