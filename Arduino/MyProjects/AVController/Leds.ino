unsigned long _ledStartMillis = 0;
void updateLeds(Mode mode) {
  unsigned long currentMillis, elapsedMillis;
  switch (mode) {
    case Mode_Off:
      setLedState(LedState_OffOff, LedState_OffOff);
      break;
    case Mode_Standby:
      setLedState(LedState_OnOff, LedState_OffOff);
      break;
    case Mode_Tv:
      setLedState(LedState_OffOff, LedState_OnOff);
      break;
    case Mode_Music_Chromecast:
      setLedState(LedState_OffOn, LedState_OffOff);
      break;
    case Mode_Music_InternetRadioPreset1:
      currentMillis = millis();
      elapsedMillis = currentMillis - _ledStartMillis;
      if (isBetween(elapsedMillis, 0, 2500))
        setLedState(LedState_OffOn, LedState_OffOff);
      else if (isBetween(elapsedMillis, 2500, 2750))
        setLedState(LedState_OffOff, LedState_OffOff);
      else
        _ledStartMillis = currentMillis;
      break;
    case Mode_Music_InternetRadioPreset2:
      currentMillis = millis();
      elapsedMillis = currentMillis - _ledStartMillis;
      if (isBetween(elapsedMillis, 0, 2500))
        setLedState(LedState_OffOn, LedState_OffOff);
      else if (isBetween(elapsedMillis, 2500, 2750))
        setLedState(LedState_OffOff, LedState_OffOff);
      else if (isBetween(elapsedMillis, 2750, 3000))
        setLedState(LedState_OffOn, LedState_OffOff);
      else if (isBetween(elapsedMillis, 3000, 3250))
        setLedState(LedState_OffOff, LedState_OffOff);
      else
        _ledStartMillis = currentMillis;
      break;
    case Mode_Music_InternetRadioPreset3:
      currentMillis = millis();
      elapsedMillis = currentMillis - _ledStartMillis;
      if (isBetween(elapsedMillis, 0, 2500))
        setLedState(LedState_OffOn, LedState_OffOff);
      else if (isBetween(elapsedMillis, 2500, 2750))
        setLedState(LedState_OffOff, LedState_OffOff);
      else if (isBetween(elapsedMillis, 2750, 3000))
        setLedState(LedState_OffOn, LedState_OffOff);
      else if (isBetween(elapsedMillis, 3000, 3250))
        setLedState(LedState_OffOff, LedState_OffOff);
      else if (isBetween(elapsedMillis, 3250, 3500))
        setLedState(LedState_OffOn, LedState_OffOff);
      else if (isBetween(elapsedMillis, 3500, 3750))
        setLedState(LedState_OffOff, LedState_OffOff);
      else
        _ledStartMillis = currentMillis;
      break;
    case Mode_Projector_Chromecast:
    case Mode_Projector_Vaux:
      setLedState(LedState_OffOff, LedState_OffOn);
      break;
    case Mode_Custom:
      if (_ledStartMillis == 0)
        setLedState(LedState_OnOff, LedState_OnOff);
      else
        setLedState(LedState_OffOn, LedState_OffOn);
      _ledStartMillis = _ledStartMillis == 0 ? 1 : 0;
      break;
  }
}

LedState _previousStandbyMusicLedState, _previousTvProjectorLedState;
void setLedState(LedState standbyMusicLedState, LedState tvProjectorLedState) {
  _previousStandbyMusicLedState = setLedState2(standbyMusicLedPin, _previousStandbyMusicLedState, standbyMusicLedState);
  _previousTvProjectorLedState = setLedState2(tvProjectorLedPin, _previousTvProjectorLedState, tvProjectorLedState);
}

LedState setLedState2(uint8_t pin, LedState previousState, LedState newState) {
  if (previousState == newState)
    return newState;
#ifdef debugLed
  p("Led pin: "); pl(pin);
  p("new pin state: "); pl(LedState_Names[newState]);
#endif
  switch (newState) {
    case LedState_OffOff:
      pinMode(pin, INPUT);
#ifdef debugLed
      pl("led pin mode INPUT");
#endif
      break;
    case LedState_OffOn:
      if (previousState == LedState_OffOff)
        pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
#ifdef debugLed
      pl("led pin mode OUTPUT");
      pl("led pin value HIGH");
#endif
      break;
    case LedState_OnOff:
      if (previousState == LedState_OffOff)
        pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
#ifdef debugLed
      pl("led pin mode OUTPUT");
      pl("led pin value LOW");
#endif
      break;
  }
  return newState;
}

