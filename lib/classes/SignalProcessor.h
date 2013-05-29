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
	static float voltageCompensation;
	static int8_t pasDirection;
	static bool isPedaling;
	static unsigned int throttleSignal;
	static bool brake1Pulled;
	static bool brake2Pulled;
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
	unsigned static long pasTimeOn;
	unsigned static long pasTimeOff;
	unsigned static long pasRotationTime;
	unsigned static long pasLastOnSignal;
	unsigned static long pasLastForwardSignal;
	unsigned static long wheelLastSignal;
	static int8_t pasCnt;
};

#endif /* SIGNALPROCESSOR_H_ */
