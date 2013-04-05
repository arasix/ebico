/*
 * SignalProcessor.h
 *
 */

#include "Arduino.h"

#ifndef SIGNALPROCESSOR_H_
#define SIGNALPROCESSOR_H_

class SignalProcessor {
public:
	SignalProcessor();
	virtual ~SignalProcessor();
	void startCollect();
	void stopCollect();
	static byte pasRPM;
private:
	static void collectPasSignals();
	static void collectWheelSignals();
	unsigned static long pasTime;
	unsigned static long pasLastTime;
	unsigned static int pasTimeOn;
	unsigned static int pasTimeOff;
	unsigned static int pasSignalCount;

};
#endif /* SIGNALPROCESSOR_H_ */
