const int buttons[] = {34, 35, 36, 37, 38, 39, 22, 23, 32, 33};
const char* buttonNames[] = {"A", "B", "C", "D", "E", "F", "G", "H", "CLKDIV", "STEPS"};
const int numButtons = 10;

int lastButtonState[numButtons];
int buttonState[numButtons];
unsigned long lastDebounceTime[numButtons];
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    lastButtonState[i] = HIGH;
  }
}

void loop() {
  for (int i = 0; i < numButtons; i++) {
    int reading = digitalRead(buttons[i]);
    
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        
        if (buttonState[i] == LOW) {
          Serial.println(buttonNames[i]);
        }
      }
    }
    
    lastButtonState[i] = reading;
  }
}
