// the power on the USB-port on the tv is somtimes on even when the tv is in standby.
// Examples are while the tv is booting after getting power and and for a while after turning the tv off
// So we also look at the total current being drawn to guess if the tv is on or off
time_t _powerCheckPreviousTime = 0;

Mode handleTvUsbPower(Mode currentMode, time_t currentModeEnteredTime) {
  tvModeAutoChanged = false;
  
  // we only care about the tv-state if we expect it to be in a given state (Mode_Tv or Mode_Standby)
  // We also only check every 5 seconds
  if((currentMode != Mode_Tv && currentMode != Mode_Standby) || (currentTime - _powerCheckPreviousTime) < 5)
    return currentMode;
  
  _powerCheckPreviousTime = currentTime;
  
  // if we are in standby, but the tv is outputting power on the USB port and we are using more power than normal standby-power (5-10)
  // the tv must have been turned on
  if(currentMode == Mode_Standby && doesTvOutputPowerOnUsb(false) && readCurrentSensor() > 15) {
#ifdef debugTvUsbPower
    pl("Changing mode to TV");
#endif
    tvModeAutoChanged = true;
    return Mode_Tv;

  // if we are in tv-mode but the tv is off (no usb power or lower power usage than normal tv mode power (25-40)) we change mode to standby
  } else if(currentMode == Mode_Tv && (!doesTvOutputPowerOnUsb(true) || readCurrentSensor() < 17)) {
#ifdef debugTvUsbPower
    pl("Changing mode to Standby");
#endif
    tvModeAutoChanged = true;
    return Mode_Standby;
  }

  return currentMode;
}

bool doesTvOutputPowerOnUsb(bool expected) {
  for(int i=0; i<5; i++) {
    if(expected) {
      if(analogRead(tvUsbPowerSensorPin) < 1000)
        return true;
    } else {
      if(analogRead(tvUsbPowerSensorPin) > 100)
        return false;
    }
    delay(100);
  }
#ifdef debugTvUsbPower
    if(expected)
      pl("Tv USB power was off, but expected to be on");
    else
      pl("Tv USB power was on, but expected to be off");
#endif
  return !expected;
}
