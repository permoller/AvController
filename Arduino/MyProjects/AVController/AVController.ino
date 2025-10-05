
#include <IRLibRecvPCI.h>
#include <IRLibDecodeBase.h>
#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLib_P04_rc6.h>
#include <IRLib_P13_DenonAvr1912.h>
#include <IRLibCombo.h>
#include <TimeLib.h>
#include <DS1302RTC.h>

#define debugModeNo
#define debugLedNo
#define debugButtonNo
#define debugTvNo
#define debugAmpNo
#define debugProjectorNo
#define debugProjectorScreenNo
// if the arduino keeps beeping after boot try to debug the Realtime Clock
#define debugRtcNo
#define debugBuzzerNo
#define debugPirNo
#define debugOutletNo
#define debugMotionNo
#define debugTvUsbPowerNo
#define debugCurrentSensorNo

#if defined(debugMode) || defined(debugLed) || defined(debugButton) || defined(debugTv) || defined(debugAmp) || defined(debugProjector) || defined(debugProjectorScreen) || defined(debugRtc) || defined(debugBuzzer) || defined(debugPir) || defined(debugOutlet) || defined(debugMotion) || defined(debugTvUsbPower) || defined(debugCurrentSensor)
#define debugAny
#endif


// ir receiver pin requires pin-changed interrupts
#define irReceiverPin 2
// ir transmitter pin requires pulse with modulation. It is defined in the library as pin 3
//#define irTransmitterPin 3
#define standbyMusicLedPin 4
#define tvProjectorLedPin 5
#define rfTransmitterPin 6
#define relay1Pin 7
#define relay2Pin 8
#define relay3Pin 9
#define relay4Pin 10
#define relay5Pin 11
#define relay6Pin 12

#define rtcClockPin A0
#define rtcDataPin A1
#define rtcChipEnabledPin A2
// the buttons requires an analog input with internal pull-up resistor
#define btnPin A3
#define pirSensorPin A4
#define buzzerPin A5
// Note: A6 and A7 are analog-only and has no internal pullup
// Current sensor needs analog input pin
#define currentSensorPin A6
#define tvUsbPowerSensorPin A7


enum Outlet {                                          Outlet_AlwaysOn, Outlet_RouterAndTv, Outlet_HdmiSplitter, Outlet_Chromecast, Outlet_ProjectorAndScreen, Outlet_AmpAndSub };
#ifdef debugOutlet
static const char *Outlet_Names[] = {                  "AlwaysOn",      "RouterAndTv",      "HdmiSplitter",      "Chromecast",      "ProjectorAndScreen",      "AmpAndSub" };
#endif
static const uint8_t Outlet_RelayPins[] = {            relay1Pin, relay2Pin,        relay3Pin,           relay4Pin,         relay5Pin,                 relay6Pin };
static uint8_t Outlet_RelayPinStates[] = {             HIGH,      HIGH,             HIGH,                HIGH,              HIGH,                      HIGH };


enum Mode {                                            Mode_Off,   Mode_Standby,   Mode_Tv,   Mode_Music_Chromecast,   Mode_Music_InternetRadioPreset1,   Mode_Music_InternetRadioPreset2,   Mode_Music_InternetRadioPreset3,   Mode_Projector_Chromecast,   Mode_Projector_Vaux,  Mode_Custom };
#ifdef debugMode
static const char *Mode_Names[] = {                    "Off",      "Standby",      "Tv",      "Music_Chromecast",      "Music_InternetRadioPreset1",      "Music_InternetRadioPreset2",      "Music_InternetRadioPreset3",      "Projector_Chromecast",      "Projector_Vaux",     "Custom" };
#endif

// 2 leds to one pin.
// When pin is input both leds are of. When pin is high one led is on and when pin is low the other led is on
enum LedState {                                        LedState_OffOff,   LedState_OffOn,   LedState_OnOff };
#ifdef debugLed
static const char *LedState_Names[] = {                "OffOff",          "OffOn",          "OnOff" };
#endif

// 4 momentary push buttons connected to 1 analog input with different resistors to ground
enum Button {                                          Button_None,   Button_Power,   Button_Tv,   Button_Music,   Button_Projector };
#ifdef debugButton
static const char *Button_Names[] = {                  "None",        "Power",        "Tv",        "Music",        "Projector" };
#endif

// Tv: Phillips (IR Protocol: RC6 20bit)
enum TvCmd {                                           TvCmd_Power,   TvCmd_Tv,   TvCmd_VolumeUp,   TvCmd_VolumeDown,   TvCmd_Mute,   TvCmd_Netflix,   TvCmd_ChannelUp,   TvCmd_ChannelDown,   TvCmd_ArrowUp,   TvCmd_ArrowDown,   TvCmd_ArrowLeft,   TvCmd_ArrowRight,   TvCmd_Ok,   TvCmd_0,   TvCmd_1 };
static const byte TvCmd_IrCodes[] = {                  0x0C,          0x9F,       0x10,             0x11,               0x0D,         0x76,            0x21,              0x20,                0x58,            0x59,              0x5A,              0x5B,               0x5C,       0x00,      0x01 };
#ifdef debugTv
static const char *TvCmd_Names[] = {                   "Power",       "Tv",       "VolumeUp",       "VolumeDown",       "Mute",       "Netflix",       "ChannelUp",       "ChannelDown",       "ArrowUp",       "ArrowDown",       "ArrowLeft",       "ArrowRight",       "Ok",       "0",       "1" };
#endif

