#include <IRLibRecvPCI.h>
#include <IRLibDecodeBase.h>
#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLib_P04_rc6.h>
#include <IRLib_P13_DenonAvr1912.h>
#include <IRLibCombo.h>

IRsend mySender;
IRrecvPCI myReceiver(2);
IRdecode myDecoder;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}
int x =0;
bool t;
void loop() {
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(true);  //Now print results. Use false for less detail
    if(myDecoder.protocolNum == RC6)
    {
      // mask toggle bit
      uint32_t value = myDecoder.value & 0xfffeffff;
      
      // vol up
      if(value == 0x10)
      {
        mySender.send(DenonAvr1912, 0x80E86A, 24);
      }
      // vol down
      if(value == 0x11)
      {
        mySender.send(DenonAvr1912, 0x88E862, 24);
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

      // TV (RC6)
      // Power 0xC
      // tv 0x9F
      // Vol up 0x10
      // Vol down 0x11
      // mute 0xD
      // netflix 0x76
      // ch up 0x21
      // ch down 0x20
      // arrow up 0x58
      // arrow down 0x59
      // arrow left 0x5A
      // arrow right 0x5B
      // ok 0x5C

      // BenQ W1070 (NEC)
      // Power 0xC40BF
      // Mute 0xC28D7
    }
    myReceiver.enableIRIn();      //Restart receiver
  }
/*
  x += 1;
  t = !t;
  Serial.println(x);
  //for(int i = 0;i<1;i++)
  //{
    if(t)
      mySender.send(RC6, 0x10010, 20);
    else
      mySender.send(RC6, 0x10, 20);
    //delayMicroseconds(x);
  //}
  delay(2000);*/
}
