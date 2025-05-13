#include <SPI.h>

// ********************************************************
// ********************************************************
// Note - make sure the ground pins are connected between 
// the boards. There is too much impedence using the USB 
// lines as a long ground!!!
// 
// "USB CDC on boot" must be set to "Enabled" to activate USBSerial
// 
// ********************************************************
// ********************************************************

SPIClass SPI_Master(FSPI);  // FSPI is the default for ESP32-C3

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting USA ESP32-C3 SPI Master Test...");

  // Initialize SPI bus with custom pins
  SPI_Master.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);

  // Configure CS pin
  pinMode(SPI_CS, OUTPUT);
  digitalWrite(SPI_CS, HIGH);  // Idle state

  Serial.println("SPI Master Initialized");
}

void loop() {
  uint8_t dataToSend[3] = {0xA1, 0xB2, 0xC3};
  uint8_t dataReceived[3] = {0};

  digitalWrite(SPI_CS, LOW);  // Select slave

  for (int i = 0; i < 3; i++) {
    dataReceived[i] = SPI_Master.transfer(dataToSend[i]);
  }

  digitalWrite(SPI_CS, HIGH); // Deselect slave

  Serial.print("Received: ");
  for (int i = 0; i < 3; i++) {
    Serial.printf("0x%02X ", dataReceived[i]);
  }
  Serial.println();

  delay(1000);  // One transaction per second
}

