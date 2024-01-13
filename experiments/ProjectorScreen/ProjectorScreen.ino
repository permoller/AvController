 #define rfTransmitPin A1     //RF Transmitter data pin = Analog pin 0
 const unsigned int shortDelayMicroseconds = 280;
 const unsigned int longDelayMicroseconds = 1120;
 const unsigned int initDelayMicroseconds = 13160;
 const byte commandRepeatCount = 20;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rfTransmitPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("down");
  sendDownCommand();
  delay(3000);
  Serial.println("up");
  sendUpCommand();
  delay(3000);
}

void sendDownCommand()
{
  for(byte i = 0; i < commandRepeatCount; i++)
  {
    writeInit();
    writeByte(0x84);
    writeByte(0x71);
    writeByte(0x64);
  }
}

void sendUpCommand()
{
  for(byte i = 0; i < commandRepeatCount; i++)
  {
    writeInit();
    writeByte(0x84);
    writeByte(0x71);
    writeByte(0x61);
  }
}

void writeInit()
{
  digitalWrite(rfTransmitPin, HIGH);
  delayMicroseconds(shortDelayMicroseconds);
  digitalWrite(rfTransmitPin, LOW);
  delayMicroseconds(initDelayMicroseconds);
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
  if(bit)
  {
    digitalWrite(rfTransmitPin, HIGH);
    delayMicroseconds(longDelayMicroseconds);
    digitalWrite(rfTransmitPin, LOW);
    delayMicroseconds(shortDelayMicroseconds);
  }
  else
  {
    digitalWrite(rfTransmitPin, HIGH);
    delayMicroseconds(shortDelayMicroseconds);
    digitalWrite(rfTransmitPin, LOW);
    delayMicroseconds(longDelayMicroseconds);
  }
}

