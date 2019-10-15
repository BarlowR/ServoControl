#include "VolzLib.h"
#include "GraupnerESCLib.h"
VolzServo servoBus;
GraupnerESC graupnerESC;

unsigned long bgn;

void setup() {
  // put your setup code here, to run once:
  Serial1.end();
  Serial.begin(9600);
  

}

void loop() {
  bgn = millis();
  Serial1.begin(19200);
  graupnerESC.setPWM(0x02, 1520);
  Serial1.end();
  Serial1.begin(115200);
  servoBus.setSPos(0x05, millis()%1000);
  //Serial1.end();
  Serial.print(millis()%1000);
  Serial.print(" ");
  Serial.println(millis()-bgn);
  delay(10);
}
