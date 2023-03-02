
#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <def.h>
#include <ShockDetection.h>
#include <LoadCell.h>
#include <Battery.h>

extern ShockDetection Shock;
extern LoadCell Scale;
extern Battery Bat;


class Bluetooth
{
public:
    Bluetooth() : _BLE_deviceInformationService("180A"),
                  _BLE_CONST_modelNumberString("2A24", BLERead, "PicafBLEgateway"),
                  _BLE_CONST_serialNumberString("2A25", BLERead, "00000000"),
                  _BLE_CONST_firmwareRevisionString("2A26", BLERead, FW_VERSION),
                  _BLE_CONST_hardwareRevisionString("2A27", BLERead, HW_VERSION),
                  _BLE_CONST_manufacturerNameString("2A29", BLERead, "idtsolution.com"),
                  //board service
                  _BLE_boardService("1daa0360-88a7-41cd-857d-a22ec7ded851"),
                  _BLE_boolKeepAlive("1daa0361-88a7-41cd-857d-a22ec7ded851", BLERead | BLENotify),
                  _BLE_arrayAuthentication("1daa0362-88a7-41cd-857d-a22ec7ded851", BLERead | BLEWrite, AUTHENTICATION_CHARATERISTIC_SIZE, true),
                  _BLE_StringShockDetect("1daa0363-88a7-41cd-857d-a22ec7ded851", BLERead | BLEWrite | BLENotify, SHOCK_DATA_SIZE, false),
                  _BLE_StringWeight("1daa0364-88a7-41cd-857d-a22ec7ded851", BLERead | BLEWrite , LOAD_CELL_DATA_SIZE, false),
                  _BLE_StringBatteryLevel("1daa0365-88a7-41cd-857d-a22ec7ded851", BLERead | BLEWrite | BLENotify , BATTERY_DATA_SIZE, false),
                  _BLE_StringRequest("1daa0366-88a7-41cd-857d-a22ec7ded851", BLERead | BLEWrite , REQUEST_DATA_SIZE, false) {}

    void setup();
    void authentication();
    void eraseAllBLEcharateristics();
    void printConnectedCentral(BLEDevice _central);
    byte StringToHEX_int(unsigned char *hex_ptr);
    void checkCentralConnected();
    void poll() { BLE.poll(); }
    void writeShockDetect();
    void writeWeight();
    void writeBatteryLevel();

private:
    void _setupBLEdeviceName();
    void _setupBLEmanufacturerData();
    void _eraseBLEcharateristic(BLECharacteristic &BLECharacteristic);

    String _sBleLocalName = "";

    bool _bFLAG_CentralConnected; // true = central connected, false = central NOT connected
    bool _bFLAG_KeepAlive;
    bool _bFLAG_Authenticated;

    // DEVICE INFO SERVICE
    BLEService _BLE_deviceInformationService;
    // DEVICE INFO SERVICE - Charateristics
    //  Model number String - Charateristic
    BLECharacteristic _BLE_CONST_modelNumberString;
    // Serial number String - Charateristic
    BLECharacteristic _BLE_CONST_serialNumberString;
    // Firmware revision String - Charateristic
    BLECharacteristic _BLE_CONST_firmwareRevisionString;
    // Hardware revision String - Charateristic
    BLECharacteristic _BLE_CONST_hardwareRevisionString;
    // Manufacturer Name String - Charateristic
    BLECharacteristic _BLE_CONST_manufacturerNameString;

    // BOARD SERVICE
    BLEService _BLE_boardService;
    // BOARD SERVICE - Charateristics
    // Keep alive - Charateristic
    BLEBoolCharacteristic _BLE_boolKeepAlive;
    // Authentication - Charateristic
    BLECharacteristic _BLE_arrayAuthentication;
    // Shock detection - Charateristic
    BLECharacteristic _BLE_StringShockDetect;
    // Load cell - Charateristic
    BLECharacteristic _BLE_StringWeight;
    // Battery - Charateristic
    BLECharacteristic _BLE_StringBatteryLevel;
    // Request from app - Charateristic
    BLECharacteristic _BLE_StringRequest;

};

#endif