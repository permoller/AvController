/*
  ProjectorScreenRf.h - Library for sending RF commands to projector screen.
  Created by Per F. Møller, February 23, 2018.
  Released into the public domain.
*/
#include "Arduino.h"
#include "ProjectorScreenRf.h"

ProjectorScreenRf::ProjectorScreenRf(int pin)
{
	_pin = pin;
}

void ProjectorScreenRf::up()
{
	writeCommand(0x84, 0x71, 0x61);
}

void ProjectorScreenRf::down()
{
	writeCommand(0x84, 0x71, 0x64);
}

void ProjectorScreenRf::writeCommand(byte b0, byte b1, byte b2)
{
	for(int i=0; i < 20; i++)
	{
		writeBit(false);
		delayMicroseconds(12040);
		writeByte(b0);
		writeByte(b1);
		writeByte(b2);
	}
}

void ProjectorScreenRf::writeByte(byte b)
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

void ProjectorScreenRf::writeBit(bool bit)
{
	digitalWrite(_pin, HIGH);
	delayMicroseconds(bit ? 1120 : 280);
	digitalWrite(_pin, LOW);
	delayMicroseconds(!bit ? 1120 : 280);
}

