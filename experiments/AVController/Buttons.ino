Button _previousBtn;
Mode handleButtons(Mode currentMode) {
  Mode nextMode = currentMode;
  Button btn = readCurrentlyPressedButton();

  if (_previousBtn == Button_None && btn != Button_None) {
#ifdef debugButton
    p("Button pressed ");
    pl(Button_Names[btn]);
#endif
    beep(50, 0, 1);
    switch (btn) {
      case Button_None:
        break;
      case Button_Power:
        if (currentMode == Mode_Off || currentMode == Mode_Standby)
          nextMode = Mode_Custom;
        else
          nextMode = Mode_Standby;
        break;
      case Button_Tv:
        nextMode = Mode_Tv;
        break;
      case Button_Music:
        if (currentMode == Mode_Music_Chromecast)
          nextMode = Mode_Music_InternetRadioPreset1;
        else if (currentMode == Mode_Music_InternetRadioPreset1)
          nextMode = Mode_Music_InternetRadioPreset2;
        else if (currentMode == Mode_Music_InternetRadioPreset2)
          nextMode = Mode_Music_InternetRadioPreset3;
        else
          nextMode = Mode_Music_Chromecast;
        break;
      case Button_Projector:
        // we can't turn on the projector if it has not been shut down yet from previously beeing turned on
        if((currentTime - projectorOutletTurnOffTime) > 0) {
          if(currentMode != Mode_Projector_Chromecast)
            nextMode = Mode_Projector_Chromecast;
          else
            nextMode = Mode_Projector_Vaux;
        } else {
          beep(500, 0, 1);
        }
        break;
    }
  }
  _previousBtn = btn;
  return nextMode;
}

Button readCurrentlyPressedButton() {
  int value, value2, delta;
  Button btn = Button_None;
  do {
    value = analogRead(btnPin);
    if (value > 900)
      break;
    delay(10);
    value2 = analogRead(btnPin);
    delta = value - value2;
  } while (!isBetween(delta, -10, 10));

#ifdef debugButton
  if (value < 900)
  {
    p("Button value ");
    pl(value);
  }
#endif

  if (isBetween(value, 450, 470)) {
    btn = Button_Power;
  } else if (isBetween(value, 360, 380)) {
    btn = Button_Tv;
  } else if (isBetween(value, 0, 30)) {
    btn = Button_Music;
  } else if (isBetween(value, 220, 240)) {
    btn = Button_Projector;
  }

  return btn;
}
