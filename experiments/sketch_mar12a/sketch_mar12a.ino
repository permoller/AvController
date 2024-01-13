void setup() {
  // put your setup code here, to run once:
pinMode(2, OUTPUT);
pinMode(3, INPUT_PULLUP);
pinMode(4, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(2, HIGH);
digitalWrite(4, LOW);
}
