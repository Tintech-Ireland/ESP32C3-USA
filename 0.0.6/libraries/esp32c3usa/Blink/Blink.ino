#include <HardwareSerial.h>

// the setup function runs once when you press reset or power the board
HardwareSerial SerialPort(0); // use UART0
#define USBSerial Serial

// int my_vprintf(const char *format, va_list args) {
//     char *log_buffer; 
//     size_t len = vasprintf(&log_buffer, format, args);
//     SerialPort.println(log_buffer);
//     return len;
// }

// TWO serial ports - one goes out on UART, the Other on the USB 
// "USB CDC on boot" must be set to "disabled" to activate USBSerial
void setup() {
  SerialPort.begin(115200, SERIAL_8N1);
  USBSerial.begin(115200) ;
  SerialPort.setDebugOutput(false); // stops debug coming out of the serial port
  USBSerial.setDebugOutput(true);   // debug comes out of the USB serial port - set to false if you want to use
                                    // it for something else
  delay(1000);

  esp_log_level_set("*", ESP_LOG_ERROR);  

  // Build settings have NO effect on these
  log_e("log_e %s", "a string error");
  log_w("log_w %s", "a string warning");
  log_i("log_i %s", "a string info");
  log_d("log_d %s", "a string debug");
  
  SerialPort.println("Start Serial");
  ESP_LOGI("blink", "Start Log blink");
  USBSerial.println("Start USB");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(10, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(10, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(2000);                      // wait for a second
  digitalWrite(10, LOW);   // turn the LED off by making the voltage LOW
  delay(2000);                      // wait for a second
  SerialPort.println("Blinks serial");
  USBSerial.println("Blinks USB");
  log_d("log_d %s", "Loop string debug");
  ESP_LOGI("blink", "Loop Log blink");
}