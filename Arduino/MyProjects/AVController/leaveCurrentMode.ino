
void leaveCurrentMode(Mode currentMode, Mode nextMode) {
  if(currentMode == nextMode)
    return;
  
  bool somethingWasTurnedOff = false;
  
#ifdef debugMode
  p("Leaving mode "); pl(Mode_Names[currentMode]);
#endif

  // send power off commands
  if (currentMode != Mode_Standby && currentMode != Mode_Off && (nextMode == Mode_Standby || nextMode == Mode_Off)) {
    sendAmpCmd(AmpCmd_PowerOff);
    somethingWasTurnedOff = true;
  }
  if (currentMode == Mode_Tv) {
    if(!tvModeAutoChanged) {
      sendTvCmd(TvCmd_Power);
      somethingWasTurnedOff = true;
    }
  }
  if ((currentMode == Mode_Projector_Chromecast || currentMode == Mode_Projector_Vaux) && nextMode != Mode_Projector_Chromecast && nextMode != Mode_Projector_Vaux) {
    sendProjectorScreenCmd(ProjectorScreenCmd_Up);
    delay(6000);
    // we send the power command twice with 1 second between to turn the projector off
    // the projector does not alway receive the signal so we send it a couple of times
    sendProjectorCmd(ProjectorCmd_Power);
    delay(1000);
    sendProjectorCmd(ProjectorCmd_Power);
    delay(1000);
    sendProjectorCmd(ProjectorCmd_Power);
    delay(1000);
    sendProjectorCmd(ProjectorCmd_Power);
    
    projectorOutletTurnOffTime = currentTime + (2 * SECS_PER_MIN);

    somethingWasTurnedOff = true;
  }

  if(somethingWasTurnedOff) {
    delay(2000);
  }
}



