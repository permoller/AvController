#define btnPin A0
#define ledPinStandbyMusic 2
#define ledPinTvProjector 3

#define Off 0
#define Standby 1
#define Tv 2
#define Music 3
#define Projector 4
#define Custom 5

#define None 0
#define Power 1

#define OffOff 0
#define OffOn 1
#define OnOff 2


void setup()
{
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(ledPinStandbyMusic, INPUT);
  pinMode(ledPinTvProjector, INPUT);
  #ifdef debug
  Serial.begin(9600);
  Serial.println("debug enabled");
  #endif
}

uint8_t _mode;
void loop()
{
  _mode = getModeFromButtons(_mode);
  setLedStateFromMode(_mode);
}


bool isBetween(int value, int min, int max)
{
  return value >= min && value <= max;
}

uint8_t readButtons()
{
  int value, value2, delta;
  uint8_t btn = None;
  do
  {
    value = analogRead(btnPin);
    if(value > 900)
      break;
    delay(10);
    value2 = analogRead(btnPin);
    delta = value-value2;
  } while (!isBetween(delta, -10, 10));

  if(isBetween(value, 450, 570))
  {
    btn = Standby;
  }
  else if(isBetween(value, 360, 380))
  {
    btn = Tv;
  }
  else if(isBetween(value, 0, 30))
  {
    btn = Music;
  }
  else if(isBetween(value, 220, 240))
  {
    btn = Projector;
  }
  return btn;
}

uint8_t _previousBtn;
uint8_t getModeFromButtons(uint8_t previousMode)
{
  uint8_t nextMode = previousMode;
  uint8_t btn = readButtons();
  if(btn != None)
  {
    #ifdef debug
    Serial.print("Button pressed "); Serial.println(btn);
    #endif
  }
  else if (_previousBtn != None)
  {
    #ifdef debug
    Serial.print("Button released "); Serial.println(_previousBtn);
    #endif
    switch(_previousBtn)
    {
      case Power:
        if (previousMode == Standby)
          nextMode = Off;
        else if (previousMode == Off)
          nextMode = Custom;
        else
          nextMode = Standby;
        break;
      case Tv:
        nextMode = Tv;
        break;
      case Music:
        nextMode = Music;
        break;
      case Projector:
        nextMode = Projector;
        break;
    }
  }
  _previousBtn = btn;
  return nextMode;
}

uint8_t setLedPinState(uint8_t pin, uint8_t previousState, uint8_t newState)
{
  if(previousState == newState)
    return newState;
  #ifdef debug
  Serial.print("pin "); Serial.println(pin);
  Serial.print("newState "); Serial.println(newState);
  #endif
  switch(newState)
  {
    case OffOff:
      pinMode(pin, INPUT);
      #ifdef debug
      Serial.println("mode INPUT");
      #endif
    break;
    case OffOn:
      //if(previousState == OffOff)
        pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
      #ifdef debug
      Serial.println("mode OUTPUT");
      Serial.println("value HIGH");
      #endif
    break;
    case OnOff:
      //if(previousState == OffOff)
        pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
            #ifdef debug
      Serial.println("mode OUTPUT");
      Serial.println("value LOW");
      #endif
    break;
  }
  return newState;
}

uint8_t _previousLedPinStandbyMusicState, _previousLedPinTvProjectorState;
void setLedState(uint8_t ledPinStandbyMusicState, uint8_t ledPinTvProjectorState)
{
  _previousLedPinStandbyMusicState = setLedPinState(ledPinStandbyMusic, _previousLedPinStandbyMusicState, ledPinStandbyMusicState);
  _previousLedPinTvProjectorState = setLedPinState(ledPinTvProjector, _previousLedPinTvProjectorState, ledPinTvProjectorState);
}

bool _customFlag;
void setLedStateFromMode(uint8_t mode)
{
  switch(mode)
  {
    case Off:
      setLedState(OffOff, OffOff);
      break;
    case Standby:
      setLedState(OnOff, OffOff);
    break;
    case Tv:
      setLedState(OffOff, OnOff);
    break;
    case Music:
      setLedState(OffOn, OffOff);
    break;
    case Projector:
      setLedState(OffOff, OffOn);
    break;
    case Custom:
      if(_customFlag)
        setLedState(OnOff, OnOff);
      else
        setLedState(OffOn, OffOn);
      _customFlag = !_customFlag;
    break;
  }
}



