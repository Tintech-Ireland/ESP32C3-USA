// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __ESP_BT_DEFS_H__
#define __ESP_BT_DEFS_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Status Return Value
typedef enum {
    ESP_BT_STATUS_SUCCESS          =  0,          /* Successful operation. */
    ESP_BT_STATUS_FAILURE          =  1,          /* Generic failure. */
    ESP_BT_STATUS_PENDING          =  2,          /* API cannot be completed right now */
    ESP_BT_STATUS_BUSY             =  3,
    ESP_BT_STATUS_NO_RESOURCES     =  4,
    ESP_BT_STATUS_WRONG_MODE       =  5,
} esp_bt_status_t;


/*Define the bt octet 16 bit size*/
#define ESP_BT_OCTET16_LEN    16
typedef uint8_t esp_bt_octet16_t[ESP_BT_OCTET16_LEN];   /* octet array: size 16 */

#define ESP_BT_OCTET8_LEN    8
typedef uint8_t esp_bt_octet8_t[ESP_BT_OCTET8_LEN];   /* octet array: size 8 */

typedef uint8_t esp_link_key[ESP_BT_OCTET16_LEN];      /* Link Key */

/// Default GATT interface id
#define ESP_DEFAULT_GATT_IF             0xff

/// Default BLE connection param, if the value doesn't be overwritten
#define ESP_BLE_CONN_PARAM_UNDEF        0xffff      /* use this value when a specific value not to be overwritten */

/// Check the param is valid or not
#define ESP_BLE_IS_VALID_PARAM(x, min, max)  (((x) >= (min) && (x) <= (max)) || ((x) == ESP_BLE_CONN_PARAM_UNDEF))

/// UUID type
typedef struct {
#define ESP_UUID_LEN_16     2
#define ESP_UUID_LEN_32     4
#define ESP_UUID_LEN_128    16
    uint16_t len;							/*!< UUID length, 16bit, 32bit or 128bit */
    union {
        uint16_t    uuid16;
        uint32_t    uuid32;
        uint8_t     uuid128[ESP_UUID_LEN_128];
    } uuid;									/*!< UUID */
} __attribute__((packed)) esp_bt_uuid_t;

/// Bluetooth device type
typedef enum {
    ESP_BT_DEVICE_TYPE_BREDR   = 0x01,
    ESP_BT_DEVICE_TYPE_BLE     = 0x02,
    ESP_BT_DEVICE_TYPE_DUMO    = 0x03,
} esp_bt_dev_type_t;

/// Bluetooth address length
#define ESP_BD_ADDR_LEN     6

/// Bluetooth device address
typedef uint8_t esp_bd_addr_t[ESP_BD_ADDR_LEN];

/// BLE device address type
typedef enum {
    BLE_ADDR_TYPE_PUBLIC        = 0x00,
    BLE_ADDR_TYPE_RANDOM        = 0x01,
    BLE_ADDR_TYPE_RPA_PUBLIC    = 0x02,
    BLE_ADDR_TYPE_RPA_RANDOM    = 0x03,
} esp_ble_addr_type_t;

/// Used to exchange the encrytyption key in the init key & response key
#define ESP_BLE_ENC_KEY_MASK    (1 << 0)
/// Used to exchange the IRK key in the init key & response key
#define ESP_BLE_ID_KEY_MASK     (1 << 1)
/// Used to exchange the CSRK key in the init key & response key
#define ESP_BLE_CSR_KEY_MASK    (1 << 2)
/// Used to exchange the link key(this key just used in the BLE & BR/EDR coexist mode) in the init key & response key
#define ESP_BLE_LINK_KEY_MASK   (1 << 3)

/// Minimum of the application id
#define ESP_APP_ID_MIN  0x0000
/// Maximum of the application id
#define ESP_APP_ID_MAX  0x7fff

#define ESP_BD_ADDR_STR         "%02x:%02x:%02x:%02x:%02x:%02x"
#define ESP_BD_ADDR_HEX(addr)   addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]

#ifdef __cplusplus
}
#endif

#endif     /* __ESP_BT_DEFS_H__ */
