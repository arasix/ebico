// Do not remove the include below
#include "main.h"

ParameterProcessor parameterProcessor;

void setup()
{
	Serial.begin(115200);
	delay(200);
	Serial.println("Hello world, Hallo Welt!");
	Serial.print("pasSensorPin=");
	Serial.println(config::pasSensorPin);
	delay(100);
	parameterProcessor.startCollect();
}

void loop()
{
//Add your repeated code here
}
