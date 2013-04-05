// Do not remove the include below
#include "main.h"

namespace globals {
	unsigned long millisRunning = 0;
}
SignalProcessor sigPro;

void setup()
{
	globals::millisRunning = millis();
	Serial.begin(115200);
	delay(200);
	Serial.println("Start ebico");
	delay(100);
	sigPro.startCollect();
}

void loop()
{
//Add your repeated code here
}
