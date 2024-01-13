#define dataSize 400
int data[dataSize];

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
long t1, t2, tDiff;
t1 = micros();
for(int i=0; i<dataSize; i++)
{
  data[i] = analogRead(A0);
}
t2 = micros();
tDiff = t2-t1;
Serial.print(tDiff);

for(int i=1; i<dataSize; i++)
{
  Serial.print(' ');
  Serial.print(data[i]);
}
Serial.println();
delay(2000);
}

