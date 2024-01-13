void sendAmpCmd(AmpCmd cmd) {
  #ifdef debugAmp
  p("Send amp command: ");
  pl(AmpCmd_Names[cmd]);
#endif
  sendIr(DenonAvr1912, AmpCmd_IrCodes[cmd], 0, true);
}

void sendAmpCmdWithoutDelay(AmpCmd cmd) {
#ifdef debugAmp
  p("Send amp command: ");
  pl(AmpCmd_Names[cmd]);
#endif
  sendIr(DenonAvr1912, AmpCmd_IrCodes[cmd], 0, false);
}

void sendTvCmd(TvCmd cmd) {
#ifdef debugTv
  p("Send tv command: ");
  pl(TvCmd_Names[cmd]);
#endif
  sendIr(RC6, TvCmd_IrCodes[cmd], 20, true);
}

void sendProjectorCmd(ProjectorCmd cmd) {
#ifdef debugProjector
  p("Send projector command: ");
  pl(ProjectorCmd_Names[cmd]);
#endif
  sendIr(NEC, ProjectorCmd_IrCodes[cmd], 0, true);
}

void sendIr(uint8_t protocol, uint32_t data, uint8_t numberOfBits, bool delayTransmition) {
  if(delayTransmition)
    delay(500);
  irReceiver.disableIRIn();
  irTransmitter.send(protocol, data, numberOfBits);
  irReceiver.enableIRIn();
}
