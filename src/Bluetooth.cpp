
#include <Bluetooth.h>

void Bluetooth::setup() // setup
{
	// begin initialization
	if (!BLE.begin())
	{
		dbg(DBG_BLUETOOTH, "starting BLE failed!");
		while (1)
			;
	}

	dbg(DBG_BLUETOOTH, "BLE setup OK");

	// set the local name and manufactuer data
	// setupBLEdeviceName();

	// set the UUID for the services this peripheral advertises:
	// BLE.setAdvertisedService(BLE_deviceInformationService); //TOGLIEREEEE!!! MANDANO A PUTTANE IL MANUFACTURER DATA!!!!
	// BLE.setAdvertisedService(BLE_boardService); //TOGLIEREEEE!!! MANDANO A PUTTANE IL MANUFACTURER DATA!!!!

	// add the characteristics to the service - BLE_deviceInformationService
	_BLE_deviceInformationService.addCharacteristic(_BLE_CONST_modelNumberString);
	_BLE_deviceInformationService.addCharacteristic(_BLE_CONST_serialNumberString);
	_BLE_deviceInformationService.addCharacteristic(_BLE_CONST_firmwareRevisionString);
	_BLE_deviceInformationService.addCharacteristic(_BLE_CONST_hardwareRevisionString);
	_BLE_deviceInformationService.addCharacteristic(_BLE_CONST_manufacturerNameString);

	// add the characteristics to the service - BLE_boardService
	_BLE_boardService.addCharacteristic(_BLE_boolKeepAlive);
	_BLE_boardService.addCharacteristic(_BLE_arrayAuthentication);
	_BLE_boardService.addCharacteristic(_BLE_StringShockDetect);
	_BLE_boardService.addCharacteristic(_BLE_StringWeight);
	_BLE_boardService.addCharacteristic(_BLE_StringBatteryLevel);
	_BLE_boardService.addCharacteristic(_BLE_StringRequest);

	// add the service
	BLE.addService(_BLE_deviceInformationService);
	BLE.addService(_BLE_boardService);

	// init all charateristics - BLE_boardService
	eraseAllBLEcharateristics();

	// start advertising
	_setupBLEdeviceName();
	_setupBLEmanufacturerData();
	BLE.advertise();

	dbg(DBG_BLUETOOTH, "IoT-shield mac address: " + String(BLE.address()));

	_keepAliveTimer.begin(KEEP_ALIVE_TIMER_VALUE * 1000);
}

/*
This function setup the bluetooth device name, this is shown when the device is discoverable.
We concatenate the BLE_LOCAL_NAME_PREFIX with the last 2 bytes of mac address in order to have a unique name
*/
void Bluetooth::_setupBLEdeviceName()
{
	String mac_add = BLE.address();
	String support = "";
	String deviceName = "";
	// Example mac address string= 11:22:33:44:55:66
	// Extract "55" string
	support = mac_add.substring(12, 14);
	deviceName = String(BLE_LOCAL_NAME_PREFIX) + "_" + support;
	// Extract "66" string
	support = mac_add.substring(15, 17);
	deviceName = deviceName + support;
	_sBleLocalName = deviceName;
	dbg(DBG_BLUETOOTH, "deviceName:" + String(deviceName.c_str()));
	BLE.setLocalName(deviceName.c_str());
}

/*
This function setup the manufacturer data property, we put the mac address of the device to give apple devices unique identifier.
*/
void Bluetooth::_setupBLEmanufacturerData()
{
	String mac_add = BLE.address();
	byte data[8] = {0xA3, 0x09, 0, 0, 0, 0, 0, 0}; // The first 2 byte identify the manufacturer --> "arduino"
	unsigned char mac_byte[2];
	unsigned index = 0;

	for (int i = 0; i < 6; i++)
	{

		mac_byte[0] = mac_add.charAt(index);
		index++;
		mac_byte[1] = mac_add.charAt(index);
		;
		index += 2; // remove the ":"
		data[i + 2] = StringToHEX_int(mac_byte);
	}

	byte manuf_array[30];
	for (int i = 0; i < 8; i++)
	{
		manuf_array[i] = data[i];
	}

	byte manuf_array_effective_len = 0;

	for (uint8_t i = 8; i < sizeof(manuf_array); i++)
	{
		if (_sBleLocalName.charAt(i - 8) != 0)
			manuf_array[i] = (byte)_sBleLocalName.charAt(i - 8);
		else
		{
			manuf_array_effective_len = i; // Index that finds the string stop char of sBleLocalName
			break;
		}
	}
#ifdef BLUETOOTH_DEBUG_

	Serial.print("Manufacturer data value:");
	for (int i = 0; i < manuf_array_effective_len; i++)
	{
		Serial.print(manuf_array[i], HEX);
		if (i < manuf_array_effective_len - 1)
			Serial.print("-");
	}
	Serial.println("");
#endif // BLUETOOTH_DEBUG_

	BLE.setManufacturerData(manuf_array, manuf_array_effective_len);
}

