int counter = 0;
int led = HIGH;

void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(counter);
  counter++;
  if(led==HIGH)
    led = LOW;
  else
    led = HIGH;
  digitalWrite(LED_BUILTIN, led);
  delay(1000);
}
