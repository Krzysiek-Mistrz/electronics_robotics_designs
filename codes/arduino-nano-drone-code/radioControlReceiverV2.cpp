#include <Wire.h>
#include <MPU6050.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define channel_number 6
#define sigPin 2
#define PPM_FrLen 27000
#define PPM_PulseLen 400

int ppm[channel_number];

const uint64_t pipeIn =  0xE8E8F0F0E1LL;

RF24 radio(9, 10);

struct MyData {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
  bool autonomousMode;
};

MyData data;

MPU6050 mpu;

void resetData() {
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2= 0;
  data.autonomousMode = false;

  setPPMValuesFromData();
}

void setPPMValuesFromData() {
  ppm[0] = map(data.throttle, 0, 255, 1000, 2000);
  ppm[1] = map(data.yaw,      0, 255, 1000, 2000);
  ppm[2] = map(data.pitch,    0, 255, 1000, 2000);
  ppm[3] = map(data.roll,     0, 255, 1000, 2000);
  ppm[4] = map(data.AUX1,     0, 1, 1000, 2000);
  ppm[5] = map(data.AUX2,     0, 1, 1000, 2000);  
}

void utrzymajWysokosc() {
  // Odczyty z modułu MPU6050
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Obliczenia przechowujące przyspieszenie grawitacyjne
  double accGrav = sqrt(ax * ax + ay * ay + az * az);

  // Docelowa wartość przyspieszenia grawitacyjnego
  double targetAccGrav = 9.81;

  // Regulacja thrustu w oparciu o różnicę między docelowym a aktualnym przyspieszeniem
  int thrust = map(constrain(targetAccGrav - accGrav, -2, 2), -2, 2, 1000, 2000);

  // Ustawianie thrustu w tablicy PPM
  ppm[0] = thrust;

  // Pozostałe wartości PPM pozostaw bez zmian
  ppm[1] = map(data.yaw,      0, 255, 1000, 2000);
  ppm[2] = map(data.pitch,    0, 255, 1000, 2000);
  ppm[3] = map(data.roll,     0, 255, 1000, 2000);
  ppm[4] = map(data.AUX1,     0, 1, 1000, 2000);
  ppm[5] = map(data.AUX2,     0, 1, 1000, 2000);
}

void setupPPM() {
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, 0);

  cli();
  TCCR1A = 0;
  TCCR1B = 0;

  OCR1A = 100;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11);
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

void setup() {
  resetData();
  setupPPM();

  Wire.begin();
  mpu.initialize();

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);

  radio.openReadingPipe(1,pipeIn);
  radio.startListening();
}

unsigned long lastRecvTime = 0;

void recvData() {
  while ( radio.available() ) {        
    radio.read(&data, sizeof(MyData));
    lastRecvTime = millis();
  }
}

void loop() {
  recvData();

  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    resetData();
  }
  
  if (data.autonomousMode) {
    utrzymajWysokosc();
  } else {
    setPPMValuesFromData();
  }
}

#define clockMultiplier 2

ISR(TIMER1_COMPA_vect) {
  static boolean state = true;

  TCNT1 = 0;

  if ( state ) {
    PORTD = PORTD & ~B00000100;
    OCR1A = PPM_PulseLen * clockMultiplier;
    state = false;
  }
  else {
    static byte cur_chan_numb;
    static unsigned int calc_rest;

    PORTD = PORTD | B00000100;
    state = true;

    if(cur_chan_numb >= channel_number) {
      cur_chan_numb = 0;
      calc_rest += PPM_PulseLen;
      OCR1A = (PPM_FrLen - calc_rest) * clockMultiplier;
      calc_rest = 0;
    }
    else {
      OCR1A = (ppm[cur_chan_numb] - PPM_PulseLen) * clockMultiplier;
      calc_rest += ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}
