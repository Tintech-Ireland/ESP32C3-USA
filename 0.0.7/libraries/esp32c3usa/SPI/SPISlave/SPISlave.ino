#include <SPI.h>
#include "driver/spi_slave.h"

#define SPI_HOST  ((spi_host_device_t)1)  // SPI2_HOST
// defined in pins_arduino.h
// #define SPI_MISO  SPI_MISO
// #define SPI_MOSI  SPI_MOSI
// #define SPI_SCLK  SPI_SCK 
// #define SPI_CS    SPI_CS  
#define BUFFER_SIZE 4

uint8_t tx_data[BUFFER_SIZE] = {0x35, 0x01, 0x01};
uint8_t rx_data[BUFFER_SIZE] = {0};

HardwareSerial Serial2(1); // Available to talk to another device

void setup() {
  // pinMode(LED_BUILTIN, OUTPUT); 
  Serial.begin(115200);
  while (!Serial); // Wait for serial connection
  // Serial.setDebugOutput(false); // stops debug coming out of the serial port
  Serial.setDebugOutput(true); // stops debug coming out of the serial port


  delay(5000); // Keep your long delay for consistency
  Serial.println("Starting USA ESP32-C3 SPI Slave Test...");

  spi_bus_config_t buscfg = {
    .mosi_io_num = SPI_MOSI,
    .miso_io_num = SPI_MISO,
    .sclk_io_num = SPI_SCK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = BUFFER_SIZE
  };

  spi_slave_interface_config_t slvcfg = {
    .spics_io_num = SPI_CS,
    .flags = 0,
    .queue_size = 1,
    .mode = 0,
    .post_setup_cb = NULL,
    .post_trans_cb = NULL
  };

  // Initialize Serial2 with custom pins
  //Serial2.begin(115200, SERIAL_8N1, RS232_RX, RS232_TX);
  Serial.println("Serial 2 initialized");
  delay(1000);

  esp_err_t ret = spi_slave_initialize(SPI_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
  if (ret != ESP_OK) {
    Serial.printf("SPI init failed: %d\n", ret);
    while (1); // Halt
  }
  Serial.println("SPI Slave (SPI2) initialized");
  delay(1000);
}

void loop() {
  spi_slave_transaction_t trans = {0};
  trans.length = BUFFER_SIZE * 8;
  trans.tx_buffer = tx_data;
  trans.rx_buffer = rx_data;

  esp_err_t ret = spi_slave_transmit(SPI_HOST, &trans, portMAX_DELAY);
  if (ret == ESP_OK) {
    Serial.print("Received SPI: ");
    for (int i = 0; i < 3; i++) {
        Serial.printf("%02X ", rx_data[i]);
        // Serial2.write (rx_data[i]) ;
    }
    Serial.println("");
  }

  delay(10); // small delay to avoid thrashing
}



