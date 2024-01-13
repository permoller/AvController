 const int dataSize = 500;  //Arduino memory is limited (max=1700)
 unsigned int storedData[dataSize];  //Create an array to store the data
 int dataIndex = 0;
 #define receivePin A0     //RF Receiver data pin = Analog pin 0
  unsigned long time1=0;  //Variable to record the start time 
 unsigned long time2=0;    //Variable to record the end time 
 unsigned long signalDuration=0; //Variable to record signal reading time
unsigned long timeoutCounter = 0;
const unsigned long timeoutCount = 100000;
 
 void setup(){
  Serial.begin(9600);
  pinMode(receivePin, INPUT);
  
  Serial.println("Waiting for signal");
 }

void loop(){

  timeoutCounter = 0;
  dataIndex = 0;
  
  do { time1 = micros(); }
  while(digitalRead(receivePin) == HIGH);

  do {
    while(timeoutCounter < timeoutCount && digitalRead(receivePin) == LOW) { timeoutCounter++; }
    time2 = micros();
    if(timeoutCounter < timeoutCount)
    {
      storedData[dataIndex++] = time2 - time1;
      timeoutCounter = 0;
      while(timeoutCounter < timeoutCount && digitalRead(receivePin) == HIGH) { timeoutCounter++; }
      time1 = micros();
      if(timeoutCounter < timeoutCount)
        storedData[dataIndex++] = time1 - time2;
    }
  } while(timeoutCounter < timeoutCount && dataIndex < dataSize);

  for(unsigned int i = 0; i < dataIndex; i++) {
    Serial.print(storedData[i]);
    Serial.print(";");
  }
  if(timeoutCounter < timeoutCount)
    Serial.println("datastore is full");
  else
    Serial.println();
 }
