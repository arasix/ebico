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
unsigned long SignalProcessor::amperageLastProcessing = 0;
unsigned long SignalProcessor::debugLastProcessing = 0;
float SignalProcessor::amperage;
unsigned long SignalProcessor::pasLastTime = 0;
unsigned long SignalProcessor::pasTimeOn = 0;
unsigned long SignalProcessor::pasTimeOff = 0;
unsigned long SignalProcessor::pasLastOnSignal = 0;
unsigned long SignalProcessor::pasLastForwardSignal = 0;
float SignalProcessor::pasRPM = 0;
int8_t SignalProcessor::pasDirection;
bool SignalProcessor::isPedaling = false;
unsigned int SignalProcessor::throttleSignal = 0;
bool SignalProcessor::brakePulled = true;
unsigned long SignalProcessor::wheelLastSignal;
float SignalProcessor::wheelRPM = 0;
int8_t SignalProcessor::pasCnt = 0;
unsigned long SignalProcessor::pasRotationTime = 0;

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
	// for some funny reason 2 interrupts are called on each rotation...
	if (Global::microsecRunning > wheelLastSignal + 100000) {
		wheelRPM = 60e6 / ((float)(Global::microsecRunning - wheelLastSignal));
	}
	wheelLastSignal = Global::microsecRunning;
}
//
//void SignalProcessor::collectPasSignals() {
//	SignalProcessor::pasTime = millis();
//	if (digitalRead(Global::pasSensorPin) == 1) {
//		SignalProcessor::pasTimeOn += SignalProcessor::pasTime - SignalProcessor::pasLastTime;
//		SignalProcessor::pasSignalCount++;
//	} else {
//		SignalProcessor::pasTimeOff += SignalProcessor::pasTime	- SignalProcessor::pasLastTime;
//	}
//	SignalProcessor::pasLastTime = SignalProcessor::pasTime;
//}

void SignalProcessor::collectPasSignals() {
	if (digitalRead(Global::pasSensorPin) == 1) {
		pasTimeOn += Global::microsecRunning - pasLastTime;
		++pasCnt;
		if (pasCnt == Global::pasNrMagnets / 2 ) {
			pasRPM = (60e6 / (float)(Global::microsecRunning - pasRotationTime)) / 2;
			if (pasTimeOn > pasTimeOff) {
				pasLastForwardSignal = Global::microsecRunning;
				pasDirection = 1;
			} else {
				pasDirection = -1;
			}
//			Serial.print("halbe Runde voll, ");
//			Serial.print(pasTimeOn);
//			Serial.print(":");
//			Serial.print(pasTimeOff);
//			Serial.print(", rpm=");
//			Serial.print(pasRPM);
//			Serial.print(", dir=");
//			Serial.print(pasDirection);
//			Serial.println("");
			pasTimeOn = 0;
			pasTimeOff = 0;
			pasCnt = 0;
			pasRotationTime = Global::microsecRunning;
		}
		pasLastOnSignal = Global::microsecRunning;
	} else {
		//Serial.println(0);
		pasTimeOff += Global::microsecRunning - pasLastTime;
	}
	pasLastTime = Global::microsecRunning;
}

bool SignalProcessor::processSignals() {
	// process PAS and wheel signal
	if (Global::millisecRunning >= pasLastProcessing + 50) {
		// pas signal
		if (pasLastOnSignal < Global::microsecRunning - 1e6) {
			pasRPM = 0;
		}
		if (pasRPM < 5) {
			pasDirection = 0;
		}
		isPedaling = pasLastForwardSignal > Global::microsecRunning - 2e6;
		// wheel signal
		if (wheelLastSignal < Global::microsecRunning - 2e6) {
			wheelRPM = 0;
		}
	}

	// process trhottle signal
	if (Global::millisecRunning >= throttleLastProcessing + 51) {
		int tSig = (analogRead(Global::throttleSensorPin) - 100) * 1.33;
		tSig < 200 ? tSig = 0 : tSig > 1010 ? tSig = 1023 : tSig;
		throttleSignal = tSig;
		throttleLastProcessing = Global::millisecRunning;
	}

	// process brake signal
	if (Global::millisecRunning >= brakeLastProcessing + 111) {
		brakePulled = digitalRead(Global::brakeSensorPin) == 0;
		brakeLastProcessing = Global::millisecRunning;
	}

	// process amperage signal
	if (Global::millisecRunning >= amperageLastProcessing + 21) {
		float signal = analogRead(Global::amperageSensorPin);
		amperage = signal / 20;
//		Serial.print("amperage Amp=");
//		Serial.print(amperage);
//		Serial.println("");
		amperageLastProcessing = Global::millisecRunning;
	}

	// serial output for debugging
	if (Global::millisecRunning >= debugLastProcessing + 20000) {
				Serial.print("isPedaling=");
				Serial.print(isPedaling);
				Serial.print(", pedal RPM=");
				Serial.print(pasRPM);
		//		Serial.print(", signal  count=");
		//		Serial.print(pasSignalCount);
				Serial.print(", direction=");
				Serial.print(pasDirection);
				Serial.print(", brake=");
				Serial.print(brakePulled);
				Serial.print(", throttle=");
				Serial.print(throttleSignal);
				Serial.print(", wheel RPM=");
				Serial.print(wheelRPM);
				Serial.print(", speed km/h=");
				Serial.print((wheelRPM * 2.090 * 60) / 1000);
				Serial.println("");
		debugLastProcessing = Global::millisecRunning;
	}

	return true;
}

SignalProcessor::~SignalProcessor() {
	stopCollect();
}
