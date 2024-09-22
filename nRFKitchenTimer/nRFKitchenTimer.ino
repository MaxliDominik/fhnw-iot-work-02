#include <PDM.h>
#include <TM1637.h>

TM1637 display(9, 10);  // Adjust pin numbers as needed
int8_t TimeDisp[] = { 0x00, 0x00, 0x00, 0x00 };

int buttonPin = 7;  // Replace with your actual button pin
int buzzerPin = 5;  // Replace with your actual buzzer pin

const int minutesPerHour = 60;
const int secondsPerMinute = 60;

int state = 0;  // 0: Idle, 1: Setting minutes, 2: Setting seconds, 3: Counting down, 4: Buzzing
int minutes = 0;
int seconds = 0;
long startTime = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  //Serial.begin(115200);
  display.init();
  display.set(7);  // Adjust brightness as needed
  display.point(1);
}

void loop() {
  int button = digitalRead(buttonPin);
  switch (state) {
       case 0: // Idle
      if (button == LOW) {
        state = 1;
        setDisplay(minutes, seconds);
        delay(1000);
      }
      break;
    case 1: // Setting minutes
      if (button == HIGH) {
        minutes++;
        if (minutes >= minutesPerHour) {
          minutes = 0;
        }
        setDisplay(minutes, seconds);
        delay(200);
      } else if (button == LOW) {
        state = 2;
        delay(1000);
      }
      break;
    case 2: // Setting seconds
      if (button == HIGH) {
        seconds++;
        if (seconds >= secondsPerMinute) {
          seconds = 0;
        }
        setDisplay(minutes, seconds);
        delay(200);
      } else if (button == LOW) {
        state = 3;
        startTime = millis();
        delay(1000);
      }
      break;
    case 3:
      {  // Counting down
        long elapsedTime = millis() - startTime;
        int remainingMinutes = (minutes * secondsPerMinute + seconds - elapsedTime / 1000) / secondsPerMinute;
        int remainingSeconds = (minutes * secondsPerMinute + seconds - elapsedTime / 1000) % secondsPerMinute;
        setDisplay(remainingMinutes, remainingSeconds);
        if (remainingMinutes == 0 && remainingSeconds == 0) {
          state = 4;
        }
        break;
      }
    case 4:
      {  // Buzzing
        digitalWrite(buzzerPin, HIGH);
        if (button == LOW) {
          digitalWrite(buzzerPin, LOW);
          state = 0;
          minutes = 0;
          seconds = 0;
        }
        break;
      }
  }
  display.display(TimeDisp);
}
void setDisplay(int min, int sec) {
  TimeDisp[0] = min / 10;
  TimeDisp[1] = min % 10;
  TimeDisp[2] = sec / 10;
  TimeDisp[3] = sec % 10;
}
