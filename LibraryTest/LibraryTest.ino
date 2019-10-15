#include "VolzLib.h"
#include "GraupnerESCLib.h"
VolzServo servos;
GraupnerESC graupnerESC;

byte servoAddress = 0x00;
int toWrite = 0;

int pos = 0;

String serialMessage;

bool usb_connected = true;

#define BUS  Serial1


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
    Serial.begin(9600);

    delay(500);

  
}

void loop() {
  
    BUS.begin(19200);

    //write PWM command
    graupnerESC.setPWM(0x02, 1500);

    //add ESC data to serial string
    if (usb_connected){
      serialMessage.concat(0x02);
      serialMessage.concat(": ");
      serialMessage.concat(1500);
      serialMessage.concat(" ");
    }

    //stop bus and restart at servo baud rate
    BUS.end();
    
    BUS.begin(115200);


    //for all servos except for the ESC, write from the channels array
    for (byte i = 0; i<0x06; i++){
      if (i != 0x02){
          pos = millis() % 2000;
          servos.setPos(i, pos);
          if (usb_connected){
            serialMessage.concat(i);
            serialMessage.concat(": ");
            serialMessage.concat(pos);
            serialMessage.concat(" ");
          }
      }
    }
  
    //stop bus
    BUS.end();

    //Serial.println(serialMessage);
    serialMessage = "";
    delay(10);
}
