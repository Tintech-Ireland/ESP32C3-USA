/* ESP32 TWAI receive example.
  Receive messages and sends them over serial.

  Connect a CAN bus transceiver to the RX/TX pins.
  For example: SN65HVD230

  TWAI_MODE_LISTEN_ONLY is used so that the TWAI controller will not influence the bus.

  The API gives other possible speeds and alerts:
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html

  Example output from a can bus message:
  -> Message received
  -> Message is in Standard Format
  -> ID: 604
  -> Byte: 0 = 00, 1 = 0f, 2 = 13, 3 = 02, 4 = 00, 5 = 00, 6 = 08, 7 = 00

  Example output with alerts:
  -> Alert: A (Bit, Stuff, CRC, Form, ACK) error has occurred on the bus.
  -> Bus error count: 171
  -> Alert: The RX queue is full causing a received frame to be lost.
  -> RX buffered: 4  RX missed: 46 RX overrun 0

  created 05-11-2022 by Stephan Martin (designer2k2)
*/

// "USB CDC on boot" must be set to "Enabled" to activate USBSerial

#include <HardwareSerial.h>
#include "driver/twai.h"

// Pins used to connect to CAN bus transceiver:
#define RX_PIN CAN_RX
#define TX_PIN CAN_TX


#define USBSerial Serial

// Intervall:
#define POLLING_RATE_MS 100

static bool driver_installed = false;

void setup() {
  // Start Serial:
  USBSerial.begin(115200) ;
  // USBSerial.setDebugOutput(false);   // debug comes out of the USB serial port - set to false if you want to use
  USBSerial.setDebugOutput(true);   // debug comes out of the USB serial port - set to false if you want to use

  // Initialize configuration structures using macro initializers
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);
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

  // Reconfigure alerts to detect frame receive, Bus-Off error and RX queue full states
  uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR | TWAI_ALERT_RX_QUEUE_FULL;
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

  // TWAI driver is now successfully installed and started
  driver_installed = true;
}

static void handle_rx_message(twai_message_t& message) {
  // Process received message
  if (message.extd) {
    USBSerial.println("Message is in Extended Format");
  } else {
    USBSerial.println("Message is in Standard Format");
  }
  USBSerial.printf("ID: %x\nByte:", message.identifier);
  if (!(message.rtr)) {
    for (int i = 0; i < message.data_length_code; i++) {
      USBSerial.printf(" %d = %02x,", i, message.data[i]);
    }
    USBSerial.println("");
  }

  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    USBSerial.printf("Message queued for transmission\n");
  } else {
    USBSerial.printf("Failed to queue message for transmission\n");
  }

}

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

void loop() {
  if (!driver_installed) {
    // Driver not installed
    delay(1000);
    return;
  }
  // Check if alert happened
  uint32_t alerts_triggered;
  twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(POLLING_RATE_MS));
  twai_status_info_t twaistatus;
  twai_get_status_info(&twaistatus);

  // Handle alerts
  if (alerts_triggered & TWAI_ALERT_ERR_PASS) {
    USBSerial.println("Alert: TWAI controller has become error passive.");
    delay(5000);
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

    delay(5000);
  }
  if (alerts_triggered & TWAI_ALERT_RX_QUEUE_FULL) {
    USBSerial.println("Alert: The RX queue is full causing a received frame to be lost.");
    USBSerial.printf("RX buffered: %d\t", twaistatus.msgs_to_rx);
    USBSerial.printf("RX missed: %d\t", twaistatus.rx_missed_count);
    USBSerial.printf("RX overrun %d\n", twaistatus.rx_overrun_count);
    delay(5000);
  }

  // Check if message is received
  if (alerts_triggered & TWAI_ALERT_RX_DATA) {
    // One or more messages received. Handle all.
    twai_message_t message;
    memset (&message, 0, sizeof(message)) ;
    while (twai_receive(&message, 0) == ESP_OK) {
      handle_rx_message(message);
      memset (&message, 0, sizeof(message)) ;
    }
  }
}
