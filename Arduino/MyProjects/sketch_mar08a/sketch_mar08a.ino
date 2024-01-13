void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
pinMode(A0, OUTPUT);
digitalWrite(A0, HIGH);
delay(2000);
digitalWrite(A0, LOW);
delay(2000);
pinMode(A0, INPUT);
delay(2000);
}
