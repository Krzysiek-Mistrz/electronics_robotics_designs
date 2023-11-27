#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(9, 10);

struct MyData {
    byte throttle;
    byte yaw;
    byte pitch;
    byte roll;
    byte AUX1;
    byte AUX2;
};

MyData data;

void resetData()
{
    data.throttle = 0;
    data.yaw = 127;
    data.pitch = 127;
    data.roll = 127;
    data.AUX1 = 0;
    data.AUX2 = 0;
}

void setup()
{
    Serial.begin(9600);
    resetData();
    radio.begin();
    radio.setAutoAck(false);
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, pipeIn);
    radio.startListening();
}

unsigned long lastRecvTime = 0;

void recvData()
{
    while (radio.available() ) {
        radio.read(&data, sizeof(MyData));
        lastRecvTime = millis();
    }
}

void loop() {
    recvData();
    unsigned long now = millis();
    if (now - lastRecvTime > 1000) {
        resetData();
    }
    Serial.print("THROTTLE: "); 
    Serial.print(data.throttle);  
    Serial.print("    ");
    Serial.print("YAW: ");      
    Serial.print(data.yaw);       
    Serial.print("    ");
    Serial.print("PITCH: ");    
    Serial.print(data.pitch);     
    Serial.print("    ");
    Serial.print("ROLL: ");     
    Serial.print(data.roll);      
    Serial.print("    ");
    Serial.print("AUX1: ");     
    Serial.print(data.AUX1);      
    Serial.print("    ");
    Serial.print("AUX2: ");     
    Serial.print(data.AUX2);      
    Serial.print("\n");
}