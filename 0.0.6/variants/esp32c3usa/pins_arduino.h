#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

// I2C
#define  PIN_WIRE_SDA 5 
#define  PIN_WIRE_SCL 3 

// SPI
#define  SPI_MISO 1  
#define  SPI_MOSI 2  
#define  SPI_SCK  0  
#define  SPI_CS   10 

// UART
#define  SERIAL0_TX 18 
#define  SERIAL0_RX 19 

// Additional serials
#define  RS232_RX 4 
#define  RS232_TX 5 

#define  RS485_RX 8 
#define  RS485_TX 9 

// CAN
#define  CAN_RX 6 
#define  CAN_TX 7 

#define TX SERIAL0_TX 
#define RX SERIAL0_RX 

#define SDA PIN_WIRE_SDA
#define SCL PIN_WIRE_SCL

#define MOSI SPI_MOSI
#define MISO SPI_MISO
#define SCK  SPI_SCK 
#define SS   SPI_CS  

// LED
#define LED_BUILTIN 10

#define RGB_BUILTIN    LED_BUILTIN
#define RGB_BRIGHTNESS 64

// Not used features
#define TOUCH_PIN  -1
#define ADC_PIN    -1
#define DAC_PIN    -1

#endif /* Pins_Arduino_h */
