#include <bluefruit.h>

#include <PDM.h>

int buttonPin = 9;  // Replace with your actual button pin
int ledPin = 5;     // Replace with your actual LED pin
int s = 0;
unsigned long lastPressTime = 0;
unsigned long pressDuration = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Start");
}

long startpress = 0;
void loop() {
  int b = digitalRead(buttonPin);
  if (s == 0 && pressed(b)) {  // s is state
    s = 1;
    digitalWrite(ledPin, HIGH);  // on
  } else if (s == 1 && !pressed(b)) {
    s = 2;
  } else if (s == 2 && pressed(b)) {

    if (startpress == 0) {
      startpress = millis();
    } else {
      if (millis() - startpress > 1000) {
        digitalWrite(ledPin, LOW);  // off
        s = 3;
      }
    }
  } else if (s == 3 && !pressed(b)) {
    s = 0;
    startpress = 0;
  } else {
    startpress = 0;
  }
}

bool pressed(int i) {
  if (i == 1) {
    return true;
  }
  return false;
}