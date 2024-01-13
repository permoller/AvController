#include <IRLibRecvPCI.h>
#include <IRLibDecodeBase.h>
#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLib_P04_rc6.h>
#include <IRLib_P13_DenonAvr1912.h>
#include <IRLibCombo.h>
#define debug

#define btnPin A0
// ir receiver pin requires pin-changed interrupts
#define irReceiverPin 2
// ir transmitter pin requires pulse with modulation. It is defined in the library as pin 3
//#define irTransmitterPin 3
#define standbyMusicLedPin 4
#define tvProjectorLedPin 5


enum Mode { Mode_Off, Mode_Standby, Mode_Tv, Mode_Music_Chromecast, Mode_Music_InternetRadioPreset1, Mode_Music_InternetRadioPreset2, Mode_Music_InternetRadioPreset3, Mode_Projector, Mode_Custom };
#ifdef debug
static const char *Mode_Names[] = { "Mode_Off", "Mode_Standby", "Mode_Tv", "Mode_Music_Chromecast", "Mode_Music_InternetRadioPreset1", "Mode_Music_InternetRadioPreset2", "Mode_Music_InternetRadioPreset3", "Mode_Projector", "Mode_Custom" };
#endif

enum TvCmd { TvCmd_None, TvCmd_Power, TvCmd_Tv, TvCmd_VolumeUp, TvCmd_VolumeDown, TvCmd_Mute, TvCmd_Netflix, TvCmd_ChannelUp, TvCmd_ChannelDown, TvCmd_ArrowUp, TvCmd_ArrowDown, TvCmd_ArrowLeft, TvCmd_ArrowRight, TvCmd_Ok };
static const byte TvCmd_IrCodes[] { 0x0, 0x0C, 0x9F, 0x10, 0x11, 0x0D, 0x76, 0x21, 0x20, 0x58, 0x59, 0x5A, 0x5B, 0x5C };
#ifdef debug
static const char *TvCmd_Names[] = { "TvCmd_None", "TvCmd_Power", "TvCmd_Tv", "TvCmd_VolumeUp", "TvCmd_VolumeDown", "TvCmd_Mute", "TvCmd_Netflix", "TvCmd_ChannelUp", "TvCmd_ChannelDown", "TvCmd_ArrowUp", "TvCmd_ArrowDown", "TvCmd_ArrowLeft", "TvCmd_ArrowRight", "TvCmd_Ok" };
#endif

enum LedPinState { LedPinState_OffOff, LedPinState_OffOn, LedPinState_OnOff };
#ifdef debug
static const char *LedPinState_Names[] = { "LedPinState_OffOff", "LedPinState_OffOn", "LedPinState_OnOff" };
#endif

enum Button { Button_None, Button_Power, Button_Tv, Button_Music, Button_Projector };
#ifdef debug
static const char *Button_Names[] = { "Button_None", "Button_Power", "Button_Tv", "Button_Music", "Button_Projector" };
#endif

IRsend irTransmitter;
IRrecvPCI irReceiver(irReceiverPin);
IRdecode irDecoder;

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(standbyMusicLedPin, INPUT);
  pinMode(tvProjectorLedPin, INPUT);
  irReceiver.enableIRIn();
#ifdef debug
  Serial.begin(9600);
  Serial.println();
  Serial.println("debug enabled");
#endif
}


Mode _previousMode;
void loop() {

  Mode nextMode = _previousMode;
  // nextMode = readIr(_previousMode, nextMode);
  if (nextMode == _previousMode)
    nextMode = readButtons(_previousMode, nextMode);
  //  if(nextMode == _previousMode)
  //    nextMode = readPirSensor(_previousMode, nextMode);
  //  if(nextMode == _previousMode)
  //    nextMode = readRealTimeClock(_previousMode, nextMode);
#ifdef debug
  if (_previousMode != nextMode) {
    Serial.print("nextMode ");
    Serial.println(Mode_Names[nextMode]);
  }
#endif
  updateLeds(_previousMode, nextMode);
  //updateRelays(_previousMode, nextMode);
  updateAmp(_previousMode, nextMode);
  updateTv(_previousMode, nextMode);
  updateProjector(_previousMode, nextMode);
  updateProjectorScreen(_previousMode, nextMode);
  _previousMode = nextMode;
}


