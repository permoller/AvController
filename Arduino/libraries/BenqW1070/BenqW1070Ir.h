/*
  BenqW1070Ir.h - Library for sending IR commands to BenQ W1070 projector.
  Created by Per F. Møller, February 23, 2018.
  Released into the public domain.
*/
#ifndef BenqW1070Ir_h
#define BenqW1070Ir_h

#include "Arduino.h"
#include "SimpleIr.h"

class BenqW1070Ir
{
	public:
		BenqW1070Ir(SimpleIr & ir);
		void power();
		void mute();
	private:
		SimpleIr & _ir;
		void writeCommand(byte, byte, byte, byte);
		void buttonStillDown();
		void writeByte(byte);
		void writeBit(bool);
};

#endif