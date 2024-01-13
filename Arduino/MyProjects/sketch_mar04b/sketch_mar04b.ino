void setup() {
  // put your setup code here, to run once:
pinMode(A0, INPUT);
Serial.begin(9600);
Serial.println("ready");
}
byte data[300];

void loop() {

  unsigned int t1 = micros();
  for(int i = 0; i < 300; i++)
  {
    byte b = 0;
    for(int j=7;j>=0;j--)
      b = b | ((digitalRead(A0) == HIGH ? 1 : 0) << j);
    data[i] = b;
  }
  unsigned int t2 = micros();
  Serial.print(t2-t1);
  int x=0;
  bool previous;
  bool current;
  bool first = true;
  for(int i = 0; i < 300; i++)
  {
    byte b = data[i];
    
    for(int j=7;j>=0;j--)
    {
      x++;
      current = (b & (1 << j))!=0;
      
      if(!first && current != previous)
      {
        Serial.print('\t');
        if(previous)
          Serial.print(x);
        else
          Serial.print(-x);
        x = 0;
      }
      first = false;
      previous = current;
    }
    
  }
  Serial.print('\t');
  if(previous)
    Serial.print(x);
  else
    Serial.print(-x);
  Serial.println();
  
}
