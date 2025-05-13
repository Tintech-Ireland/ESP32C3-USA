#include <HardwareSerial.h>
#include <Wire.h>

// "USB CDC on boot" must be set to "Enabled" to activate USBSerial

#define I2C_DEV_ADDR 0x55
#define USBSerial Serial

#define I2C_SCL_PIN PIN_WIRE_SCL
#define I2C_SDA_PIN PIN_WIRE_SDA


uint32_t i = 0;

/*
USA Pinout
   3.3V             EN
GPIO  0             GPIO  1
GPIO  2         SCL GPIO  3
GPIO  4         SDA GPIO  5
GPIO  6             GPIO  7
GPIO  8             GPIO  9
     TX             RX
GPIO 10             GND
*/



void onRequest(){
  Wire.print(i++);
  Wire.print(" Packets.");
  USBSerial.println("onRequest");
}

void onReceive(int len){
  USBSerial.printf("onReceive[%d]: ", len);
  while(Wire.available()){
    USBSerial.write(Wire.read());
  }
  USBSerial.println();
}

void setup() {
  USBSerial.begin(115200);
  // USBSerial.setDebugOutput(false);   // debug comes out of the USB serial port - set to false if you want to use
  USBSerial.setDebugOutput(true);   // debug comes out of the USB serial port - set to false if you want to use
  delay(1000);

  USBSerial.println("Start");
   // Serial.setDebugOutput(true);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);

  Wire.setPins(I2C_SDA_PIN, I2C_SCL_PIN) ; 
  
  Wire.begin((uint8_t)I2C_DEV_ADDR);

#if CONFIG_IDF_TARGET_ESP32
  char message[64];
  snprintf(message, 64, "%u Packets.", i++);
  Wire.slaveWrite((uint8_t *)message, strlen(message));
#endif
}

void loop() {

}