void Bluetooth::_eraseBLEcharateristic(BLECharacteristic &BLECharacteristic)
{
	// create a dynamic array of the size of the charateristic
	char *buffer_p = (char *)malloc(sizeof(char) * BLECharacteristic.valueSize());

	// clean-up the buffer
	for (int i = 0; i < BLECharacteristic.valueSize(); i++)
	{
		buffer_p[i] = 0;
	}

	// write the buffer value to charateristic
	BLECharacteristic.writeValue(buffer_p, BLECharacteristic.valueSize(), false);

	// relase memory
	free(buffer_p);
}

void Bluetooth::eraseAllBLEcharateristics()
{
	_eraseBLEcharateristic(_BLE_boolKeepAlive);
	_eraseBLEcharateristic(_BLE_StringShockDetect);
	_eraseBLEcharateristic(_BLE_StringWeight);
	_eraseBLEcharateristic(_BLE_StringBatteryLevel);
	_eraseBLEcharateristic(_BLE_StringRequest);
}

void Bluetooth::printConnectedCentral(BLEDevice _central)
{
	if (_central.connected())
	{
#ifdef BLUETOOTH_DEBUG_
		Serial.print("New central device connected. MAC ADDRESS:");
		Serial.println(_central.address());
#endif // BLUETOOTH_DEBUG_
	}
	else
	{
#ifdef BLUETOOTH_DEBUG_
		Serial.println("Central device disconnected");
#endif // BLUETOOTH_DEBUG_
	}
}

byte Bluetooth::StringToHEX_int(unsigned char *hex_ptr)
{
	byte hex_val = 0;
	bool shift = 1;
	for (int i = 0; i < 2; i++)
	{
		if (hex_ptr[i] == '1')
			hex_val += (1 << 4 * shift);
		else if (hex_ptr[i] == '2')
			hex_val += (2 << 4 * shift);
		else if (hex_ptr[i] == '3')
			hex_val += (3 << 4 * shift);
		else if (hex_ptr[i] == '4')
			hex_val += (4 << 4 * shift);
		else if (hex_ptr[i] == '5')
			hex_val += (5 << 4 * shift);
		else if (hex_ptr[i] == '6')
			hex_val += (6 << 4 * shift);
		else if (hex_ptr[i] == '7')
			hex_val += (7 << 4 * shift);
		else if (hex_ptr[i] == '8')
			hex_val += (8 << 4 * shift);
		else if (hex_ptr[i] == '9')
			hex_val += (9 << 4 * shift);
		else if (hex_ptr[i] == 'a' || hex_ptr[i] == 'A')
			hex_val += (0xA << 4 * shift);
		else if (hex_ptr[i] == 'b' || hex_ptr[i] == 'B')
			hex_val += (0xB << 4 * shift);
		else if (hex_ptr[i] == 'c' || hex_ptr[i] == 'C')
			hex_val += (0xC << 4 * shift);
		else if (hex_ptr[i] == 'd' || hex_ptr[i] == 'D')
			hex_val += (0xD << 4 * shift);
		else if (hex_ptr[i] == 'e' || hex_ptr[i] == 'E')
			hex_val += (0xE << 4 * shift);
		else if (hex_ptr[i] == 'f' || hex_ptr[i] == 'F')
			hex_val += (0xF << 4 * shift);

		shift = 0;
	}
	return hex_val;
}

