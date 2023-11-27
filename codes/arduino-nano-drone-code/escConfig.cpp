/*ESC calibration*/
#include <Servo.h>
#include <Arduino.h>

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000
#define MOTOR_PIN 9
int DELAY = 1000;
Servo motor;

void setup() {
  Serial.begin(9600);
  Serial.println("ESC CALIBRATION...");
  Serial.println(" ");
  delay(1500);
  Serial.println("...CALIBRATION...");
  delay(1000);
  motor.attach(MOTOR_PIN);
  Serial.print("WRITING MAXIMUM OUTPUT: (");
  Serial.print(MAX_SIGNAL);
  Serial.print("\n");
  motor.writeMicroseconds(MAX_SIGNAL);
  Serial.println("\n");
  Serial.println("\n");
  Serial.print("WRITING MINIMUM OUTPUT: (");
  Serial.print(MIN_SIGNAL);
  Serial.print("\n");
  motor.writeMicroseconds(MIN_SIGNAL);
  Serial.println("===ESC CALIBRATION SUCCESS===");
  Serial.println("TYPE VALUE BETWEEN 1000 AND 2000:");
}

void loop() {
  if (Serial.available() > 0)
  {
    int DELAY = Serial.parseInt();
    if (DELAY > 999)
    {
      motor.writeMicroseconds(DELAY);
      float SPEED = (DELAY-1000)/10;
      Serial.print("\n");
      Serial.println("MOTOR SPEED:"); 
      Serial.print("  "); 
      Serial.print(SPEED); 
      Serial.print("%"); 
    }     
  }
}