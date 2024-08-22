#include <QTRSensors.h>
#include <Arduino.h>

// Definicje czujników
#define NUM_SENSORS 8
#define SENSOR_PINS {13, 12, 14, 27, 26, 25, 33, 32}

const uint8_t sensorPins[NUM_SENSORS] = SENSOR_PINS;
QTRSensors qtr;
uint16_t sensorValues[NUM_SENSORS];

// Definicje pinów
const int PWMA = 15;  // PWM dla kanału A
const int AIN1 = 2;   // Kierunek silnika AIN1
const int AIN2 = 4;   // Kierunek silnika AIN2
const int PWMB = 5;   // PWM dla kanału B
const int BIN1 = 18;  // Kierunek silnika BIN1
const int BIN2 = 19;  // Kierunek silnika BIN2

// PID constants
float Kp = 0.1;
float Ki = 0;
float Kd = 0;

// PID variables
int lastError = 0;
int integral = 0;

// Motor speed variables
const uint8_t maxSpeed = 255;
const uint8_t baseSpeed = 150;

void setup() {
  // Konfiguracja pinów jako wyjścia
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Początkowe wyłączenie silnika
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0);

  Serial.begin(115200);

  // Initialize the sensor
  qtr.setTypeRC();
  qtr.setSensorPins(sensorPins, NUM_SENSORS);
  
  // Kalibracja czujników ok 10s nalezy poruszac czunikiem wystawiajac na najciemniejsze i najjasniejsze punkty
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }
}

void loop() {
  // Odczytanie pozycji linii
  uint16_t position = qtr.readLineBlack(sensorValues);
  
  int error = 3500 - position; // Idealne położenie to 3500

  // PID control
  int P = error;  //from -3500 to 3500
  integral += error;
  int D = error - lastError;
  int output = Kp * P + Ki * integral + Kd * D;
  lastError = error;

  // Motor control
  int leftMotorSpeed = baseSpeed + output;
  int rightMotorSpeed = baseSpeed - output;

  // Constrain the motor speeds to 0-255
  leftMotorSpeed = constrain(leftMotorSpeed, 0, maxSpeed);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, maxSpeed);

  // Set motor speeds
  if (leftMotorSpeed >= 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, leftMotorSpeed);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, -leftMotorSpeed);
  }

  if (rightMotorSpeed >= 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, rightMotorSpeed);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, -rightMotorSpeed);
  }

  // Print the sensor values for debugging
  /*
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.print("Position: ");
  Serial.print(position);
  Serial.print(" Error: ");
  Serial.print(error);
  Serial.print(" left motor: ");
  Serial.println(leftMotorSpeed);
  Serial.print(" right motor: ");
  Serial.println(rightMotorSpeed);
  */
  
  if(position > 3500) {
    Serial.println("RIGHT, pos: ");
    Serial.print(position);
    Serial.print('\t');
    Serial.println(error);
  } else {
    Serial.println("LEFT, pos: ");
    Serial.print(position);
    Serial.print('\t');
    Serial.println(error);
  }
  
  delay(50);  // Adjust delay for smooth operation
}

void analogWrite(int pin, int value) {
  ledcAttachPin(pin, 0);           // Kanał 0
  ledcSetup(0, 5000, 8);           // 5kHz PWM, 8-bitowa rozdzielczość
  ledcWrite(0, value);             // Ustawienie wartości PWM
}