// =========== IR START =============
Mode readIr(Mode previousMode) {
  
  if (irReceiver.getResults()) {
    irDecoder.decode();
#ifdef debug
    irDecoder.dumpResults(true);
#endif
    if (irDecoder.protocolNum == RC6) {

      // mask toggle bit
      uint32_t value = irDecoder.value & 0xfffeffff;
      TvCmd cmd = TvCmd_None;
      int arraySize = sizeof(TvCmd_IrCodes)/sizeof(TvCmd_IrCodes[0]);
      for(int i=0; i < arraySize; i++)
      {
        if(TvCmd_IrCodes[i] == value)
          cmd = (TvCmd)i;
      }
      #ifdef debug
      Serial.println(TvCmd_Names[cmd]);
      #endif
      switch(cmd)
      {
        case TvCmd_None:
        break;
        case TvCmd_Power:
        if(previousMode == Mode_Off || previousMode == Mode_Standby) {
          return Mode_Tv;
        } else {
          return Mode_Standby;
        }
        case TvCmd_VolumeUp:
        irTransmitter.send(DenonAvr1912, 0x80E86A, 24);
        break;
        case TvCmd_VolumeDown:
        irTransmitter.send(DenonAvr1912, 0x88E862, 24);
        break;
        default:
        break;
      }
    }
    irReceiver.enableIRIn();
  }
  return previousMode;
}
// ----------- IR END -----------


// =========== BUTTON START =============
Button _previousBtn;
Mode readButtons(Mode previousMode, Mode nextMode) {

  Button btn = readButtons2();

  if (_previousBtn != Button_None && btn == Button_None) {
#ifdef debug
    Serial.print("Button  ");
    Serial.println(Button_Names[btn]);
#endif
    switch (_previousBtn) {
      case Button_None:
        break;
      case Button_Power:
        if (previousMode == Mode_Standby)
          nextMode = Mode_Off;
        else if (previousMode == Mode_Off)
          nextMode = Mode_Custom;
        else
          nextMode = Mode_Standby;
        break;
      case Button_Tv:
        nextMode = Mode_Tv;
        break;
      case Button_Music:
        if (previousMode == Mode_Music_Chromecast)
          nextMode = Mode_Music_InternetRadioPreset1;
        else if (previousMode == Mode_Music_InternetRadioPreset1)
          nextMode = Mode_Music_InternetRadioPreset2;
        else if (previousMode == Mode_Music_InternetRadioPreset2)
          nextMode = Mode_Music_InternetRadioPreset3;
        else
          nextMode = Mode_Music_Chromecast;
        break;
      case Button_Projector:
        nextMode = Mode_Projector;
        break;
    }
  }
  _previousBtn = btn;
  return nextMode;
}

