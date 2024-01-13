#include <ProjectorScreenRf.h>
#include <SimpleIr.h>
#include <DenonAvr1912Ir.h>
#include <BenqW1070Ir.h>

SimpleIr ir(A0);
DenonAvr1912Ir receiver(ir);
BenqW1070Ir projector(ir);
ProjectorScreenRf screen(A1);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
