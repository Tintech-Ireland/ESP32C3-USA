#include <HardwareSerial.h>

// the setup function runs once when you press reset or power the board
// "USB CDC on boot" must be set to "Enabled" to activate USBSerial

HardwareSerial SerialPort(0); // use UART0
#define USBSerial Serial

#define UART_RX_PIN_RS232  RS232_RX
#define UART_TX_PIN_RS232  RS232_TX

#define UART_RX_PIN_RS485  RS485_RX
#define UART_TX_PIN_RS485  RS485_TX

// Change this depending on which port you want to test

// To test RS232
// #define UART_RX_PIN  UART_RX_PIN_RS232
// #define UART_TX_PIN  UART_TX_PIN_RS232

// To test RS485
// #define UART_RX_PIN  UART_RX_PIN_RS485
// #define UART_TX_PIN  UART_TX_PIN_RS485

#define UART_RX_PIN  UART_RX_PIN_RS485
#define UART_TX_PIN  UART_TX_PIN_RS485

// int my_vprintf(const char *format, va_list args) {
//     char *log_buffer;
//     size_t len = vasprintf(&log_buffer, format, args);
//     SerialPort.println(log_buffer);
//     return len;
// }

// NOTE ************************************************************
// This fails if you set the arduino IDE core debug level
// to verbose!!!!!
// If you do this for some reason it stops sending
// NOTE ************************************************************

void setup() {
  SerialPort.begin(9600, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
  USBSerial.begin(115200) ;
  SerialPort.setDebugOutput(false); // stops debug coming out of the serial port
  USBSerial.setDebugOutput(true);   // debug comes out of the USB serial port - set to false if you want to use
                                    // it for something else
  delay(1000);

  SerialPort.println("Start Serial");

  USBSerial.println("Start USB");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(10, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(10, HIGH);  // turn the LED on (HIGH is the voltage level)

  SerialPort.println("Hello There\r\n");
  USBSerial.println("Sending Data");

  delay(2000);                      // wait for a bit
  digitalWrite(10, LOW);   // turn the LED off by making the voltage LOW

}