void Bluetooth::authentication()
{
	char aInData[AUTHENTICATION_CHARATERISTIC_SIZE];
	if (_bFLAG_CentralConnected && !_bFLAG_Authenticated)
	{
		if (_BLE_arrayAuthentication.written())
		{
			// calculate the key value --> mac address + crc32
			String mac_add = BLE.address();
			byte macAddress[16]; // mac address ascii char (6*2=12) + 4 crc32
			byte mac_byte[2];
			unsigned index = 0;
			bool bEqual = false;

			for (int i = 0; i < 12; i++)
			{

				mac_byte[0] = mac_add.charAt(index);
				index++;
				mac_byte[1] = mac_add.charAt(index);
				;
				index += 2; // remove the ":"
				macAddress[i] = StringToHEX_int(mac_byte);
			}

			_BLE_arrayAuthentication.readValue(aInData, AUTHENTICATION_CHARATERISTIC_SIZE);

#ifdef BLUETOOTH_DEBUG_
			Serial.print("authentication in value:");
			for (int i = 0; i < 6; i++)
			{
				Serial.print(aInData[i], HEX);
				Serial.print(".");
			}
			Serial.println("");

			Serial.print("mac address value:");
			for (int i = 0; i < 6; i++)
			{
				Serial.print(macAddress[i], HEX);
				Serial.print(".");
			}
			Serial.println();

#endif

			bEqual = true;
			// compare received mac
			/*for(int i=0; i<6; i++)
			{
				if(((aInData[i]-i)& 0xFF) != (macAddress[i]))
				{
					Serial.println(aInData[i],HEX);
					Serial.println(aInData[i]-i,HEX);
					bEqual = false;
				}
			}*/

			// clean charateristic
			for (int i = 0; i < AUTHENTICATION_CHARATERISTIC_SIZE; i++)
			{
				aInData[i] = 0;
			}

			// authentication success
			if (bEqual)
			{
				aInData[0] = 0x31;
				_bFLAG_Authenticated = true;
				ledLoopPhase = 2;
				ledStatusUpdated = true;
				dbg(DBG_LED, "LEDLOOP PHASE = 2");
				dbg(DBG_BLUETOOTH, "autentication success");
			}
			// autentication failed
			else
			{
				aInData[0] = 0x30;
				dbg(DBG_BLUETOOTH, "autentication failed");
			}

			_BLE_arrayAuthentication.writeValue(aInData, AUTHENTICATION_CHARATERISTIC_SIZE, true);
		}
	}

	if (!_bFLAG_CentralConnected && ledLoopPhase != 0)
	{
		_bFLAG_Authenticated = false;
		ledLoopPhase = 0;
		dbg(DBG_LED, "LEDLOOP PHASE = 0");
	}
}

void Bluetooth::checkCentralConnected()
{

	// Check Central device connection
	if (_bFLAG_CentralConnected != BLE.connected())
	{
		_bFLAG_CentralConnected = BLE.connected();
		printConnectedCentral(BLE.central());
		// digitalWrite(PIN_LED_CONNECTED, bFLAG_CentralConnected);

		// If new connection is established clear all charateristics values
		if (_bFLAG_CentralConnected)
			eraseAllBLEcharateristics();
	}

	if (_bFLAG_CentralConnected == 1 && ledLoopPhase == 0)
	{
		ledLoopPhase = 1;
		ledStatusUpdated = true;
		dbg(DBG_LED, "LEDLOOP PHASE = 1");
	}
}

void Bluetooth::keepAlive()
{
	if (!_bFLAG_CentralConnected || !_bFLAG_Authenticated)
		return;

	_keepAliveTimer.start();
	if (_keepAliveTimer.fire())
	{
		_keepAliveTimer.stop();

		_bFLAG_KeepAlive = !_bFLAG_KeepAlive;
		_BLE_boolKeepAlive.writeValue(_bFLAG_KeepAlive);

		dbg(DBG_BLUETOOTH, "Keep alive: " + String(_bFLAG_KeepAlive) + "\n");
	}
}

