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
	static unsigned int pasRPM;
	static int pasDirection;
	static bool isPedaling;
	static unsigned int throttleSignal;
	static bool brakePulled;
	static unsigned int wheelRPM;
	static float current;

private:
	static void collectPasSignals();
	static void collectWheelSignals();
	unsigned static long pasLastProcessing;
	unsigned static long throttleLastProcessing;
	unsigned static long brakeLastProcessing;
	unsigned static long currentLastProcessing;
	unsigned static long wheelLastProcessing;
	unsigned static long debugLastProcessing;
	unsigned static long pasTime;
	unsigned static long pasLastTime;
	unsigned static int pasTimeOn;
	unsigned static int pasTimeOff;
	unsigned static int pasSignalCount;
	unsigned static long pasLastValidSignal;
	unsigned static long wheelLastSignal;
	unsigned static int wheelSignalCount;
	unsigned static long wheelTimeCount;
	static unsigned int currentSignal;
};
#endif /* SIGNALPROCESSOR_H_ */
