/* IRLib_P13_DenonAvr1912.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */
/*
 * The protocol seams to be based on Kaseikyo
 * http://www.hifi-remote.com/johnsfine/DecodeIR.html#Kaseikyo
 * IRP notation: {37k,432}<1,-1|1,-3>(8,-4,M:8,N:8,X:4,D:4,S:8,F:8,G:8,1,-173)+ {X=M:4:0^M:4:4^N:4:0^N:4:4}
 * M = 42 = 0x2a = 0010 1010
 * N = 76 = 0x4c = 0100 1100
 * X = M:4:0^M:4:4^N:4:0^N:4:4 = 1010^0010^1100^0100 = 0000
 * D = 2 = 0x2 = 0010
 * S, F and G is the actual data that changes between commands
 * 
 * The first 24 bits are always 0x2a4c02 and are not exposed
 * The last 24 bits are the actual data returned when decoding and accepted when sending
 */

#ifndef IRLIB_PROTOCOL_13_H
#define IRLIB_PROTOCOL_13_H
#define IR_SEND_PROTOCOL_13		case 13: IRsendDenonAvr1912::send(data); break;
#define IR_DECODE_PROTOCOL_13	if(IRdecodeDenonAvr1912::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_13 ,public virtual IRdecodeDenonAvr1912
	#define PV_IR_SEND_PROTOCOL_13   ,public virtual IRsendDenonAvr1912
#else
	#define PV_IR_DECODE_PROTOCOL_13  public virtual IRdecodeDenonAvr1912
	#define PV_IR_SEND_PROTOCOL_13    public virtual IRsendDenonAvr1912
#endif

#ifdef IRLIBSENDBASE_H

class IRsendDenonAvr1912: public virtual IRsendBase {
  public:
    void send(uint32_t data) {
    //void IRsendDenonAvr1912::send(uint32_t data, uint32_t data2)//optional form
		
		// sendGeneric() has been patched so it does not always include an additional space when no stopbit is used.
		// This allows us to call it twice to send more than 32 bits total

		sendGeneric(0x2a4c02, 24, 432 * 8, 432 * 4, 432, 432, 432 * 3, 432, 37, false, 0);
		sendGeneric(data, 24, 0, 0, 432, 432, 432 * 3, 432, 37, false, 0);
		
		// stop bit
		mark(432);
		// the final space requires more than 16 bits so we make one call to space() and another to delay()
		space(736 /*432 * 173 % 1000*/);
		delay(74 /*432 * 173 / 1000*/);
    };
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
class IRdecodeDenonAvr1912: public virtual IRdecodeBase {
  public:
    bool decode(void) {
      IRLIB_ATTEMPT_MESSAGE(F("DenonAvr1912"));
      
	  // space since the last frame + header mark and space + 48 bits made from a mark and a space + stop bit
	  uint8_t expectedLength = 
		  1 //space since last command
		  + 1 // header mark
		  + 1 // header space
		  + (48 * 2) // 48 bits each having a mark and a space
		  + 1; // mark from stop bit. The final space is included in the next command in the space since last command
	  if (!decodeGeneric(expectedLength, 432 * 8, 432 * 4, 432, 432 * 3, 432))
		  return false;

	  protocolNum = DenonAvr1912;
	  bits = 24;
	  value = value & 0x00ffffff;
	  address = 0;
      return true;
    }
};

#endif //IRLIBDECODEBASE_H

#define IRLIB_HAVE_COMBO

#endif //IRLIB_PROTOCOL_13_H