void Bluetooth::writeShockDetect()
{
	if (!_bFLAG_CentralConnected || !_bFLAG_Authenticated || !Shock.readNewData())
		return;

	dbg(DBG_BLUETOOTH, "Sending shock data: " + Shock.readShockData() + "\n");

	_BLE_StringShockDetect.writeValue(Shock.readShockData().c_str(), SHOCK_DATA_SIZE, false);
	Shock.writeNewData(false);
	Shock.writeShockData(SHOCK_DATA_DEFAULT_VALUE);
}

void Bluetooth::readRequest()
{
	if (!_BLE_StringRequest.written() || !_bFLAG_CentralConnected || !_bFLAG_Authenticated)
		return;

	char requestDataIn[2]; // = REQUEST_DATA_DEFAULT_VALUE;
	_BLE_StringRequest.readValue(requestDataIn, REQUEST_DATA_SIZE);
	// dbg(DBG_BLUETOOTH, "REQ IN: " + String(requestDataIn));

	if (requestDataIn[0] == 'P' && requestDataIn[1] == '0')
		writeWeight();
	else if (requestDataIn[0] == 'F' && requestDataIn[1] == '0')
		findRequest(0);
	else if (requestDataIn[0] == 'F' && requestDataIn[1] == '1')
		findRequest(1);

	dbg(DBG_BLUETOOTH, "Request incoming: " + String(requestDataIn[0]) + String(requestDataIn[1]) + "\n");

	_eraseBLEcharateristic(_BLE_StringRequest);
}

void Bluetooth::writeWeight()
{
	float tmpWeight = Scale.getWeight();

	int tmp = tmpWeight / 1;
	String scaleDataTmp = "P;" + String(tmp);
	tmpWeight -= tmp;
	tmpWeight = tmpWeight * 100;

	tmp = tmpWeight / 1;

	Scale.writeScaleData(scaleDataTmp + "." + String(tmp));

	dbg(DBG_BLUETOOTH, "Sending load cell data: " + Scale.readScaleData() + "\n");

	_BLE_StringWeight.writeValue(Scale.readScaleData().c_str(), LOAD_CELL_DATA_SIZE, false);
}

void Bluetooth::findRequest(bool sound)
{
	if (!sound)
	{
		_findReq = 1;

		ledLoopPhase = 3;
		ledStatusUpdated = true;
		dbg(DBG_LED, "LEDLOOP PHASE = 3");
	}
	else
	{
		_findReq = 2;
		// SUONA
	}
}

void Bluetooth::writeBatteryLevel()
{
	if (!_bFLAG_CentralConnected || !_bFLAG_Authenticated)
		return;

	if (!_firstWeightSent)
	{
		_firstWeightSent = true;

		Battery.getLevel();
		String data_tmp = Battery.readData(); //"B;OUT";  // Battery.getStringBatteryLevel();

		_BLE_StringBatteryLevel.writeValue(data_tmp.c_str(), BATTERY_DATA_SIZE, false);

		Battery.writeData("");

#ifdef BLUETOOTH_DEBUG_
		Serial.println(/*"\n*/ "Fault reg:");
		for (int x = 7; x >= 0; x--)
			Serial.print(bitRead(nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_FAULTS), x));
		Serial.print("\n");

		Serial.println("Sending battery level: " + data_tmp);
		// Serial.println("Fault register:");
		// Serial.println(nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_FAULTS), BIN);
#endif
	}
	else
	{
		Battery.startTimer();
		if (Battery.fireTimer())
		{
			Battery.stopTimer();

			Battery.getLevel();
			String data_tmp = Battery.readData(); //"B;OUT";  // Battery.getStringBatteryLevel();

			_BLE_StringBatteryLevel.writeValue(data_tmp.c_str(), BATTERY_DATA_SIZE, false);

			Battery.writeData("");

#ifdef BLUETOOTH_DEBUG_
			Serial.println(/*"\n*/ "Fault reg: 0x01");
			for (int x = 7; x >= 0; x--)
				Serial.print(bitRead(nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_FAULTS), x));
			Serial.print("\n");

			Serial.println("Sending battery level: " + data_tmp);
			// Serial.println("Fault register:");
			// Serial.println(nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_FAULTS), BIN);

#endif
		}
	}
}