Button readButtons2() {
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

  if (isBetween(value, 450, 570)) {
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

bool isBetween(int value, int min, int max) {
  return value >= min && value <= max;
}
// ----------- BUTTON END -----------

/*
  // =========== PIR START =============
  Mode readPirSensor(Mode previousMode, Mode nextMode) {
    return nextMode;
  }
  // ----------- PIR END -----------


  // =========== TIME START =============
  Mode readRealTimeClock(Mode previousMode, Mode nextMode) {
    return nextMode;
  }
  // ----------- TIME END -----------
*/

// =========== LED START =============
long _ledTimestamp = 0;
void updateLeds(Mode previousMode, Mode nextMode) {
  long currentTime;
  switch (nextMode) {
    case Mode_Off:
      setLedPinState(LedPinState_OffOff, LedPinState_OffOff);
      break;
    case Mode_Standby:
      setLedPinState(LedPinState_OnOff, LedPinState_OffOff);
      break;
    case Mode_Tv:
      setLedPinState(LedPinState_OffOff, LedPinState_OnOff);
      break;
    case Mode_Music_Chromecast:
      setLedPinState(LedPinState_OffOn, LedPinState_OffOff);
      break;
    case Mode_Music_InternetRadioPreset1:
      currentTime = millis();
      if (previousMode != Mode_Music_InternetRadioPreset1 || currentTime > _ledTimestamp + 3000) {
        _ledTimestamp = currentTime;
        setLedPinState(LedPinState_OffOn, LedPinState_OffOff);
      } else if (isBetween(currentTime, _ledTimestamp + 2500, _ledTimestamp + 2750))
        setLedPinState(LedPinState_OffOff, LedPinState_OffOff);
      break;
    case Mode_Music_InternetRadioPreset2:
      currentTime = millis();
      if (previousMode != Mode_Music_InternetRadioPreset2 || currentTime > _ledTimestamp + 3250) {
        _ledTimestamp = currentTime;
        setLedPinState(LedPinState_OffOn, LedPinState_OffOff);
      } else if (isBetween(currentTime, _ledTimestamp + 2500, _ledTimestamp + 2750))
        setLedPinState(LedPinState_OffOff, LedPinState_OffOff);
      else if (isBetween(currentTime, _ledTimestamp + 2750, _ledTimestamp + 3000))
        setLedPinState(LedPinState_OffOn, LedPinState_OffOff);
      else if (isBetween(currentTime, _ledTimestamp + 3000, _ledTimestamp + 3250))
        setLedPinState(LedPinState_OffOff, LedPinState_OffOff);
      break;
    case Mode_Music_InternetRadioPreset3:
      currentTime = millis();
      if (previousMode != Mode_Music_InternetRadioPreset3 || currentTime > _ledTimestamp + 3750) {
        _ledTimestamp = currentTime;
        setLedPinState(LedPinState_OffOn, LedPinState_OffOff);
      } else if (isBetween(currentTime, _ledTimestamp + 2500, _ledTimestamp + 2750))
        setLedPinState(LedPinState_OffOff, LedPinState_OffOff);
      else if (isBetween(currentTime, _ledTimestamp + 2750, _ledTimestamp + 3000))
        setLedPinState(LedPinState_OffOn, LedPinState_OffOff);
      else if (isBetween(currentTime, _ledTimestamp + 3000, _ledTimestamp + 3250))
        setLedPinState(LedPinState_OffOff, LedPinState_OffOff);
      else if (isBetween(currentTime, _ledTimestamp + 3250, _ledTimestamp + 3500))
        setLedPinState(LedPinState_OffOn, LedPinState_OffOff);
      else if (isBetween(currentTime, _ledTimestamp + 3500, _ledTimestamp + 3750))
        setLedPinState(LedPinState_OffOff, LedPinState_OffOff);
      break;
    case Mode_Projector:
      setLedPinState(LedPinState_OffOff, LedPinState_OffOn);
      break;
    case Mode_Custom:
      if (_ledTimestamp == 0)
        setLedPinState(LedPinState_OnOff, LedPinState_OnOff);
      else
        setLedPinState(LedPinState_OffOn, LedPinState_OffOn);
      _ledTimestamp = _ledTimestamp == 0 ? 1 : 0;
      break;
  }
}

LedPinState _previousStandbyMusicLedPinState, _previousTvProjectorLedPinState;
void setLedPinState(LedPinState standbyMusicLedPinState, LedPinState tvProjectorLedPinState) {
  _previousStandbyMusicLedPinState = setLedPinState2(standbyMusicLedPin, _previousStandbyMusicLedPinState, standbyMusicLedPinState);
  _previousTvProjectorLedPinState = setLedPinState2(tvProjectorLedPin, _previousTvProjectorLedPinState, tvProjectorLedPinState);
}

LedPinState setLedPinState2(uint8_t pin, LedPinState previousState, LedPinState newState) {
  if (previousState == newState)
    return newState;
#ifdef debug
  Serial.print("pin "); Serial.println(pin);
  Serial.print("newState "); Serial.println(LedPinState_Names[newState]);
#endif
  switch (newState) {
    case LedPinState_OffOff:
      pinMode(pin, INPUT);
#ifdef debug
      Serial.println("mode INPUT");
#endif
      break;
    case LedPinState_OffOn:
      if (previousState == LedPinState_OffOff)
        pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
#ifdef debug
      Serial.println("mode OUTPUT");
      Serial.println("value HIGH");
#endif
      break;
    case LedPinState_OnOff:
      if (previousState == LedPinState_OffOff)
        pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
#ifdef debug
      Serial.println("mode OUTPUT");
      Serial.println("value LOW");
#endif
      break;
  }
  return newState;
}
// ----------- LED END -----------

/*
  // =========== RELAY START =============
  void updateRelays(Mode previousMode, Mode nextMode)
  {
  }
  // ----------- RELAY END -----------
*/

// =========== AMP START =============
void updateAmp(Mode previousMode, Mode nextMode) {
  if (previousMode != nextMode) {
    if (nextMode == Mode_Standby || nextMode == Mode_Off) {
#ifdef debug
      Serial.println("Amp Power off");
#endif
      irTransmitter.send(DenonAvr1912, 0x8C008E, 0);
    } else {
#ifdef debug
      Serial.println("Amp Power on");
#endif
      irTransmitter.send(DenonAvr1912, 0x840086, 0);
    }
  }
  // Forstærker (DenonAvr1912)
  // power on 0x840086
  // power off 0x8C008E
  // bd 0x88B43E
  // tv 0x84B432
  // game1 0x8AB43C
  // game2 0x8EB438
  // mute 0x84E86E
  // vol up 0x80E86A
  // vol down 0x88E862
  // internet radio 0x8A74FC
  // pre1 0x84F076
  // pre2 0x8CF07E
  // pre3 0x82F070
}
// ----------- AMP END -----------

// =========== TV START =============
void updateTv(Mode previousMode, Mode nextMode) {
  if (previousMode != Mode_Tv && nextMode == Mode_Tv) {
#ifdef debug
    Serial.println("TV Power");
#endif
    irTransmitter.send(RC6, 0x10, 20);
  } else if (previousMode == Mode_Tv && nextMode != Mode_Tv) {
#ifdef debug
    Serial.println("TV Power");
#endif
    irTransmitter.send(RC6, 0x10, 20);
  }
}
// ----------- TV END -----------

// =========== PROJECTOR START =============
void updateProjector(Mode previousMode, Mode nextMode) {
  // BenQ W1070 (NEC)
  // Power 0xC40BF
  // Mute 0xC28D7
  if (previousMode != Mode_Projector && nextMode == Mode_Projector) {
#ifdef debug
    Serial.println("Projector Power");
#endif
    irTransmitter.send(NEC, 0xC40BF, 0);
  } else if (previousMode == Mode_Projector && nextMode != Mode_Projector) {
#ifdef debug
    Serial.println("Projector Power off");
#endif
    irTransmitter.send(NEC, 0xC40BF, 0);
    delay(1000);
    irTransmitter.send(NEC, 0xC40BF, 0);
  }
}
// ----------- PROJECTOR END -----------

// =========== PROJECTOR SCREEN START =============
void updateProjectorScreen(Mode previousMode, Mode nextMode)
{
  if(previousMode != Mode_Projector && nextMode == Mode_Projector)
  {
    // TODO
  }
}
// ----------- PROJECTOR SCREEN END -----------






