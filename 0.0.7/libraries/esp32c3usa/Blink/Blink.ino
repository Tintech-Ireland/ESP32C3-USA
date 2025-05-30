#include <HardwareSerial.h>

// the setup function runs once when you press reset or power the board
// "USB CDC on boot" must be set to "Enabled" to activate USBSerial

HardwareSerial SerialPort(0); // use UART0
#define USBSerial Serial

// int my_vprintf(const char *format, va_list args) {
//     char *log_buffer; 
//     size_t len = vasprintf(&log_buffer, format, args);
//     SerialPort.println(log_buffer);
//     return len;
// }

// TWO serial ports - one goes out on UART, the Other on the USB 
void setup() {
  SerialPort.begin(115200, SERIAL_8N1);
  USBSerial.begin(115200) ;
  SerialPort.setDebugOutput(false); // stops debug coming out of the serial port
  USBSerial.setDebugOutput(true);   // debug comes out of the USB serial port - set to false if you want to use
                                    // it for something else
  delay(1000);

   // Build settings have NO effect on these
  log_e("log_e %s", "a string error");
  log_w("log_w %s", "a string warning");
  log_i("log_i %s", "a string info");
  log_d("log_d %s", "a string debug");
  
  SerialPort.println("Start Serial");
 
  USBSerial.println("Start USB");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(2000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(2000);                      // wait for a second
  SerialPort.println("Blinks serial");
  USBSerial.printf("Blinks USB again\r\n");
  log_d("log_d %s", "Loop string debug");
}