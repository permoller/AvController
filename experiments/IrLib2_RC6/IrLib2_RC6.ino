#include <IRLibRecvPCI.h>
#include <IRLibDecodeBase.h>
#include <IRLib_P04_rc6.h>
#include <IRLibCombo.h>

IRrecvPCI myReceiver(2);
IRdecode myDecoder;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}

void loop() {
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(true);  //Now print results. Use false for less detail
    myReceiver.enableIRIn();      //Restart receiver
  }
}
