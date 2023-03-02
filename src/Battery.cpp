
#include <Battery.h>

void Battery::setup()
{
	nicla::enableCharge(100);

#ifdef BATTERY_DEBUG_
	Serial.println("Battery setup OK");
#endif
}

String Battery::getLevel()
{
	if (nicla::getBatteryStatus() == 5)
	{
		_batteryData = "B;" + String(nicla::getBatteryStatus() * 20);

#ifdef BATTERY_DEBUG_
		Serial.println("\nBattery level: " + String(nicla::getBatteryStatus() * 20));
		Serial.println("Sending battery level: " + _batteryData);
#endif

		return _batteryData;
	}
	else
	{
		_batteryData = "B;0" + String(nicla::getBatteryStatus() * 20);

#ifdef BATTERY_DEBUG_
		Serial.println("\nBattery level: " + String(nicla::getBatteryStatus() * 20));
		Serial.println("Sending battery level: " + _batteryData);
#endif

		return _batteryData;
	}
}

void Led::setColor(uint16_t n, uint8_t color)
{

	uint8_t r, g, b;

	switch (color)
	{
	case LED_COLOR_RED:
		r = 200;
		g = 0;
		b = 0;
		break;

	case LED_COLOR_GREEN:
		r = 0;
		g = 200;
		b = 0;
		break;

	case LED_COLOR_BLUE:
		r = 0;
		g = 0;
		b = 200;
		break;
		
	default:
		r = 0;
		g = 0;
		b = 0;
		break;
	}

	_led.SetPixelColor(n, r, g, b);
	_led.Show();
}