/*
  DenonAvr1912Ir.h - Library for sending IR commands to Denon AVR 1912 receiver.
  Created by Per F. Møller, February 23, 2018.
  Released into the public domain.
*/
#include "Arduino.h"
#include "SimpleIr.h"
#include "DenonAvr1912Ir.h"

DenonAvr1912Ir::DenonAvr1912Ir(SimpleIr & ir) : _ir(ir)
{
}

void DenonAvr1912Ir::powerOn()
{
	writeCommand(0x2a, 0x4c, 0x02, 0x84, 0x0, 0x86);
}

void DenonAvr1912Ir::powerOff()
{
	writeCommand(0x2a, 0x4c, 0x02, 0x8c, 0x0, 0x8e);
}

void DenonAvr1912Ir::sourceSelectBd()
{
	writeCommand(0x2a, 0x4c, 0x02, 0x88, 0xb4, 0x3e);
}

void DenonAvr1912Ir::sourceSelectTv()
{
	writeCommand(0x2a, 0x4c, 0x02, 0x84, 0xb4, 0x32);
}

void DenonAvr1912Ir::sourceSelectGame1()
{
	writeCommand(0x2a, 0x4c, 0x02, 0x8a, 0xb4, 0x3c);
}

void DenonAvr1912Ir::sourceSelectGame2()
{
	writeCommand(0x2a, 0x4c, 0x02, 0x8e, 0xb4, 0x38);
}

void DenonAvr1912Ir::writeCommand(byte b0, byte b1, byte b2, byte b3, byte b4, byte b5)
{
	_ir.writeHighLow(3350, 1712);
	writeByte(b0);
	writeByte(b1);
	writeByte(b2);
	writeByte(b3);
	writeByte(b4);
	writeByte(b5);
	writeBit(false);
	delayMicroseconds(8300);
}

void DenonAvr1912Ir::writeByte(byte b)
{
	writeBit((b & B10000000) != 0);
	writeBit((b & B01000000) != 0);
	writeBit((b & B00100000) != 0);
	writeBit((b & B00010000) != 0);
	writeBit((b & B00001000) != 0);
	writeBit((b & B00000100) != 0);
	writeBit((b & B00000010) != 0);
	writeBit((b & B00000001) != 0);
}

void DenonAvr1912Ir::writeBit(bool bit)
{
	_ir.writeHighLow(420, bit ? 1268 : 420);
}

