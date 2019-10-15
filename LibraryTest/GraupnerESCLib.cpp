#include "Arduino.h"
#include "GraupnerESCLib.h"

GraupnerESC::GraupnerESC(){
  //RS485bus.begin(19200);
}


void GraupnerESC::setPWM(byte esc, short pos) //set throttle value. Valid range is 1000 -> 2000.
{
  byte cmd[4] = {GRAUPNER_SET_PWM, esc, lowByte(pos), highByte(pos)};
  sendCmd(cmd, 4);
}



void GraupnerESC::setAd(byte escID) //set ESC address
{
  byte cmd[4] = {GRAUPNER_SET_ID, escID, lowByte(motor_off), highByte(motor_off)};
  sendCmd(cmd, 4);
}


void GraupnerESC::sendCmd(unsigned char * cmd, size_t cmd_length) // Send a command to the RS485 bus.
{ 
  //Assemble and write command to serial bus
  uint8_t csum = checksum(cmd, cmd_length);
   
  byte cmdcrc[cmd_length+1];
  for (int i = 0; i < cmd_length; i++)
  {
    cmdcrc[i] = cmd[i]; 
     // usbSerial.print(cmd[i], HEX);
     // usbSerial.print(" ");

  }
  //usbSerial.println(csum, HEX);

  cmdcrc[cmd_length] = csum; 
  RS485bus.write(cmdcrc, cmd_length+1); //write command to serial bus
}




uint8_t GraupnerESC::checksum(unsigned char * cmd, size_t cmd_length) // checksum method
{
  uint8_t checksum = 0x00;
  for (int i = 0; i < cmd_length; i++)
  {
      checksum += cmd[i];
  }
  return checksum;
  
}


/* for reference: the checksum method from graupner_esc.c
    uint8_t checksum;
    checksum = 0;
    for (int i = 0; i < tx_size; i++)
    {
        checksum += frame[i];
    }
    frame[tx_size] = checksum;

 */



bool GraupnerESC::checkConn(byte id)
{
  byte braking = ((300000) / 32) >> 6;

  byte cmd[5]= {0x50, id, lowByte(motor_off), highByte(motor_off)};
  sendCmd(cmd, 4);

  byte rd = 0x00;
  unsigned long ts = millis(); 
  while (rd != 0x51 && (millis() - ts < 200)) 
  {
    if (RS485bus.available())
    {
      rd = RS485bus.read();
      usbSerial.println(rd, HEX);
    }
  } 
  if (rd == 0x00){
    errorReport("No Response from ESC");
    return false;
  }
  if (rd == 0x51){
    alert("connected");
  }
  return true;
}



void GraupnerESC::errorReport(String code) // Throw given error
{ 
  usbSerial.print("ERROR: ");
  usbSerial.println(code);
}


void GraupnerESC::alert(String code) // Throw given alert
{
  usbSerial.print("Alert: ");
  usbSerial.println(code);
}
