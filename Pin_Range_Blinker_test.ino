const int loPin = 34;
const int hiPin = 39;

void setup() {
  // Initialize pins loPin to hiPin as outputs
  for (int pin = loPin; pin <= hiPin; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  // Loop through pins loPin to hiPin and blink the LED
  for (int pin = loPin; pin <= hiPin; pin++) {
    digitalWrite(pin, HIGH);  // Turn LED on
    delay(500);               // Wait for 500 milliseconds
    digitalWrite(pin, LOW);   // Turn LED off
    delay(500);               // Wait for 500 milliseconds
  }
}
