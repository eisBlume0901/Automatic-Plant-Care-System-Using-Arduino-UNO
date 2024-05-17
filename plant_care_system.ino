// Notes
// Components that only require 5V
// DHT, LCD, Water Level,

// Components that requires 9V, add another breadboard, note they should not share the same ground or else it would lead to
// short circuiting
// Stepper Motor


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
int displayTime = 2000;

#include <IRremote.hpp>
#define IR_RECEIVE_PIN 3
String hexValue;

#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temperature;
float minTemp = 18; // Chili plant minimum temperature
float maxTemp = 30; // Chili plant maximum temperature
float humidity;

int waterLevelSensorPin = A0;
int maxWater = 270; // Water level threshold for the buzzer to send signal (0 - 1023 reading)

int buzzPin = 4;

int triggerPin = 7;
int echoPin = 5;
#define PULSE_TIME 10
int pingDuration = 50;
int catDistance = 35; // cm

#include <Servo.h>
Servo servo;
int servoPin = 6;

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  dht.begin();

  pinMode(waterLevelSensorPin, INPUT);

  pinMode(buzzPin, OUTPUT);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servo.attach(servoPin);
  servo.write(90);
}

void loop()
{
  lcd.clear();

  // activateCatBarrier();
  
  alertHighWaterLevel();

  displayTemperatureAndHumidity();


  // Put soil moisture and automatic irrigation here instead of remote 
  // There will be a soilThreshold so that it will automatically detect if it needs water
  
  if (IrReceiver.decode()) {
    hexValue = String(IrReceiver.decodedIRData.decodedRawData, HEX);
    hexValue.toUpperCase();

    if (hexValue == "E916FF00") { // Number 1

    }

    if (hexValue == "E619FF00") {
      Serial.println("It is number 2");
    }

    if (hexValue == "F20DFF00") {
      Serial.println("It is number 3");
    }

    if (hexValue == "F30CFF00") {
      Serial.println("It is number 4");
    }

    if (hexValue == "E718FF00") {
      Serial.println("It is number 5");
    }

    if (hexValue == "A15EFF00") {
      Serial.println("It is number 6");
    }

    if (hexValue == "F708FF00") {
      Serial.println("It is number 7");
    }

    if (hexValue == "E31CFF00") {
      Serial.println("It is number 8");
    }

    if (hexValue == "A55AFF00") {
      Serial.println("It is number 9");
    }

    if (hexValue == "AD52FF00") {
      Serial.println("It is number 0");
    }

    if (hexValue == "BD42FF00") {
      Serial.println("It is an asterisk");
    }

    if (hexValue == "B54AFF00") {
      Serial.println("It is a hash");
    }

    if (hexValue == "BF40FF00") {
      Serial.println("It is an OK");
    }

    if (hexValue == "BB44FF00") {
      Serial.println("It is arrow left");
    }

    if (hexValue == "BC43FF00") {
      Serial.println("It is arrow right");
    }

    if (hexValue == "B946FF00") {
      Serial.println("It is arrow up");
    }      

    if (hexValue == "EA15FF00") {
      Serial.println("It is arrow down");
    }

    IrReceiver.resume(); // Enable receiving of the next value

  }
}

// Only reads the temperature and humidity
void displayTemperatureAndHumidity(void) {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  lcd.setCursor(0, 0);
  lcd.print("Temp       ");
  lcd.print(temperature);
  lcd.setCursor(0, 1);
  lcd.print("Humid      ");
  lcd.print(humidity);
  delay(displayTime);
  lcd.clear();
}

// For water dispenser of the plant
// alerting user either for water leakage, rainfall, tank overflow
void alertHighWaterLevel(void) {
  int waterLevel = analogRead(waterLevelSensorPin);
  int alarmDuration = 200;

  
  while (maxWater <= waterLevel) {
    lcd.setCursor(0, 0);
    lcd.print("Water Leakage");
    lcd.setCursor(0, 1);
    lcd.print("Drain water");
    delay(displayTime);
    lcd.clear();

    digitalWrite(buzzPin, HIGH);
    delay(alarmDuration);
    digitalWrite(buzzPin, LOW);
    delay(alarmDuration);

    waterLevel = analogRead(waterLevelSensorPin);
  }
}

void activateCatBarrier() {

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(PULSE_TIME);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(PULSE_TIME);
  digitalWrite(triggerPin, LOW);
  int pingTravelTime = pulseIn(echoPin, HIGH);

  float pingInCM = pingTravelTime / 58;

  while (pingInCM <= catDistance) {
    lcd.setCursor(0, 0);
    lcd.print("Activating");
    lcd.setCursor(0, 1);
    lcd.print("Cat Barrier");
    delay(displayTime);
    lcd.clear();

    servo.write(0);
    
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(PULSE_TIME);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(PULSE_TIME);
    digitalWrite(triggerPin, LOW);
    pingTravelTime = pulseIn(echoPin, HIGH);
    pingInCM = pingTravelTime / 58;

    if (pingInCM > catDistance) {
      servo.write(90);
      break;
    }
  }
}
