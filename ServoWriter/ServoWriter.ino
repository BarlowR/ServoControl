#include <VolzLib.h>

VolzServo servoBus;


byte servoAddress = 0x00;
int toWrite = 0;


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
    Serial.begin(9600);

    delay(500);

  
}

void loop() {
  servoAddress = servoBus.getAd();
  Serial.print("Current Address: ");
  Serial.println(servoAddress, HEX);
  Serial.println("Servo address to write?");
  while(!Serial.available())
  toWrite = Serial.parseInt();
  Serial.print("Writing: ");
  Serial.println(toWrite, HEX);
  if (servoBus.setAd(servoAddress, toWrite)){
    Serial.println("Write Complete.");
  }
  while(Serial.available())Serial.read();
}
