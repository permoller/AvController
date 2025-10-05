
void enterNextMode(Mode currentMode, Mode nextMode) {
  if(currentMode == nextMode)
    return;

#ifdef debugMode
  p("Entering mode "); pl(Mode_Names[nextMode]);
#endif
  
  // send power on commands and change input
  if (nextMode != Mode_Standby && nextMode != Mode_Off && (currentMode == Mode_Standby || currentMode == Mode_Off)) {
    sendAmpCmd(AmpCmd_PowerOn);
    delay(2000);
  }
  if ((nextMode == Mode_Projector_Chromecast || nextMode == Mode_Projector_Vaux) && currentMode != Mode_Projector_Chromecast && currentMode != Mode_Projector_Vaux) {
    sendProjectorScreenCmd(ProjectorScreenCmd_Down);
    sendProjectorCmd(ProjectorCmd_Power);
  }
  switch (nextMode) {
    case Mode_Standby:
    case Mode_Off:
    case Mode_Custom:
      break;
    case Mode_Tv:
      sendAmpCmd(AmpCmd_SourceSelectGame1);
      if(!tvModeAutoChanged) {
        sendTvCmd(TvCmd_Power);
        // wait for the tv to be ready to change channel
        // delay(8000);
        // change channel to the most used channel for the children so they don't have to change the channel manually
        // sendTvCmd(TvCmd_1);
        // sendTvCmd(TvCmd_0);
      }
      break;
    case Mode_Projector_Chromecast:
      sendAmpCmd(AmpCmd_SourceSelectGame2);
      break;
    case Mode_Projector_Vaux:
      sendAmpCmd(AmpCmd_SourceSelectVaux);
      break;
    case Mode_Music_Chromecast:
      sendAmpCmd(AmpCmd_SourceSelectGame2);
      break;
    case Mode_Music_InternetRadioPreset1:
      sendAmpCmd(AmpCmd_InternetRadio);
      sendAmpCmd(AmpCmd_InternetRadioPreset1);
      break;
    case Mode_Music_InternetRadioPreset2:
      sendAmpCmd(AmpCmd_InternetRadioPreset2);
      break;
    case Mode_Music_InternetRadioPreset3:
      sendAmpCmd(AmpCmd_InternetRadioPreset3);
      break;
  }
}
