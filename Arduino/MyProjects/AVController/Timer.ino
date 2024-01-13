bool _timerWarningTriggered = false;
Mode handleTimer(Mode currentMode, time_t currentModeEnteredTime) {
  int h = hour(epochToLocal(currentTime));
  time_t idleTime, idleTimeBeforePowerOff;
    
  // Automatically turn on in the morning so we will have internet before we get out of bet
  // and automatically turn off if there has been no activity for some time
  if(currentMode == Mode_Off && isBetween(h, 5, 8)) {
    return Mode_Standby;
  } else if(currentMode == Mode_Standby && !isBetween(h, 5, 8)) {
    idleTime = currentTime - max(currentModeEnteredTime, motionDetectedTime);

    idleTimeBeforePowerOff = 3 * SECS_PER_HOUR;

    if(idleTime > idleTimeBeforePowerOff) {
      return Mode_Off;
    } else if(idleTime > (idleTimeBeforePowerOff - (5 * SECS_PER_MIN))) {
      if(!_timerWarningTriggered) {
        beep(500, 500, 5);
        _timerWarningTriggered = true;
      }
      return currentMode;
    }
  }
  _timerWarningTriggered = false;
  return currentMode;
}

