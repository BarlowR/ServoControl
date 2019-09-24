#include "SBUS.h"

SBUS L9R(Serial2);

uint16_t channels[16];
bool failSafe;
bool lostFrame;

void setup() {
  // put your setup code here, to run once:
  L9R.begin();
}

void loop() {
  String channelDisplay = "L9R Receiver :";

  if(L9R.read(&channels[0], &failSafe, &lostFrame)){
    for(byte i = 0; i < 8; i++){
      channelDisplay.concat("ch ");
      channelDisplay.concat(i);
      channelDisplay.concat(": ");
      channelDisplay.concat(channels[i]);
      channelDisplay.concat(" ");
    }
  }
  Serial.println(channelDisplay);
  delay(1);
}
