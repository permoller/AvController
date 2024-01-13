#define recievePin A0

byte address;
byte command;
bool toggleBit;

#define BITPERIOD 895
#define MARGIN 100

void setup() {
  // put your setup code here, to run once:
  pinMode(recievePin, INPUT);
  Serial.begin(9600);
  Serial.println(HIGH);
  Serial.println(LOW);
  Serial.println("ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  int error = tryRead();
  if(error == 0)
  {
    Serial.println("Got data");
    Serial.println(toggleBit);
    Serial.println(address);
    Serial.println(command);
  }
  else if (error != -100)
  {
    Serial.println("Error");
    Serial.println(error);
  }
    
}

int tryRead()
{
  int b;
  // start bit 1 (4 times normal length)
  if(!readExpectedMinMax(HIGH, (BITPERIOD-MARGIN)*3, (BITPERIOD+MARGIN)*3))
    return -100;
    
  if(!readExpectedMinMax(LOW, BITPERIOD-MARGIN, BITPERIOD+MARGIN))
    return -200;

  // start bit 2
  if(b = readBit() != HIGH)
    return -300 - b;

  // field bits
  for(int i = 0; i < 3; i++)
  {
    if(b = readBit() != LOW)
      return -400 - (10+i) - b;
  }

  // togle bit (2 times normal length)
  int t = readBit(BITPERIOD*2, MARGIN*2);
  if(t != LOW && t != HIGH)
    return -500 - t;

  // address bits
  int a = readByte();
  if(a < 0 || a > 255)
    return a;
    
  // command bits
  int c = readByte();
  if(c < 0 || c > 255)
    return c;
  
  toggleBit = t;
  address = a;
  command = c;
  return 0;
}

int readByte()
{
  byte result = 0;
  int b;
  for(int i = 0; i < 8; i++)
  {
   result =  result << 1;
    b = readBit();
    if(b == HIGH)
      result =  result | 1;
    else if(b == LOW)
      result = result | 0;
    else
      return b*10;
  }
  return result;
}
int readBit()
{
  return readBit(BITPERIOD, MARGIN);
}

int readBit(int bitPeriod, int margin)
{
  delayMicroseconds(bitPeriod/4);
  int b = read();
  if(!readExpectedMinMax(b, (bitPeriod/4)-margin, (bitPeriod/4)+margin))
    return -2;
    
  if(!readExpectedMin(invert(b), (bitPeriod/2)-margin, bitPeriod/2))
    return -3;
  
  return b;
}

bool readExpectedMin(int expected, int minDuration, int expectedDuration)
{
  unsigned long timestamp = micros();
  unsigned long minTimeout = timestamp + minDuration;
  unsigned long expectedTimeout = timestamp + expectedDuration;
  while(read() == expected)
  {
    timestamp = micros();
    if(timestamp > expectedTimeout)
      break;
  }
  return timestamp > minTimeout;
}

bool readExpectedMinMax(int expected, int minDuration, int maxDuration)
{
  unsigned long timestamp = micros();
  unsigned long minTimeout = timestamp + minDuration;
  unsigned long maxTimeout = timestamp + maxDuration;
  while(read() == expected)
  {
    timestamp = micros();
    if(timestamp > maxTimeout)
      return false;
  }
  return timestamp > minTimeout;
}

int read()
{
  return digitalRead(recievePin) == LOW ? HIGH : LOW;
}

int invert(int value)
{
  return 
    value == HIGH ? LOW :
    value == LOW ? HIGH :
    value * -1;
}

