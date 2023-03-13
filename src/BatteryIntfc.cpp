
#include <BatteryIntfc.h>

void BatteryIntfc::setup()
{
	nicla::enableCharge(100);

	dbg(DBG_BATTERY, "Battery setup OK");
}

String BatteryIntfc::getLevel()
{
	if (nicla::getBatteryStatus() == 5)
	{
		_data = "B;" + String(nicla::getBatteryStatus() * 20);

		dbg(DBG_BATTERY, "\nBattery level: " + String(nicla::getBatteryStatus() * 20));
		dbg(DBG_BATTERY, "Sending battery level: " + _data);

		return _data;
	}
	else
	{
		_data = "B;0" + String(nicla::getBatteryStatus() * 20);

		dbg(DBG_BATTERY, "\nBattery level: " + String(nicla::getBatteryStatus() * 20));
		dbg(DBG_BATTERY, "Sending battery level: " + _data);

		return _data;
	}
}

void BatteryIntfc::_readBit(uint8_t tmp)
{
	#ifdef BATTERY_DEBUG_
	for (int x = 7; x >= 0; x--)
		Serial.print(bitRead(tmp, x));
	Serial.print("\n");
	#endif
}

void BatteryIntfc::readReg()
{
	dbg(DBG_BATTERY, "___________________________");
	dbg(DBG_BATTERY, "\nStatus reg: 0x00");
	_readBit(nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_STATUS));

	dbg(DBG_BATTERY, "\nFault reg: 0x01");
	_readBit(nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_FAULTS));

	dbg(DBG_BATTERY, "\nFast_chg reg: 0x01");
	_readBit(nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_FAST_CHG));

	dbg(DBG_BATTERY, "\nBatt_mon reg: 0x0A");
	nicla::_pmic.writeByte(BQ25120A_ADDRESS, BQ25120A_BATT_MON, 1);
	delay(3);
	_readBit(nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_BATT_MON));
}

void BatteryIntfc::disableCharge()
{
	uint8_t chg_reg = nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_FAST_CHG);

	dbg(DBG_BATTERY, "\nOLD Fast_chg: ");
	_readBit(chg_reg);

	chg_reg |= 0b00000010;
	nicla::_pmic.writeByte(BQ25120A_ADDRESS, BQ25120A_FAST_CHG, chg_reg);

	dbg(DBG_BATTERY, "\nNEW Fast_chg: ");
	_readBit(nicla::_pmic.readByte(BQ25120A_ADDRESS, BQ25120A_FAST_CHG));
}
