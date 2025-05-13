/* ESP32 TWAI transmit example.
  This transmits a message every second.

  Connect a CAN bus transceiver to the RX/TX pins.
  For example: SN65HVD230

  The API gives other possible speeds and alerts:
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html

  created 27-06-2023 by Stephan Martin (designer2k2)
*/

// "USB CDC on boot" must be set to "Enabled" to activate USBSerial

#include <HardwareSerial.h>
#include "driver/twai.h"

#define USBSerial Serial

// Pins used to connect to CAN bus transceiver:
#define RX_PIN CAN_RX
#define TX_PIN CAN_TX

// Intervall:
#define TRANSMIT_RATE_MS 1000

#define POLLING_RATE_MS 1000

static bool driver_installed = false;

unsigned long previousMillis = 0;  // will store last time a message was send

char * getTWAIState (twai_state_t state) {
    char * result = "Unknown" ;

    switch (state) {
      case TWAI_STATE_STOPPED :
        result = "Stopped" ;
        break ;

      case TWAI_STATE_RUNNING :
        result = "Running" ;
      break ;

      case TWAI_STATE_BUS_OFF:
        result = "Bus Off" ;
      break ;

      case TWAI_STATE_RECOVERING:
        result = "Recovering" ;
      break ;
    }

    return (result) ;
}

void setup() {
  // Start Serial:
  USBSerial.begin(115200) ;
  // USBSerial.setDebugOutput(false);   // debug comes out of the USB serial port - set to false if you want to use
  USBSerial.setDebugOutput(true);   // debug comes out of the USB serial port - set to false if you want to use

  // Initialize configuration structures using macro initializers
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);
  // twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NO_ACK);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();  //Look in the api-reference for other speed sets.
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    USBSerial.println("Driver installed");
  } else {
    USBSerial.println("Failed to install driver");
    return;
  }

  // Start TWAI driver
  if (twai_start() == ESP_OK) {
    USBSerial.println("Driver started");
  } else {
    USBSerial.println("Failed to start driver");
    return;
  }

// #if 0
  // Reconfigure alerts to detect TX alerts and Bus-Off errors
  uint32_t alerts_to_enable = TWAI_ALERT_TX_IDLE | TWAI_ALERT_TX_SUCCESS | TWAI_ALERT_TX_FAILED | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR;
  if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
    USBSerial.println("CAN Alerts reconfigured");
  } else {
    USBSerial.println("Failed to reconfigure alerts");
    return;
  }

  twai_status_info_t status;
  twai_get_status_info(&status);
  USBSerial.printf("TWAI state            %s\n",   getTWAIState(status.state));
  delay(10000) ;
  // #endif

  // TWAI driver is now successfully installed and started
  driver_installed = true;

  previousMillis = millis () ;
}

unsigned char counter = 0 ;

static void send_message() {
  // Send message

  // Configure message to transmit
  twai_message_t message;
  memset (&message, 0, sizeof(message)) ;
  message.identifier = 0x0F6aa;
  message.extd = 1;
  // message.ss = 1 ;

  message.data_length_code = 8;
  // for (int i = 0; i < 4; i++) {
  //   message.data[i] = i;
  // }

message.data[0] = 0x54;
message.data[1] = 0x32;
message.data[2] = 0x10;
message.data[3] = counter++ ;
message.data[4] = 0x64;
message.data[5] = 0x42;
message.data[6] = 0x20;
message.data[7] = counter++ ;

  // Queue message for transmission
  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    USBSerial.printf("Message queued for transmission\n");
  } else {
    USBSerial.printf("Failed to queue message for transmission\n");
  }
}

void loop() {
  if (!driver_installed) {
    // Driver not installed
    delay(1000);
    return;
  }

  // Check if alert happened
  int problem = 0 ;

  // #if 0
  uint32_t alerts_triggered;
  twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(POLLING_RATE_MS));
  twai_status_info_t twaistatus;
  twai_get_status_info(&twaistatus);

  // Handle alerts
  if (alerts_triggered & TWAI_ALERT_ERR_PASS) {
    USBSerial.println("Alert: TWAI controller has become error passive.");
    delay(1000);
    problem = 1 ;

  }
  if (alerts_triggered & TWAI_ALERT_BUS_ERROR) {
    USBSerial.println("Alert: A (Bit, Stuff, CRC, Form, ACK) error has occurred on the bus.");
    USBSerial.printf("Bus error count: %d\n", twaistatus.bus_error_count);

    twai_status_info_t status;
    twai_get_status_info(&status);
    USBSerial.printf("TWAI state            %s\n",   getTWAIState(status.state));
    USBSerial.printf("TWAI msgs_to_tx       0x%X\n", status.msgs_to_tx);
    USBSerial.printf("TWAI msgs_to_rx       0x%X\n", status.msgs_to_rx);
    USBSerial.printf("TWAI tx_error_counter 0x%X\n", status.tx_error_counter);
    USBSerial.printf("TWAI rx_error_counter 0x%X\n", status.rx_error_counter);
    USBSerial.printf("TWAI tx_failed_count  0x%X\n", status.tx_failed_count);
    USBSerial.printf("TWAI rx_missed_count  0x%X\n", status.rx_missed_count);
    USBSerial.printf("TWAI arb_lost_count   0x%X\n", status.arb_lost_count);
    USBSerial.printf("TWAI bus_error_count  0x%X\n", status.bus_error_count);
    delay(1000);
    problem = 1 ;
  }
  if (alerts_triggered & TWAI_ALERT_TX_FAILED) {
    USBSerial.println("Alert: The Transmission failed.");
    USBSerial.printf("TX buffered: %d\t", twaistatus.msgs_to_tx);
    USBSerial.printf("TX error: %d\t", twaistatus.tx_error_counter);
    USBSerial.printf("TX failed: %d\n", twaistatus.tx_failed_count);
    delay(1000);
    problem = 1 ;
  }
  if (alerts_triggered & TWAI_ALERT_TX_SUCCESS) {
    USBSerial.println("Alert: The Transmission was successful.");
    USBSerial.printf("TX buffered: %d\t", twaistatus.msgs_to_tx);
    delay(1000);
    problem = 1 ;
  }
  // #endif

  // Send message
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= TRANSMIT_RATE_MS) {
    previousMillis = currentMillis;
    send_message();
  }

  if (problem == 0) {
    delay(1) ;
  }

}
