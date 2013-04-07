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
unsigned long SignalProcessor::currentLastProcessing = 0;
unsigned long SignalProcessor::debugLastProcessing = 0;
float SignalProcessor::current;

unsigned long SignalProcessor::pasTime = 0;
unsigned long SignalProcessor::pasLastTime = 0;
unsigned int SignalProcessor::pasTimeOn = 0;
unsigned int SignalProcessor::pasTimeOff = 0;
unsigned int SignalProcessor::pasSignalCount = 0;
unsigned long SignalProcessor::pasLastValidSignal = 0;
unsigned int SignalProcessor::pasRPM = 0;
int SignalProcessor::pasDirection;
bool SignalProcessor::isPedaling = false;
unsigned int SignalProcessor::throttleSignal = 0;
bool SignalProcessor::brakePulled = true;
unsigned int SignalProcessor::wheelSignalCount = 0;
unsigned long SignalProcessor::wheelTimeCount = 0;
unsigned long SignalProcessor::wheelLastSignal = 0;
unsigned int SignalProcessor::wheelRPM = 0;

void SignalProcessor::startCollect() {
	Serial.println("call: SignalProcessor::collect()");
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
	}
	wheelLastSignal = Global::timeRunning;
}

void SignalProcessor::collectPasSignals() {
	SignalProcessor::pasTime = millis();
	if (digitalRead(config::pasSensorPin) == 1) {
		SignalProcessor::pasTimeOn += SignalProcessor::pasTime - SignalProcessor::pasLastTime;
		SignalProcessor::pasSignalCount++;
	} else {
		SignalProcessor::pasTimeOff += SignalProcessor::pasTime	- SignalProcessor::pasLastTime;
	}
	SignalProcessor::pasLastTime = SignalProcessor::pasTime;
}

bool SignalProcessor::processSignals() {
	// process PAS signal
	if (Global::timeRunning >= pasLastProcessing + 1000) {
		pasRPM = (pasSignalCount * 60) / config::pasNrMagnets;
		if (pasSignalCount < 3) {
			pasDirection = 0;
		} else if (pasTimeOn < pasTimeOff) {
			pasDirection = -1;
		} else {
			pasDirection = 1;
			pasLastValidSignal = Global::timeRunning;
		}
		isPedaling = pasLastValidSignal > Global::timeRunning - 2000;

	}

	// process trhottle signal
	if (Global::timeRunning >= throttleLastProcessing + 200) {

		int tSig = (analogRead(config::throttleSensorPin) - 100) * 1.33;
		tSig < 200 ? tSig = 0 : tSig > 1010 ? tSig = 1023 : tSig;
		throttleSignal = tSig;
		throttleLastProcessing = Global::timeRunning;
	}

	// process brake signal
	if (Global::timeRunning >= brakeLastProcessing + 500) {
		brakePulled = digitalRead(config::brakeSensorPin) == 0;
		brakeLastProcessing = Global::timeRunning;
	}

	// process wheel signal
	if (Global::timeRunning >= wheelLastProcessing + 1000) {
		float millisecPerRound = wheelTimeCount / wheelSignalCount;
		wheelRPM = 60000 / millisecPerRound;
		wheelSignalCount = 0;
		wheelTimeCount = 0;
		wheelLastProcessing = Global::timeRunning;
	}

	// process curent signal
	if (Global::timeRunning >= currentLastProcessing + 50) {
		float signal = analogRead(config::currentSensorPin);
		current = signal / 20;
//		Serial.print("current Amp=");
//		Serial.print(current);
//		Serial.println("");
		currentLastProcessing = Global::timeRunning;
	}

	// serial output for debugging
	if (Global::timeRunning >= debugLastProcessing + 2000) {
				Serial.print("isPedaling=");
				Serial.print(isPedaling);
				Serial.print(", pedal RPM=");
				Serial.print(pasRPM);
		//		Serial.print(", signal  count=");
		//		Serial.print(pasSignalCount);
		//		Serial.print(", direction=");
		//		Serial.print(pasDirection);
				Serial.print(", brake=");
				Serial.print(brakePulled);
				Serial.print(", trhrottle=");
				Serial.print(throttleSignal);
				Serial.print(", wheel RPM=");
				Serial.print(wheelRPM);
				Serial.print(", speed km/h=");
				Serial.print((wheelRPM * 2.090 * 60) / 1000);
				Serial.println("");
				pasLastProcessing = Global::timeRunning;
				pasTimeOn = 0;
				pasTimeOff = 0;
				pasSignalCount = 0;


		debugLastProcessing = Global::timeRunning;
	}

	return true;
}

SignalProcessor::~SignalProcessor() {
	stopCollect();
}
