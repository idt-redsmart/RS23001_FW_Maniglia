
#ifndef BLUETOOTH_
#define BLUETOOTH_

#include <Arduino_BHY2.h>
#include <ArduinoBLE.h>
#include "def.h"
// #include "PrjDefines.h"

class Bluetooth
{
public:
    Bluetooth() : _BLE_deviceInformationService("180A"),
                  _BLE_CONST_modelNumberString("2A24", BLERead, "PicafBLEgateway"),
                  _BLE_CONST_serialNumberString("2A25", BLERead, "00000000"),
                  _BLE_CONST_firmwareRevisionString("2A26", BLERead, FW_VERSION),
                  _BLE_CONST_hardwareRevisionString("2A27", BLERead, HW_VERSION),
                  _BLE_CONST_manufacturerNameString("2A29", BLERead, "idtsolution.com"),
                  _BLE_boardService("1daa0360-88a7-41cd-857d-a22ec7ded851"),
                  _BLE_boolKeepAlive("1daa0361-88a7-41cd-857d-a22ec7ded851", BLERead | BLENotify),
                  _BLE_arrayAuthentication("1daa0362-88a7-41cd-857d-a22ec7ded851", BLERead | BLEWrite, AUTHENTICATION_CHARATERISTIC_SIZE, true) {}

    void setup();
    void authentication();
    void eraseAllBLEcharateristics();
    void printConnectedCentral(BLEDevice _central);
    byte StringToHEX_int(unsigned char *hex_ptr);
    void checkCentralConnected();
    inline void poll() { BLE.poll(); }


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
    // BOARD SERVICE - Charateristics
    BLEService _BLE_boardService;
    // Keep alive - Charateristic
    BLEBoolCharacteristic _BLE_boolKeepAlive;
    // Authentication - Charateristic
    BLECharacteristic _BLE_arrayAuthentication;
};

#endif