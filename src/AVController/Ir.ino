//uint32_t _irPreviousRawValueReceived = 0;
//time_t _irPreviousTime = 0;
Mode handleIr(Mode currentMode) {
  uint32_t value = 0;
  
  if(irReceiver.getResults()) {
    irDecoder.decode();

#ifdef debugTv
  pl("Recieved IR signal");
  if(irDecoder.protocolNum == DenonAvr1912)
    pl(irDecoder.value);
  irDecoder.dumpResults(false);
#endif

    // we only listen for commands from the tv-remote
    if(irDecoder.protocolNum == RC6 && irDecoder.bits == 20)
      value = irDecoder.value & 0xfffeffff; // mask toggle bit
    
    irReceiver.enableIRIn();
  }
  if (value == 0) {
  } else if (value == TvCmd_IrCodes[TvCmd_VolumeUp]) {
    sendAmpCmdWithoutDelay(AmpCmd_VolumeUp);
  } else if (value == TvCmd_IrCodes[TvCmd_VolumeDown]) {
    sendAmpCmdWithoutDelay(AmpCmd_VolumeDown);
  } else if (value == TvCmd_IrCodes[TvCmd_Mute]) {
    sendAmpCmd(AmpCmd_Mute);
  }
  return currentMode;
//  
//  if (!irReceiver.getResults()) {
//    _irPreviousRawValueReceived = 0;
//    return currentMode;
//  }
//
//  irDecoder.decode();
//
//#ifdef debugTv
//  pl("Recieved IR signal");
//  irDecoder.dumpResults(false);
//#endif
//
//  uint8_t protocolNum = irDecoder.protocolNum;
//  uint8_t bits = irDecoder.bits;
//  uint32_t rawValue = irDecoder.value;
//  // mask toggle bit
//  uint32_t value = rawValue & 0xfffeffff;
//  
//  irReceiver.enableIRIn();
//  
//  // we only handle commands from the tv-remote
//  if (protocolNum != RC6 || bits != 20) {
//    return currentMode;
//  }
//  
//  // if we receive the same command multiple times without the togle-bit changing we only handle the first
//  if (_irPreviousRawValueReceived == rawValue && (currentTime - _irPreviousTime) < 10) {
//#ifdef debugTv
//    pl("Ignoring repeated TV command");
//#endif
//    return currentMode;
//  }
//
//  _irPreviousRawValueReceived = rawValue;
//  _irPreviousTime = currentTime;
//
//#ifdef debugTv
//  const char *cmdName = "Unknown";
//  int arraySize = sizeof(TvCmd_IrCodes) / sizeof(TvCmd_IrCodes[0]);
//  for (int i = 0; i < arraySize; i++)
//  {
//    if (TvCmd_IrCodes[i] == value) {
//      cmdName = TvCmd_Names[i];
//      break;
//    }
//  }
//  p("Tv command received: ");
//  pl(cmdName);
//#endif
//
//  
//  /* We no longer turn the tv on when a power signal is received. We rely on TvUsbPower to detect when the tv is on.
//   if (value == TvCmd_IrCodes[TvCmd_Power]) {
//    // if mode is standby we enter tv mode, else we enter standby
//    if (currentMode == Mode_Standby) {
//      // the tv will most likely have received the power command so we don't need to send it
//      dontSendTvPowerCommand = true;
//      return Mode_Tv;
//    } else if (currentMode == Mode_Tv) {
//      // the tv will most likely have received the power command so we don't need to send it
//      dontSendTvPowerCommand = true;
//      return Mode_Standby;
//    }
//  } else*/ if (value == TvCmd_IrCodes[TvCmd_VolumeUp]) {
//    sendAmpCmd(AmpCmd_VolumeUp);
//  } else if (value == TvCmd_IrCodes[TvCmd_VolumeDown]) {
//    sendAmpCmd(AmpCmd_VolumeDown);
//  } else if (value == TvCmd_IrCodes[TvCmd_Mute]) {
//    sendAmpCmd(AmpCmd_Mute);
//  }
//  return currentMode;
}


