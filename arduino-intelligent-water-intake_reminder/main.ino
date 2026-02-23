#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x20, 16, 2); //lcd
const int tiltSensorPin = 2;


const int greenLedPin = 3;
const int yellowLedPin = 4;
const int vibrationPin = 5;
const int redLedPin = 6;
const int buzzerPin = 7;

//---------------Time Tuning from here--------------
const unsigned long reminderInterval = 30000; 
const unsigned long greenTime = 10000; 
const unsigned long yellowTime = 10000;
const unsigned long redBlinkInterval = 5000;

//----------------States-----------------------
enum SystemState {
  IDLE,
  GREEN_ALERT,
  YELLOW_ALERT,
  RED_ALERT
};

SystemState currentState = IDLE;
SystemState lastDisplayedState = IDLE;

//-----------Initial Varibales---------------
unsigned long lastDrinkTime = 0;
unsigned long stateStartTime = 0;
unsigned long lastRedToggle = 0;
bool redOn = false;

bool lastTiltState = HIGH;





void setup() {
  Serial.begin(9600);

  pinMode(tiltSensorPin, INPUT_PULLUP);

  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(vibrationPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  stopAllOutputs();
  

  lastDrinkTime = millis();
  Serial.println("System Initialising");
  
  //LCD part
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water Reminder");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  
  Serial.println("System Ready");
}





void loop() {
  checkTiltSensor();
  handleStateMachine();
  updateLCD();
}



//-------------------TILT SENSOR-----------------------
bool bottlePicked() {
  return digitalRead(tiltSensorPin) == LOW;  // low means active because we used INPUT_PULLUP
}

void checkTiltSensor() {
  bool currentTilt = digitalRead(tiltSensorPin);

  if (currentTilt != lastTiltState) {
    delay(20); //debounse
    lastTiltState = currentTilt;

    if (currentTilt == LOW) {
      Serial.println("----------------------------------");
      Serial.println("Bottle picked up");
      Serial.println("Resetting the System to default");
      Serial.println("----------------------------------");

      lastDrinkTime = millis();
      currentState = IDLE;
      stopAllOutputs();

    }
  }
}

//----------------STATES HANDLING USING CASES-------------
void handleStateMachine() {
  unsigned long now = millis();

  switch (currentState) {

    case IDLE:
      if (now - lastDrinkTime >= reminderInterval) {
        Serial.println("----------------------------------");
        Serial.println("No drinking detected");
        Serial.println("Green LED ON");
        Serial.println("----------------------------------");

        digitalWrite(greenLedPin, HIGH);
        currentState = GREEN_ALERT;
        stateStartTime = now;
      }
      break;

    case GREEN_ALERT:
      if (bottlePicked()) return;

      if (now - stateStartTime >= greenTime) {
        Serial.println("Yellow LED + Vibration ON");

        digitalWrite(greenLedPin, LOW);
        digitalWrite(yellowLedPin, HIGH);
        digitalWrite(vibrationPin, HIGH);

        currentState = YELLOW_ALERT;
        stateStartTime = now;
      }
      break;

    case YELLOW_ALERT:
      if (bottlePicked()) return;

      if (now - stateStartTime >= yellowTime) {
        Serial.println("----------------------------------");
        Serial.println("Red LED + Buzzer ON");

        digitalWrite(yellowLedPin, LOW);
        digitalWrite(vibrationPin, LOW);

        currentState = RED_ALERT;
        lastRedToggle = now;
      }
      break;

    case RED_ALERT:
      if (bottlePicked()) return;

      if (now - lastRedToggle >= redBlinkInterval) {
        lastRedToggle = now;
        redOn = !redOn;

        if (redOn) {
          digitalWrite(redLedPin, HIGH);
          tone(buzzerPin, 1000);
        } else {
          digitalWrite(redLedPin, LOW);
          noTone(buzzerPin);
        }
      }
      break;
  }
}



//---------------LCD Function---------------------

void updateLCD() {
  if (currentState == lastDisplayedState) return;

  lcd.clear();


  switch (currentState) {
    case IDLE:
      lcd.setCursor(0, 0);
      lcd.print("Bottle Picked");
      lcd.setCursor(0, 1);
      lcd.print("Up :)");
      break;

    case GREEN_ALERT:
      lcd.setCursor(0, 0);
      lcd.print("Time to Drink");
      lcd.setCursor(0, 1);
      lcd.print("Water :)");
      break;

    case YELLOW_ALERT:
      lcd.setCursor(0, 0);
      lcd.print("Dehydration :(");
      lcd.setCursor(0, 1);
      lcd.print("Detected");
      break;

    case RED_ALERT:
      lcd.setCursor(0, 0);
      lcd.print("DRINK WATER NOW");
      lcd.setCursor(0, 1);
      lcd.print("PLEASE !!!!");
      break;
    
  
  }

  lastDisplayedState = currentState;
}

// ------------------------------------//

void stopAllOutputs() {
  digitalWrite(greenLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(redLedPin, LOW);
  digitalWrite(vibrationPin, LOW);
  noTone(buzzerPin);

  Serial.println("All turned OFF");
}
