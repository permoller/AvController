/*
SimpleIr.h - Library for sending 38kHz IR signals.
Created by Per F. Møller, February 23, 2018.
Released into the public domain.
*/
#include "Arduino.h"
#include "SimpleIr.h"

SimpleIr::SimpleIr(int pin)
{
	pinMode(pin, OUTPUT);
	_pin = pin;
	digitalWrite(_pin, LOW);
}

void SimpleIr::writeHighLow(unsigned int highDurationMicroseconds, unsigned int lowDurationMicroseconds)
{
	// 38kHz = 26.3 us
	// 13.15 high + 13.15 low
	// digitalWrite ~ 4.7 us
	int cyclesRemaining = (highDurationMicroseconds + 13) / 26;
	while (cyclesRemaining > 0)
	{
		cyclesRemaining--;
		digitalWrite(_pin, HIGH);
		delayMicroseconds(11);
		digitalWrite(_pin, LOW);
		delayMicroseconds(10);
	}
	if(lowDurationMicroseconds > 10000)
	{
		delay(lowDurationMicroseconds/1000);
	}
	else
	{
		delayMicroseconds(lowDurationMicroseconds);
	}
}
