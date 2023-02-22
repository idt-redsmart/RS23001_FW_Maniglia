#include <Arduino.h>
#include <Nicla_System.h>
#include <def.h>
#include <Bluetooth.h>
#include "LoadCell.h"

// #include <Arduino_BHY2.h>
// #include <ArduinoBLE.h>
LoadCell Scale;
Bluetooth BT;

// bool bFLAG_CentralConnected = false; // true = central connected, false = central NOT connected
// bool bFLAG_KeepAlive = false;
// bool bFLAG_Authenticated = false;

void setup()
{
  // put your setup code here, to run once:
  DEBUG.begin(115200);
  while (!DEBUG)
  {
  }
  // delay(500);

  nicla::begin();

  Scale.setup();

  BT.setup();

}

void loop()
{
  // put your main code here, to run repeatedly:
  BT.poll();
  BT.authentication();
  BT.checkCentralConnected();
}
