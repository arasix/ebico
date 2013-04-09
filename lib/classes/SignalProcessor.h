/*
 * SignalProcessor.h
 *
 */

#include "Arduino.h"
#include "../../main.h"

#ifndef SIGNALPROCESSOR_H_
#define SIGNALPROCESSOR_H_

class SignalProcessor {
public:
	SignalProcessor();
	virtual ~SignalProcessor();
	void startCollect();
	void stopCollect();
	bool processSignals();
	static float pasRPM;
	static uint8_t pasDirection;
	static bool isPedaling;
	static uint8_t throttleSignal;
	static bool brakePulled;
	static float wheelRPM;
	static float amperage;
private:
	static void collectPasSignals();
	static void collectWheelSignals();
	unsigned static long pasLastProcessing;
	unsigned static long throttleLastProcessing;
	unsigned static long brakeLastProcessing;
	unsigned static long amperageLastProcessing;
	unsigned static long debugLastProcessing;
	unsigned static long pasLastTime;
	unsigned static int pasTimeOn;
	unsigned static int pasTimeOff;
	unsigned static long pasLastOnSignal;
	unsigned static long pasLastForwardSignal;
	unsigned static long wheelLastSignal;
};

#endif /* SIGNALPROCESSOR_H_ */
