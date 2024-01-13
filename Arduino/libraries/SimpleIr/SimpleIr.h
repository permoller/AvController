/*
SimpleIr.h - Library for sending 38kHz IR signals.
Created by Per F. Møller, February 23, 2018.
Released into the public domain.
*/
#ifndef SimpleIr_h
#define SimpleIr_h

#include "Arduino.h"

class SimpleIr
{
	public:
		SimpleIr(int pin);
		void writeHighLow(unsigned int highDurationMicroseconds, unsigned int lowDurationMicroseconds);
	private:
		int _pin;
};

#endif