// NEC IR Protokol: http://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol

 #define transmitPin A1     //RF Transmitter data pin = Analog pin 0
 const unsigned int shortDurationMicroseconds = 562;
 const unsigned int longDurationMicroseconds = 1687;
 const unsigned int initMarkDurationMicroseconds = 9000;
 const unsigned int initSpaceDurationMicroseconds = 4500;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(transmitPin, OUTPUT);
  Serial.println("ready");
//  delay(1000);
//  Serial.println("power");
//  unsigned long a = micros();
//  unsigned long b = micros();
  sendPowerCommand();
  /*unsigned long c = micros();
  Serial.println(b-a);
  Serial.println(c-b);
  delay(5000);
  Serial.println("mute");
  sendMuteCommand();*/
}

void loop() {
  
}

void sendPowerCommand()
{
  writeInit();
  writeByte(0x00);
  writeByte(0x0C);
  writeByte(0x40);
  writeByte(0xBF);
  writeEnd();
}

void sendMuteCommand()
{
  writeInit();
  writeByte(0x00);
  writeByte(0x0C);
  writeByte(0x28);
  writeByte(0xD7);
  writeEnd();
}

void writeInit()
{
  mark(initMarkDurationMicroseconds);
  space(initSpaceDurationMicroseconds);
}

void writeEnd()
{
  mark(shortDurationMicroseconds);
}

void writeByte(byte b)
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

void writeBit(bool bit)
{
  mark(shortDurationMicroseconds);
  if(bit)
  {
    space(longDurationMicroseconds);
  }
  else
  {
    space(shortDurationMicroseconds);
  }
}

void mark(int durationMicroseconds)
{
  // 38kHz = 26.3 us
  // 13.15 high + 13.15 low
  // digitalWrite ~ 4.7 us

  int cyclesRemaining = (durationMicroseconds+13)/26;
  
  while(cyclesRemaining > 0)
  {
    cyclesRemaining--;
    digitalWrite(transmitPin, HIGH);
    delayMicroseconds(11);
    digitalWrite(transmitPin, LOW);
    delayMicroseconds(10);
  }
}

void space(int durationMicroseconds)
{
  delayMicroseconds(durationMicroseconds);
}

