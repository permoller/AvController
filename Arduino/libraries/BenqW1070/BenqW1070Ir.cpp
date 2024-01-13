/*
  BenqW1070Ir.h - Library for sending IR commands to BenQ W1070 projector.
  Created by Per F. Møller, February 23, 2018.
  Released into the public domain.
*/
#include "Arduino.h"
#include "SimpleIr.h"
#include "BenqW1070Ir.h"

#define Short 550

BenqW1070Ir::BenqW1070Ir(SimpleIr & ir) : _ir(ir)
{
}

void BenqW1070Ir::power()
{
	writeCommand(0x00, 0x0c, 0x40, 0xbf);
}

void BenqW1070Ir::mute()
{
	writeCommand(0x00, 0x0c, 0x28, 0xd7);
}

void BenqW1070Ir::writeCommand(byte b0, byte b1, byte b2, byte b3)
{
	_ir.writeHighLow(8200, 4500);
	writeByte(b0);
	writeByte(b1);
	writeByte(b2);
	writeByte(b3);
	_ir.writeHighLow(Short, 46700);
	buttonStillDown();
	buttonStillDown();
	buttonStillDown();
}

void BenqW1070Ir::buttonStillDown()
{
	_ir.writeHighLow(8200, 2300);
	_ir.writeHighLow(Short, 30700);
}

void BenqW1070Ir::writeByte(byte b)
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

void BenqW1070Ir::writeBit(bool bit)
{
	_ir.writeHighLow(Short, bit ? 1670 : Short);
}

