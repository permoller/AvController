/*
  DenonAvr1912Ir.h - Library for sending IR commands to Denon AVR 1912 receiver.
  Created by Per F. Møller, February 23, 2018.
  Released into the public domain.
*/
#ifndef DenonAvr1912Ir_h
#define DenonAvr1912Ir_h

#include "Arduino.h"
#include "SimpleIr.h"

class DenonAvr1912Ir
{
	public:
		DenonAvr1912Ir(SimpleIr & ir);
		void powerOn();
		void powerOff();
		void sourceSelectBd();
		void sourceSelectTv();
		void sourceSelectGame1();
		void sourceSelectGame2();
	private:
		SimpleIr & _ir;
		void writeCommand(byte, byte, byte, byte, byte, byte);
		void writeByte(byte);
		void writeBit(bool);
};

#endif