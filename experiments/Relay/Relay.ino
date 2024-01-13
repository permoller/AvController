
void setup() {
  // put your setup code here, to run once:

pinMode(2, INPUT_PULLUP);
pinMode(3, INPUT_PULLUP);
pinMode(4, INPUT_PULLUP);
pinMode(5, INPUT_PULLUP);
pinMode(A0, OUTPUT);
pinMode(A1, OUTPUT);
pinMode(A2, OUTPUT);
pinMode(A3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(A0, digitalRead(2));
  digitalWrite(A1, digitalRead(3));
  digitalWrite(A2, digitalRead(4));
  digitalWrite(A3, digitalRead(5));

}
