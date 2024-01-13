 #define ledPin 13           //Onboard LED = digital pin 13
 #define rfReceivePin A0     //RF Receiver data pin = Analog pin 0
 #define rfTransmitPin A1     //RF Transmitter data pin = Analog pin 0
 const unsigned int shortTickCount = 1;
 const unsigned int longTickCount = 4;
 const unsigned int veryLongTickCount = 46;
 unsigned int microsecondsPerTickCount = 280;
 unsigned int commandRepeatCount = 20;
 unsigned int counter = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(rfReceivePin, INPUT);
  pinMode(rfTransmitPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("down");
  writeDown();
  delay(3000);
  Serial.println("up");
  writeUp();
  delay(3000);
}

void writeDown()
{
  for(int i = 0; i < commandRepeatCount; i++)
  {
    writeInit();
    writeByte(0x84);
    writeByte(0x71);
    writeByte(0x64);
  }
}

void writeUp()
{
  for(int i = 0; i < commandRepeatCount; i++)
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
  delayMicroseconds(shortTickCount*microsecondsPerTickCount);
  digitalWrite(rfTransmitPin, LOW);
  delayMicroseconds(veryLongTickCount*microsecondsPerTickCount);
}

void writeByte(byte b)
{
  for(int i = 0; i < 8; i++)
    writeBit((b & (B10000000 >> i)) != 0);
}

void writeBit(bool bit)
{
  if(bit)
  {
    digitalWrite(rfTransmitPin, HIGH);
    delayMicroseconds(longTickCount*microsecondsPerTickCount);
    digitalWrite(rfTransmitPin, LOW);
    delayMicroseconds(shortTickCount*microsecondsPerTickCount);
  }
  else
  {
    digitalWrite(rfTransmitPin, HIGH);
    delayMicroseconds(shortTickCount*microsecondsPerTickCount);
    digitalWrite(rfTransmitPin, LOW);
    delayMicroseconds(longTickCount*microsecondsPerTickCount);
  }
}

