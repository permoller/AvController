
void sendProjectorScreenCmd(ProjectorScreenCmd cmd) {
#ifdef debugProjectorScreen
  p("Send projector screen command: ");
  pl(ProjectorScreenCmd_Names[cmd]);
#endif

  uint32_t data = ProjectorScreenCmd_RfCodes[cmd];

  for (int i = 0; i < 20; i++) {
    digitalWrite(rfTransmitterPin, HIGH);
    delayMicroseconds(280);
    digitalWrite(rfTransmitterPin, LOW);
    delayMicroseconds(13160);
    for (int j = 8; j < 32; j++) {
      bool b = ((data << j) & 0x80000000) != 0;
      digitalWrite(rfTransmitterPin, HIGH);
      delayMicroseconds(b ? 1120 : 280);
      digitalWrite(rfTransmitterPin, LOW);
      delayMicroseconds(!b ? 1120 : 280);
    }
  }
}

