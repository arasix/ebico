/*
 * ParameterProcessor.h
 *
 */

#include "Arduino.h"

#ifndef PARAMETERPROCESSOR_H_
#define PARAMETERPROCESSOR_H_

class ParameterProcessor {
public:
	ParameterProcessor();
	virtual ~ParameterProcessor();
	void startCollect();
	void stopCollect();
private:
	static void collectPasSignals();
	static void collectWheelSignals();
};
#endif /* PARAMETERPROCESSOR_H_ */
