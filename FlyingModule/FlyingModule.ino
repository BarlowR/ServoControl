#include <VolzLib.h>
#include <GraupnerESCLib.h>
#include "SBUS.h"


                                //BUS MAPPING
#define AIL_L   0x00
#define FLAP_L  0x01
#define ESC     0x02
#define FLAP_R  0x03
#define AIL_R   0x04
#define TAIL_R  0x05
#define TAIL_L  0x06

#define BUS     Serial1         //RS485 Hardware serial port

#define WRITE_INTERVAL 10000    //Interval at which commands are written to the servos and ESC & timer
IntervalTimer busWrite;


VolzServo servos;               //Servo Library
GraupnerESC graupnerESC;        //ESC Library


SBUS RXSR(Serial2);             //Rx   
uint16_t channels[16];
bool failSafe;
bool lostFrame;

bool usb_connected = false;     //USB connection state

String serialMessage = "";      //String for sending serial messages

void setup() {

  //initialize
  Serial.begin(9600);           
  RXSR.begin();
  busWrite.begin(writeBus, WRITE_INTERVAL);
  pinMode(LED_BUILTIN, OUTPUT);

  //check for usb serial connection (sends a message and waits for a response). CRITICAL: COMMENT OUT BEFORE FLIGHT
  //SerialConnectionCheck();                   
}

void loop() {

  //check if there's a new SBUS packet available & if so write it to the channels array
  if(RXSR.read(&channels[0], &failSafe, &lostFrame)){}
}


void writeBus(){

  int pos = 0;                  //placeholder for PWM command calculations
  
  if (!failSafe){               //check for rx failsafe

    //shift/scale rx data to fit ESC command (1500-2000)
    pos = (channels[ESC] - 1000) * 50/80; 
    if (pos > 0) pos += 1500;
    else pos = 1500;

    // start bus at ESC baud rate
    BUS.begin(19200);

    //write PWM command
    graupnerESC.setPWM(ESC, pos);

    //add ESC data to serial string
    if (usb_connected){
      serialMessage.concat(ESC);
      serialMessage.concat(": ");
      serialMessage.concat(pos);
      serialMessage.concat(" ");
    }

    //stop bus and restart at servo baud rate
    BUS.end();
    BUS.begin(115200);


    //for all servos except for the ESC, write from the channels array
    for (byte i = 0; i<=0x06; i++){
      if (i != 0x02){
          pos = channels[i] -1000;
          servos.setSPos(i, pos);
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

    //print serial string
    if (usb_connected){
      Serial.println(serialMessage);
      serialMessage = "";
    }

  }

  //if in failsafe mode, turn off motor & print FAILSAFE to serial
  else{
    BUS.begin(19200);
    graupnerESC.setPWM(ESC, 1500);
    BUS.end();
    if (usb_connected) Serial.println("FAILSAFE");
  }
}



bool SerialConnectionCheck(){         //send message to USB serial port and wait for timeout or response. Blink LED to show status (blinking -> waiting, on -> no connection, off -> connection) 
  unsigned long timeout = millis();
  int poll = 0;
  Serial.println("Connected?");
  while(!Serial.available() && millis()-timeout < 5000){
    if (millis() % 500 < 250) digitalWrite(LED_BUILTIN, HIGH);
    else digitalWrite(LED_BUILTIN, LOW);
    if (poll > 100000){
      poll = 0;
      Serial.println("Connected?");
    }
    else poll ++;
  }
  if (Serial.available()){
    usb_connected = true;
    digitalWrite(LED_BUILTIN, LOW);
  }
  else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
