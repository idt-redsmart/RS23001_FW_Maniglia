#include "Bluetooth.h"



void Bluetooth::setup()//setup
{
	// begin initialization
	if (!BLE.begin())
	{
#ifdef DEBUG
		Serial.println("starting BLE failed!");
#endif // DEBUG
		while (1)
			;
	}

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
	// _BLE_boardService.addCharacteristic(_BLE_arrayOutputData);
	// _BLE_boardService.addCharacteristic(_BLE_arrayInputData);
	_BLE_boardService.addCharacteristic(_BLE_boolKeepAlive);
	_BLE_boardService.addCharacteristic(_BLE_arrayAuthentication);

	// add the service
	BLE.addService(_BLE_deviceInformationService);
	BLE.addService(_BLE_boardService);

	// init all charateristics - BLE_boardService
	eraseAllBLEcharateristics();

	// start advertising
	_setupBLEdeviceName();
	_setupBLEmanufacturerData();
	BLE.advertise();

#ifdef DEBUG
	Serial.println("IoT-shield mac address: " + String(BLE.address()));
#endif // DEBUG
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
	DEBUG.println("deviceName:" + String(deviceName.c_str()));
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
#ifdef DEBUG

	DEBUG.print("Manufacturer data value:");
	for (int i = 0; i < manuf_array_effective_len; i++)
	{
		DEBUG.print(manuf_array[i], HEX);
		if (i < manuf_array_effective_len - 1)
			DEBUG.print("-");
	}
	DEBUG.println("");
#endif // DEBUG

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
	// eraseBLEcharateristic(BLE_arrayOutputData);
	// eraseBLEcharateristic(BLE_arrayInputData);
	_eraseBLEcharateristic(_BLE_boolKeepAlive);
}

void Bluetooth::printConnectedCentral(BLEDevice _central)
{
	if (_central.connected())
	{
#ifdef DEBUG
		DEBUG.print("New central device connected. MAC ADDRESS:");
		DEBUG.println(_central.address());
#endif // DEBUG
	}
	else
	{
#ifdef DEBUG
		DEBUG.println("Central device disconnected");
#endif // DEBUG
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
	if(_bFLAG_CentralConnected && !_bFLAG_Authenticated)
	{
		if(_BLE_arrayAuthentication.written())
			{
				//calculate the key value --> mac address + crc32
				String mac_add = BLE.address();
				byte macAddress[16]; //mac address ascii char (6*2=12) + 4 crc32
				byte mac_byte[2];
				unsigned index = 0;		
				bool bEqual = false;	

				for (int i = 0; i < 12; i++) {

					mac_byte[0] = mac_add.charAt(index);
					index++;
					mac_byte[1] = mac_add.charAt(index); ;
					index += 2;		//remove the ":"
					macAddress[i] = StringToHEX_int(mac_byte);
				}
				
   				_BLE_arrayAuthentication.readValue(aInData, AUTHENTICATION_CHARATERISTIC_SIZE);
				
				#ifdef DEBUG
				DEBUG.print("authentication in value:");
				for(int i=0; i<6; i++)
				{
					DEBUG.print(aInData[i],HEX);
					DEBUG.print(".");
				}
				DEBUG.println("");

				DEBUG.print("mac address value:");
				for(int i=0; i<6; i++)
 				{
					DEBUG.print(macAddress[i],HEX);
					DEBUG.print(".");
				}
				DEBUG.println();

				#endif

				bEqual = true;
				//compare received mac
				/*for(int i=0; i<6; i++)
				{
					if(((aInData[i]-i)& 0xFF) != (macAddress[i]))
					{
						DEBUG.println(aInData[i],HEX);
						DEBUG.println(aInData[i]-i,HEX);
						bEqual = false;
					}
				}*/

				//clean charateristic
				for(int i=0; i<AUTHENTICATION_CHARATERISTIC_SIZE; i++)
				{
					aInData[i]= 0;
				}

				
				//authentication success
				if(bEqual)
				{
					aInData[0]=0x31;
					_bFLAG_Authenticated = true;
					#ifdef DEBUG
					DEBUG.println("autentication success");
					#endif
				}
				//autentication failed
				else
				{
					aInData[0]=0x30;
					#ifdef DEBUG
					DEBUG.println("autentication failed");
					#endif
				}
				
				_BLE_arrayAuthentication.writeValue(aInData, AUTHENTICATION_CHARATERISTIC_SIZE, true);

			}

	}
	
	if(!_bFLAG_CentralConnected)
	{
		_bFLAG_Authenticated = false;
	}
}

void Bluetooth::checkCentralConnected(){
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
}