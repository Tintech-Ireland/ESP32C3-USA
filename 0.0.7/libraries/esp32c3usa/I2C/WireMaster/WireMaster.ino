#include <HardwareSerial.h>
#include <Wire.h>
  
#define I2C_DEV_ADDR 0x55
#define USBSerial Serial

#define I2C_SCL_PIN 3
#define I2C_SDA_PIN 5

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

uint32_t i = 0;

void setup() {
  USBSerial.begin(115200) ;
  // USBSerial.setDebugOutput(false);   // debug comes out of the USB serial port - set to false if you want to use
  USBSerial.setDebugOutput(true);   // debug comes out of the USB serial port - set to false if you want to use

  delay(1000);
  USBSerial.println("Start");
  Wire.setPins(I2C_SDA_PIN, I2C_SCL_PIN) ; 

  Wire.begin();
  delay(1000);

}

void loop() {
  USBSerial.println("Loop Start");
  delay(2000);

  //Write message to the slave
  Wire.beginTransmission(I2C_DEV_ADDR);
  Wire.printf("Hello World! %u", i++);
  uint8_t error = Wire.endTransmission(true);
  USBSerial.printf("endTransmission: %u\n", error);

  //Read 16 bytes from the slave
  uint8_t bytesReceived = Wire.requestFrom(I2C_DEV_ADDR, 16);
  USBSerial.printf("requestFrom: %u\n", bytesReceived);
  if((bool)bytesReceived){ //If received more than zero bytes
    uint8_t temp[bytesReceived];
    Wire.readBytes(temp, bytesReceived);
    log_print_buf(temp, bytesReceived);
  }
}
