#include <SPI.h>

#define PIN_MISO SPI_MISO
#define PIN_MOSI SPI_MOSI
#define PIN_SCK  SPI_SCK 
#define PIN_CS   SPI_CS  

SPIClass SPI_Master(FSPI);  // FSPI is the default for ESP32-C3

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting USA ESP32-C3 SPI Master Test...");

  // Initialize SPI bus with custom pins
  SPI_Master.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

  // Configure CS pin
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH);  // Idle state

  Serial.println("SPI Master Initialized");
}

void loop() {
  uint8_t dataToSend[3] = {0xA1, 0xB2, 0xC3};
  uint8_t dataReceived[3] = {0};

  digitalWrite(PIN_CS, LOW);  // Select slave

  for (int i = 0; i < 3; i++) {
    dataReceived[i] = SPI_Master.transfer(dataToSend[i]);
  }

  digitalWrite(PIN_CS, HIGH); // Deselect slave

  Serial.print("Received: ");
  for (int i = 0; i < 3; i++) {
    Serial.printf("0x%02X ", dataReceived[i]);
  }
  Serial.println();

  delay(1000);  // One transaction per second
}