// Amp: Denon Avr1912 (IR)
enum AmpCmd {                                          AmpCmd_PowerOn,   AmpCmd_PowerOff,   AmpCmd_SourceSelectBd,   AmpCmd_SourceSelectTv,   AmpCmd_SourceSelectGame1,   AmpCmd_SourceSelectGame2,   AmpCmd_SourceSelectVaux,   AmpCmd_Mute,   AmpCmd_VolumeUp,   AmpCmd_VolumeDown,   AmpCmd_InternetRadio,   AmpCmd_InternetRadioPreset1,   AmpCmd_InternetRadioPreset2,   AmpCmd_InternetRadioPreset3 };
static const uint32_t AmpCmd_IrCodes[] = {             0x840086,         0x8C008E,          0x88B43E,                0x84B432,                0x8AB43C,                   0x8EB438,                   0x89B43F,                  0x84E86E,      0x80E86A,          0x88E862,            0x8A74FC,               0x84F076,                      0x8CF07E,                      0x82F070 };
#ifdef debugAmp
static const char *AmpCmd_Names[] {                    "PowerOn",        "PowerOff",        "SourceSelectBd",        "SourceSelectTv",        "SourceSelectGame1",        "SourceSelectGame2",        "SourceSelectVaux"         "Mute",        "VolumeUp",        "VolumeDown",        "InternetRadio",        "InternetRadioPreset1",        "InternetRadioPreset2",        "InternetRadioPreset3" };
#endif

// Projector: BenQ W1070 (IR Protocol: NEC)
enum ProjectorCmd {                                    ProjectorCmd_Power,   ProjectorCmd_Mute };
static const uint32_t ProjectorCmd_IrCodes[] = {       0xC40BF,              0xC28D7 };
#ifdef debugProjector
static const char *ProjectorCmd_Names[] {              "Power",              "Mute" };
#endif

// Projector screen: Media mount (RF 433Mhz)
enum ProjectorScreenCmd {                              ProjectorScreenCmd_Up,   ProjectorScreenCmd_Down };
static const uint32_t ProjectorScreenCmd_RfCodes[] = { 0x847161,                0x847164 };
#ifdef debugProjectorScreen
static const char *ProjectorScreenCmd_Names[] {        "Up",                    "Down" };
#endif


// used globally to get the same current time during the main loop
time_t currentTime;

// used to indicate a point in time in the future when it is ok to turn the power off
time_t projectorOutletTurnOffTime = 0;

// the most resent point in time when the motion sensor detected motion
time_t motionDetectedTime;


bool tvModeAutoChanged = false;

IRsend irTransmitter;
IRrecvPCI irReceiver(irReceiverPin);
IRdecode irDecoder;
DS1302RTC RTC(rtcChipEnabledPin, rtcDataPin, rtcClockPin);



void setup() {

#ifdef debugAny
  Serial.begin(9600);
  while (!Serial);
  pl("");
  pl("Debug enabled");
#endif

  irReceiver.enableIRIn();
  pinMode(standbyMusicLedPin, INPUT);
  pinMode(tvProjectorLedPin, INPUT);
  digitalWrite(rfTransmitterPin, LOW);
  pinMode(rfTransmitterPin, OUTPUT);
  digitalWrite(relay1Pin, HIGH);
  pinMode(relay1Pin, OUTPUT);
  digitalWrite(relay2Pin, HIGH);
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay3Pin, HIGH);
  pinMode(relay3Pin, OUTPUT);
  digitalWrite(relay4Pin, HIGH);
  pinMode(relay4Pin, OUTPUT);
  digitalWrite(relay5Pin, HIGH);
  pinMode(relay5Pin, OUTPUT);
  digitalWrite(relay6Pin, HIGH);
  pinMode(relay6Pin, OUTPUT);
  pinMode(pirSensorPin, INPUT);
  pinMode(btnPin, INPUT_PULLUP);
  digitalWrite(buzzerPin, HIGH);
  pinMode(buzzerPin, OUTPUT);
  pinMode(currentSensorPin, INPUT);
  pinMode(tvUsbPowerSensorPin, INPUT);

  initTime();
  motionDetectedTime = now();

#ifdef debugAny
  pl("Setup done");
#endif
  beep(50, 200, 3);
}

Mode _currentMode = Mode_Off;
Mode _nextMode = Mode_Off;
time_t _currentModeEnteredTime = 0;
void loop() {
  tvModeAutoChanged = false;
  updateCurrentTime();
  if(_currentMode == _nextMode)
    _nextMode = handleIr(_currentMode);
  if(_currentMode == _nextMode)
    _nextMode = handleButtons(_currentMode);
  if(_currentMode == _nextMode)
    _nextMode = handleTvUsbPower(_currentMode, _currentModeEnteredTime);
  //if(_currentMode == _nextMode)
  //  _nextMode = handleMotion(_currentMode);
  if(_currentMode == _nextMode)
    _nextMode = handleTimer(_currentMode, _currentModeEnteredTime);

  if(_currentMode != _nextMode) {
    updateLeds(Mode_Off);
    leaveCurrentMode(_currentMode, _nextMode);
    updatePowerOutlets(_nextMode);
    enterNextMode(_currentMode, _nextMode);
#ifdef debugMode
    p("Mode changed from "); p(Mode_Names[_currentMode]); p(" to "); pl(Mode_Names[_nextMode]);
#endif
    _currentModeEnteredTime = currentTime;
    _currentMode = _nextMode;
  }
  updateLeds(_nextMode);

}

