
void beep(uint16_t beepDurationMilliseconds, uint16_t pauseDurationMilliseconds, uint8_t numberOfBeeps) {
  // Active Buzzer. Makes sound when pin is low
  for (int i = 0; i < numberOfBeeps; i++) {
    digitalWrite(buzzerPin, LOW);
    delay(beepDurationMilliseconds);
    digitalWrite(buzzerPin, HIGH);
    delay(pauseDurationMilliseconds);
  }
}


bool isBetween(int value, int min, int max) {
  return value >= min && value <= max;
}


void fatalError() {
  while (true) {
    setLedState(LedState_OffOn, LedState_OffOn);
    beep(100,1000,1);
    setLedState(LedState_OnOff, LedState_OnOff);
    beep(100,1000,1);
  }
}

int readCurrentSensor() {
  int minValue = 1024, maxValue = 0, value;
  
  for(int i=0; i<400; i++)
  {
    value = analogRead(currentSensorPin);
    if(value < minValue)
      minValue = value;
    if(value > maxValue)
      maxValue = value;
  }
  value = maxValue - minValue;
#ifdef debugCurrentSensor
    p("Current sensor: ");
    pl(value);
#endif
  return value;
}


#ifdef debugAny
time_t lastPrintTime = 0;

void printLocalTimeIfChanged() {
  if (lastPrintTime != currentTime) {
    lastPrintTime = currentTime;
    Serial.println();
    Serial.println("----------------------------");
    pl(epochToLocal(currentTime));
  }
}

void p(const char *str) {
  printLocalTimeIfChanged();
  Serial.print(str);
}
void pl(const char *str) {
  printLocalTimeIfChanged();
  Serial.println(str);
}

void p(int number) {
  printLocalTimeIfChanged();
  Serial.print(number);
}
void pl(int number) {
  p(number);
  Serial.println();
}

void p(time_t t) {
  printLocalTimeIfChanged();
  p(year(t));
  p("-");
  p(month(t));
  p("-");
  p(day(t));
  p(" ");
  print2digits(hour(t));
  p(":");
  print2digits(minute(t));
  p(":");
  print2digits(second(t));
}
void pl(time_t t) {
  p(t);
  Serial.println();
}

void print2digits(int number) {
  printLocalTimeIfChanged();
  if (number >= 0 && number < 10)
    Serial.write('0');
  Serial.print(number);
}
#endif
