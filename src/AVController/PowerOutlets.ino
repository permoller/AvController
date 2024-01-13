void updatePowerOutlets(Mode mode) {
  bool powerChangedAlwaysOn = outletPower(Outlet_AlwaysOn, true);
  bool powerChangedRouterAndTv = outletPower(Outlet_RouterAndTv, mode != Mode_Off);
  bool powerChangedAmpAndSub = outletPower(Outlet_AmpAndSub, mode != Mode_Standby && mode != Mode_Off);
  bool powerChangedHdmiSplitter = outletPower(Outlet_HdmiSplitter, mode == Mode_Custom || mode == Mode_Tv || mode == Mode_Projector_Chromecast);
  bool powerChangedChromecast = outletPower(Outlet_Chromecast, mode == Mode_Custom || mode == Mode_Projector_Chromecast || mode == Mode_Music_Chromecast);
  bool powerChangedProjectorAndScreen = outletPower(Outlet_ProjectorAndScreen, mode == Mode_Custom || mode == Mode_Projector_Chromecast || mode == Mode_Projector_Vaux || projectorOutletTurnOffTime > currentTime);
  
  if(powerChangedAlwaysOn || powerChangedRouterAndTv || powerChangedAmpAndSub || powerChangedHdmiSplitter || powerChangedChromecast || powerChangedProjectorAndScreen) {
    delay(2000);
  }
}


// Power to outlets is connected through "normally open" on relay, which is open when control pin is high (or not connected).
// Set pin low to turn outlet on and high to turn outlet off
bool outletPower(Outlet outlet, bool on) {
  uint8_t pinState = on ? LOW : HIGH;
  if (Outlet_RelayPinStates[outlet] != pinState) {
#ifdef debugOutlet
    p(Outlet_Names[outlet]); p(" powered "); pl(pinState == HIGH ? "off" : "on");
#endif
    digitalWrite(Outlet_RelayPins[outlet], pinState);
    Outlet_RelayPinStates[outlet] = pinState;
    return true;
  }
  return false;
}
