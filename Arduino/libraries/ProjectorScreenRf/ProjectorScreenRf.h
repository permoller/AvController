/*
  ProjectorScreenRf.h - Library for sending RF commands to projector screen.
  Created by Per F. Møller, February 23, 2018.
  Released into the public domain.
*/
#ifndef ProjectorScreenRf_h
#define ProjectorScreenRf_h

#include "Arduino.h"

class ProjectorScreenRf
{
	public:
		ProjectorScreenRf(int pin);
		void up();
		void down();
	private:
		int _pin;
		void writeCommand(byte, byte, byte);
		void writeByte(byte);
		void writeBit(bool);
};

#endif