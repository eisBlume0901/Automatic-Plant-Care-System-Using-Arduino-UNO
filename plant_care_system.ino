 // Notes
  // Components that only require 5V
  // DHT, LCD, Water Level, Soil Moisture

  // Components that requires 9V, add another breadboard, note they should not share the same ground or else it would lead to
  // short circuiting
  // DC Motor with Fan

  // Plant Parameters
  float minTemp = 18; // Chili plant minimum temperature
  float maxTemp = 35; // Chili plant maximum temperature
  float maxHumid = 60; // Chili plant maximum humidity
  float minHumid = 55; // Chili plant minimum humidity
  float maxSoilMoisture = 530;

  boolean retracted = false;


  // LCD
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
  int displayTime = 2000;

  // Temperature and Humidity
  #include <DHT.h>
  #define DHTPIN 4
  #define DHTTYPE DHT11
  DHT dht(DHTPIN, DHTTYPE);
  float temperature;
  float humidity;

  // Water Level Sensor
  #define waterLevelSensorPin A0
  int maxWater = 270; // Water level threshold for the buzzer to send signal (0 - 1023 reading)

  // Buzzer
  int buzzPin = 8;

  // Servo
  #include <Servo.h>
  Servo servo;
  int servoPin = 6;


  // DC motor with fan
  #define speedPin 5
  #define directionPin1 2
  #define directionPin2 7
  #define fanMotorSpeed 255

  // Water Pump and Soil Moisture
  #define soilMoisturePin A1
  #define pumpPin 3
  float soilMoisture;
  int waterDuration = 5000; // 5 seconds


  void setup()
  {
    lcd.init(); // initialize the lcd
    lcd.backlight();

    dht.begin();

    pinMode(waterLevelSensorPin, INPUT);

    pinMode(buzzPin, OUTPUT);

    servo.attach(servoPin);

    pinMode(speedPin, OUTPUT);
    pinMode(directionPin1, OUTPUT);
    pinMode(directionPin2, OUTPUT);
  }

  void loop()
  {
    lcd.clear();

    displayTemperatureAndHumidity();

    deploySunlightShade();

    activateFan();

    alertHighWaterLevel();
  }

  // Only reads the temperature and humidity
  void displayTemperatureAndHumidity(void) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    lcd.setCursor(0, 0);
    lcd.print("Room Temp  ");
    lcd.print(temperature);
    lcd.setCursor(0, 1);
    lcd.print("Humidity   ");
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

  void activateFan(void) {

    // Range between 55 to 60
    while (humidity >= minHumid && humidity <= maxHumid) {
      humidity = dht.readHumidity();
      turnOnFan();

      if (minHumid < humidity && humidity > maxHumid) {
        turnOffFan();
        break;
      }
    }

  } 
    

  void turnOnFan(void) {
    lcd.setCursor(0, 0);
    lcd.print("Activating fan!");
    delay(displayTime);
    lcd.clear();

    digitalWrite(directionPin1, HIGH);
    digitalWrite(directionPin2, LOW);
    analogWrite(speedPin, fanMotorSpeed);
  }

  void turnOffFan(void) {
    lcd.setCursor(0, 0);
    lcd.print("Turning off fan!");
    delay(displayTime);
    lcd.clear();

    digitalWrite(directionPin1, LOW);
    digitalWrite(directionPin2, LOW);
    analogWrite(speedPin, 0);
  }

  
  void deploySunlightShade(void) {
    int delayTime = 2000;
    if (temperature < minTemp && retracted == false) {
      lcd.setCursor(0,0);
      lcd.print("Removing");
      lcd.setCursor(0, 1);
      lcd.print(" sunlight shade");
      delay(displayTime);

      servo.write(0);
      delay(delayTime);
      servo.write(90); 
      retracted = true;
      temperature = dht.readTemperature();

    } else if (temperature > maxTemp && retracted == true) {
      lcd.setCursor(0,0);
      lcd.print("Deploying");
      lcd.setCursor(0, 1);
      lcd.print(" sunlight shade");
      delay(displayTime);
      
      servo.write(180);
      delay(delayTime); 
      servo.write(90);
      retracted = false;
      temperature = dht.readTemperature();
    }
  }
