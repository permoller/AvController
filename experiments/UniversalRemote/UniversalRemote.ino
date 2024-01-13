#include <ProjectorScreenRf.h>
#include <SimpleIr.h>
#include <DenonAvr1912Ir.h>
#include <BenqW1070Ir.h>

#define Btn1 2
#define Btn2 3
#define Btn3 4

SimpleIr ir(A1);
DenonAvr1912Ir receiver(ir);
BenqW1070Ir projector(ir);
ProjectorScreenRf screen(A0);

void setup() {
  // put your setup code here, to run once:
  pinMode(Btn1, INPUT_PULLUP);
  pinMode(Btn2, INPUT_PULLUP);
  pinMode(Btn3, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(Btn1) == LOW)
  {
    //receiver.powerOn();
    //receiver.sourceSelectTv();
    //projector.power();
    screen.down();
  delay(1000);
  }
  if(digitalRead(Btn2) == LOW)
  {
    //receiver.powerOff();
    //receiver.sourceSelectGame1();
    //projector.mute();
    screen.up();
  delay(1000);
  }
  if(digitalRead(Btn3) == LOW)
  {
    //receiver.sourceSelectBd();
    //receiver.sourceSelectGame2();
  //delay(2000);
  }
}
