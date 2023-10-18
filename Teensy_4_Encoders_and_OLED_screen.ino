/*
  Teensy 4.0 Experiments with Rotary Encoders and OLED display
  - Reads values from 4 rotary encoders
  - Displays encoder values on an OLED screen
  - Centers startup text on OLED display
  Written by Nordl3, 2023
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

// Global Settings
#define GLOBAL_FONT_SIZE  2
#define LINE_SPACING  (8 * GLOBAL_FONT_SIZE)
#define COARSE_STEP  1  // Set this to higher values like 2, 3, 4... for coarser steps

Encoder knobA(2, 3);
Encoder knobB(4, 5);
Encoder knobC(6, 7);
Encoder knobD(8, 9);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  Serial.println("4 Knobs Encoder Test:");

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

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

  newA *= COARSE_STEP;
  newB *= COARSE_STEP;
  newC *= COARSE_STEP;
  newD *= COARSE_STEP;

  if (newA != positionA || newB != positionB || newC != positionC || newD != positionD) {
    display.clearDisplay();
    display.setTextSize(GLOBAL_FONT_SIZE);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.print("A=");
    display.println(newA);

    display.setCursor(0, LINE_SPACING);
    display.print("B=");
    display.println(newB);

    display.setCursor(0, 2 * LINE_SPACING);
    display.print("C=");
    display.println(newC);

    display.setCursor(0, 3 * LINE_SPACING);
    display.print("D=");
    display.println(newD);

    display.display();

    positionA = newA;
    positionB = newB;
    positionC = newC;
    positionD = newD;
  }
}
