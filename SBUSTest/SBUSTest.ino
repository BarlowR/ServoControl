#include <VolzLib.h>
#include "SBUS.h"

SBUS RXSR(Serial2);

VolzServo servoBs;


uint16_t channels[16];
bool failSafe;
bool lostFrame;

void setup() {
  // put your setup code here, to run once:
  RXSR.begin();
  Serial.begin(9600);

}

void loop() {
  String channelDisplay = "L9R Receiver :";

  if(RXSR.read(&channels[0], &failSafe, &lostFrame)){
    for(byte i = 0; i < 7; i++){
      channelDisplay.concat("ch ");
      channelDisplay.concat(i +1);
      channelDisplay.concat(": ");
      channelDisplay.concat(channels[i]);
      channelDisplay.concat(" ");
      servoBs.setSPos(i, (channels[i] - 1000) * 2);

    }
    Serial.println(channelDisplay);
    
    servoBs.setSPos(1, -1900);
    servoBs.setSPos(1, -1900);
  }
  delay(1);
}
