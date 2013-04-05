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
	void processSignals();
	static byte pasRPM;
	static bool pasDirection;
	static bool isPedaling;
private:
	static void collectPasSignals();
	static void collectWheelSignals();
	unsigned static long pasTime;
	unsigned static long pasLastTime;
	unsigned static int pasTimeOn;
	unsigned static int pasTimeOff;
	unsigned static int pasSignalCount;
	unsigned static long pasLastProcessing;
};
#endif /* SIGNALPROCESSOR_H_ */
