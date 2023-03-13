#ifndef DEF_H
#define DEF_H

// MAIN
#include <Arduino.h>
#include <Nicla_System.h>
#include <FireTimer.h>

// BLE
#include <Arduino_BHY2.h>
#include <ArduinoBLE.h>

// LOAD CELL
#include <HX711.h>

// LED
#include <Led/NiclaSenseNeoPixel.h>

extern void dbg(int index, String str);

// DEBUG
#define MAIN_DEBUG_
#define BATTERY_DEBUG_
#define BLUETOOTH_DEBUG_
#define LOAD_CELL_DEBUG_
#define SHOCK_DEBUG_
#define LED_DEBUG_

// LOAD CELL
#define LOAD_CELL_A_DOUT_PIN P0_1
#define LOAD_CELL_A_SCK_PIN P0_2
#define LOAD_CELL_B_DOUT_PIN P0_3
#define LOAD_CELL_B_SCK_PIN P0_4
#define LOAD_CELL_CALIBRATION_FACTOR 100
#define LOAD_CELL_DATA_SIZE 7
#define LOAD_CELL_DATA_DEFAULT_VALUE "P;00.00"

// BLE
// Local name
#define BLE_LOCAL_NAME_PREFIX "RS23001"
#define FW_VERSION "1.0"
#define HW_VERSION "1.0"

#define AUTHENTICATION_CHARATERISTIC_SIZE 50 // 50 byte

// KEEP ALIVE
#define KEEP_ALIVE_TIMER_VALUE 30 // SECONDI

// SHOCK DETECTION
#define SHOCK_THRESHOLD 700
#define SHOCK_DEBOUNCE_TIME 200
#define SHOCK_NUM_SAMPLE 10
#define SHOCK_SCALE_VALUE 10
#define SHOCK_DATA_SIZE 16
#define SHOCK_DATA_DEFAULT_VALUE "U;0000;0000;0000"

// BATTERY
#define BATTERY_FREQ_DATA_SEND 60 // SECONDI
#define BATTERY_DATA_SIZE 5
#define BATTERY_DATA_DEFAULT_VALUE "B;000"
#define BATTERY_CORRECTION_READ 14

// REQUEST
#define REQUEST_DATA_SIZE 2
#define REQUEST_DATA_DEFAULT_VALUE ""

// LED
#define LED_PIN P0_10
#define LED_NUM 2

#define LED_ON_TIME_NOT_CON 200                            // MS
#define LED_OFF_TIME_NOT_CON (15000 - LED_ON_TIME_NOT_CON) // MS
#define LED_ON_TIME 200                                    // MS
#define LED_OFF_TIME (5000 - LED_ON_TIME)                  // MS
#define LED_FIND_TIMER_VALUE 5                             // SECONDI

#define LED_STATUS_NOT_CONNECTED 0
#define LED_STATUS_NOT_AUTH 1
#define LED_STATUS_AUTH 2
#define LED_STATUS_FIND 3

#define LED_COLOR_OFF 0
#define LED_COLOR_RED 1
#define LED_COLOR_GREEN 2
#define LED_COLOR_BLUE 3
// #define LED_COLOR_BLUE_PULSE 4

// DBG
#define DBG_MAIN 0
#define DBG_BATTERY 1
#define DBG_BLUETOOTH 2
#define DBG_LOAD_CELL 3
#define DBG_SHOCK 4
#define DBG_LED 5

#endif