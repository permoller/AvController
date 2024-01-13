void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = read();
  if(isButton1(value))
  {
    Serial.println("Button 1 down");
    while(isButton1(value = read()));
    Serial.println("Button 1 up");
  }
else
  if(isButton2(value))
  {
    Serial.println("Button 2 down");
    while(isButton2(value = read()));
    Serial.println("Button 2 up");
  }
else
  if(isButton3(value))
  {
    Serial.println("Button 3 down");
    while(isButton3(value = read()));
    Serial.println("Button 3 up");
  }
else
  if(isButton4(value))
  {
    Serial.println("Button 4 down");
    while(isButton4(value = read()));
    Serial.println("Button 4 up");
  }
  
  if(value < 900)
  {
    Serial.println(value);
  }
  delay(300);
}
bool isButton1(int value)
{
  return isBetween(value, 10, 20);
}

bool isButton2(int value)
{
  return isBetween(value, 65, 75);
}

bool isButton3(int value)
{
  return isBetween(value, 130, 150);
}

bool isButton4(int value)
{
  return isBetween(value, 220, 240);
}

bool isBetween(int value, int min, int max)
{
  return value >= min && value <= max;
}

int read()
{
  int v1, v2, dif;
  do
  {
    v1 = analogRead(A0);
    delay(50);
    v2 = analogRead(A0);
    dif = v1-v2;
  } while (dif < -10 || dif > 10);
  return v2;
}


