#include "VolzLib.h"

VolzServo servoBs;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  int pos1 = (((millis()/2)%3600)-1800);
  int pos2 = ((millis())%3600)-1800;
  unsigned long maxDelay = micros();
  servoBs.setSPos(2, -1900);
  servoBs.setSPos(1, -1900);
  servoBs.setSPos(2, -1900);
  servoBs.setSPos(1, -1900);
  servoBs.setSPos(2, -1900);
  servoBs.setSPos(1, -1900);
  Serial.println(micros()-maxDelay);
}
