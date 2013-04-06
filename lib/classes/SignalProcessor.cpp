/*
 * SignalProcessor.cpp
 *
 */

#include "SignalProcessor.h"

SignalProcessor::SignalProcessor() {
}

unsigned long SignalProcessor::pasLastProcessing = 0;
unsigned long SignalProcessor::throttleLastProcessing = 0;
unsigned long SignalProcessor::brakeLastProcessing = 0;
unsigned long SignalProcessor::wheelLastProcessing = 0;
unsigned long SignalProcessor::pasTime = 0;
unsigned long SignalProcessor::pasLastTime = 0;
unsigned int SignalProcessor::pasTimeOn = 0;
unsigned int SignalProcessor::pasTimeOff = 0;
unsigned int SignalProcessor::pasSignalCount = 0;
unsigned int SignalProcessor::pasRPM = 0;
int SignalProcessor::pasDirection;
bool SignalProcessor::isPedaling = false;
unsigned int SignalProcessor::throttleSignal = 0;
bool SignalProcessor::brakePulled = true;
unsigned int SignalProcessor::wheelSignalCount = 0;
unsigned long SignalProcessor::wheelTimeCount = 0;
unsigned long SignalProcessor::wheelLastSignal = 0;
unsigned int SignalProcessor::wheelRPM = 0;
unsigned int SignalProcessor::wheelRPMMemo[12];

void SignalProcessor::startCollect() {
	Serial.println("call: SignalProcessor::collect()");
	pinMode(config::pasSensorPin, INPUT);
	digitalWrite(config::pasSensorPin, HIGH);
	pinMode(config::wheelSensorPin, INPUT);
	digitalWrite(config::wheelSensorPin, HIGH);
	pinMode(config::throttleSensorPin, INPUT);
	pinMode(config::brakeSensorPin, INPUT);
	attachInterrupt(0, SignalProcessor::collectPasSignals, CHANGE);
	attachInterrupt(1, SignalProcessor::collectWheelSignals, RISING);
}

void SignalProcessor::stopCollect() {
	detachInterrupt(0);
	detachInterrupt(1);
}

void SignalProcessor::collectWheelSignals() {
	// for some funny reson, 2 interrupts are called each round...
	if (Global::timeRunning > wheelLastSignal + 5) {
		++ wheelSignalCount;
		wheelTimeCount += Global::timeRunning - wheelLastSignal;
//		Serial.print("wheel signal received=");
//		Serial.println(digitalRead(config::wheelSensorPin));
	}
	wheelLastSignal = Global::timeRunning;
}

void SignalProcessor::collectPasSignals() {
	//Serial.print("pas signal received=");
	//Serial.println(digitalRead(config::pasSensorPin));
	SignalProcessor::pasTime = millis();
	if (digitalRead(config::pasSensorPin) == 1) {
		SignalProcessor::pasTimeOn += SignalProcessor::pasTime - SignalProcessor::pasLastTime;
		SignalProcessor::pasSignalCount++;
	} else {
		SignalProcessor::pasTimeOff += SignalProcessor::pasTime	- SignalProcessor::pasLastTime;
	}
	SignalProcessor::pasLastTime = SignalProcessor::pasTime;
}

void SignalProcessor::processSignals() {
	// process PAS signal
	if (Global::timeRunning >= pasLastProcessing + 2000) {
		pasRPM = (pasSignalCount * 30) / config::pasNrMagnets;
		if (pasSignalCount < 3) {
			pasDirection = 0;
			isPedaling = false;
		} else if (pasTimeOn < pasTimeOff) {
			pasDirection = -1;
			isPedaling = false;
		} else {
			pasDirection = 1;
			isPedaling = true;
		}
		Serial.print("pas: isPedaling=");
		Serial.print(isPedaling);
		Serial.print(", RPM=");
		Serial.print(pasRPM);
		Serial.print(", signal  count=");
		Serial.print(pasSignalCount);
		Serial.print(", on=");
		Serial.print(pasTimeOn);
		Serial.print(", off=");
		Serial.print(pasTimeOff);
		Serial.print(", direction=");
		Serial.print(pasDirection);
		Serial.println("");
		pasLastProcessing = Global::timeRunning;
		pasTimeOn = 0;
		pasTimeOff = 0;
		pasSignalCount = 0;
	}

	// process trhottle signal
	if (Global::timeRunning >= throttleLastProcessing + 200) {
		int tSig = (analogRead(config::throttleSensorPin) - 189) * 1.49;
		tSig < 10 ? tSig = 0 : tSig > 1000 ? tSig = 1023 : tSig;
		throttleSignal = tSig;
		Serial.print("trhrottle: ");
		Serial.print(tSig);
		Serial.println("");
		throttleLastProcessing = Global::timeRunning;
	}

	// process brake signal
	if (Global::timeRunning >= brakeLastProcessing + 500) {
		brakePulled = digitalRead(config::brakeSensorPin) == 0;
//		Serial.print("brake: ");
//		Serial.print(brakePulled);
//		Serial.println("");
		brakeLastProcessing = Global::timeRunning;
	}


	// process wheel signal
	if (Global::timeRunning >= wheelLastProcessing + 1000) {

		float millisecPerRound = wheelTimeCount / wheelSignalCount;
		wheelRPM = 60000 / millisecPerRound;


//		unsigned int tmp = 0;
//		for (int i=0; i<12; i++) {
//			tmp += wheelRPMMemo[i];
//			if (i<11) {
//				wheelRPMMemo[i] = wheelRPMMemo[i+1];
//			}
//		}
//		wheelRPMMemo[11] = wheelSignalCount;
//		wheelRPM = tmp * 5;
		float kmh = (wheelRPM * 2.090 * 60) / 1000;
		Serial.print("wheel RPM: ");
		Serial.print(wheelRPM);
		Serial.print(", speed=");
		Serial.print(kmh);
		Serial.print("km/h");
		Serial.println("");
		wheelSignalCount = 0;
		wheelTimeCount = 0;
		wheelLastProcessing = Global::timeRunning;
	}

}

SignalProcessor::~SignalProcessor() {
	stopCollect();
}
