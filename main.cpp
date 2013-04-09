// Do not remove the include below
#include "main.h"

SignalProcessor sigPro;
BikeManager bikeManager;

void setup() {

	Serial.begin(115200);
	delay(200);
	Serial.println("Start ebico");
	delay(100);
	bikeManager.initPins();
	sigPro.startCollect();
}

void loop() {
	Global::millisecRunning = millis();
	Global::microsecRunning = micros();
	bool isReady = sigPro.processSignals();
	if (isReady) {
		bikeManager.adjustThrottle();
	}
}
