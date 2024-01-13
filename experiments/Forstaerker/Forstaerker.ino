
 #define transmitPin A1
 const unsigned int shortDurationMicroseconds = 420;
 const unsigned int longDurationMicroseconds = 1268;
 const unsigned int initMarkDurationMicroseconds = 3350;
 const unsigned int initSpaceDurationMicroseconds = 1712;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(transmitPin, OUTPUT);
  Serial.println("ready");
  powerOn();
}

void loop() {
  
}

void powerOn()
{
  sendCommand(0x2a, 0x4c, 0x02, 0x84, 0x0, 0x86);
}

void powerOff()
{
  sendCommand(0x2a, 0x4c, 0x02, 0x8c, 0x0, 0x8e);
}


void selectBdInput()
{
  sendCommand(0x2a, 0x4c, 0x02, 0xa8, 0xb4, 0x1e);
}

void selectTvInput()
{
  sendCommand(0x2a, 0x4c, 0x02, 0xa4, 0xb4, 0x12);
}

void selectGame1Input()
{
  sendCommand(0x2a, 0x4c, 0x02, 0xaa, 0xb4, 0x1c);
}

void selectGame2Input()
{
  sendCommand(0x2a, 0x4c, 0x02, 0xae, 0xb4, 0x18);
}

void sendCommand(byte b1, byte b2, byte b3, byte b4, byte b5, byte b6)
{
  writeInit();
  writeByte(b1);
  writeByte(b2);
  writeByte(b3);
  writeByte(b4);
  writeByte(b5);
  writeByte(b6);
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

