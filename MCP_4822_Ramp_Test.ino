/**
 * Teensy 4.0 with Two MCP4822 DACs Ramp-Up Test Sequence
 * 
 * This script drives two MCP4822 DACs connected to a Teensy 4.0 board over SPI.
 * It generates a ramp-up signal on each of the four output channels (two channels per DAC),
 * turning off each channel immediately after the ramp-up to make sure only one is active at any time.
 * 
 * Hardware Setup:
 * - MCP4822 #1 (Left) connected to Teensy SPI with CS on Pin 10
 * - MCP4822 #2 (Right) connected to Teensy SPI with CS on Pin 12
 * - Shared SPI MOSI on Pin 11
 * - Shared SPI CLK on Pin 13
 */

#include <SPI.h>

const int CS_PIN1 = 10; // Left MCP4822 DAC
const int CS_PIN2 = 12; // Right MCP4822 DAC
const unsigned long RAMP_MICRO_DELAY = 150; // Delay in microseconds for each step in ramping
const uint16_t RAMP_MIN = 0; // Min possible output Voltage
const uint16_t RAMP_MAX = 4095; // Max possible output Voltage

unsigned long lastRampStepTime; // Variable to hold the time of the last ramp step

// Setup code here
void setup() {
  SPI.begin();
  pinMode(CS_PIN1, OUTPUT);
  pinMode(CS_PIN2, OUTPUT);
  digitalWrite(CS_PIN1, HIGH);
  digitalWrite(CS_PIN2, HIGH);
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
}

// Function to write data to the DAC
void writeDAC(int csPin, uint8_t channel, uint16_t value) {
  uint16_t packet = (channel << 15) | (0b11 << 12) | (value & 0x0FFF);
  digitalWrite(csPin, LOW);
  SPI.transfer16(packet);
  digitalWrite(csPin, HIGH);
}

// Function to generate ramp-up signal
void generateRamp(int csPin, uint8_t channel) {
  writeDAC(csPin, channel, 0);  // Set channel to low at the start of each ramp
  lastRampStepTime = micros();
  for (uint16_t i = RAMP_MIN; i <= RAMP_MAX; i++) {
    while (micros() - lastRampStepTime < RAMP_MICRO_DELAY); // Wait until the time has elapsed
    lastRampStepTime = micros();
    writeDAC(csPin, channel, i);
  }
  writeDAC(csPin, channel, 0);  // Turn off immediately after ramping up
}

// Main loop
void loop() {
  generateRamp(CS_PIN1, 0);
  generateRamp(CS_PIN1, 1);
  generateRamp(CS_PIN2, 0);
  generateRamp(CS_PIN2, 1);
}
