#include "Arduino.h"
#include "VolzLib.h"

VolzServo::VolzServo(){
  //RS485bus.begin(115200);
}


void VolzServo::setPos(byte servo, short pos) //set absolute servo position. Valid range is +- 1934.
{
  byte cmd[4] = {0x76, servo, highByte(pos), lowByte(pos)};
  byte ans[6] = {0};
  sendCmd(ans, cmd, 0x56, 0);
}

void VolzServo::setSPos(byte servo, short pos) //set absolute servo position. Valid range is +- 1934.
{
  byte cmd[4] = {0x77, servo, highByte(pos), lowByte(pos)};
  byte ans[6];
  sendCmd(ans, cmd, 0x00, 0);
}

byte VolzServo::getAd() // Get the address of the servo currently attached to the RS485 bus. Only attach one servo at a time. Returns the servo address.
{ 
  byte cmd[4] = {0xDA, 0x1F, 0x00, 0x00};
  byte ans[6];
  sendCmd(ans, cmd, 0x00, 0);

  delay(10);
  byte i = 0;
  while(RS485bus.available()){
    byte rsp = RS485bus.read();
    ans[i] = rsp; 
    if (rsp == 255 || rsp == 0) {}
    else i++;
  }
  if (ans[0] == 0x6D) return ans[2];
  else errorReport("Improper response to get addresss");
}


bool VolzServo::setAd(byte curAd, byte setAd) // Set the address of the servo at the given address. Returns true if successful. Use getAd to find the address.
{ 
  byte cmd[4] = {0xAA, curAd, setAd, setAd};
  byte ans[6];
  sendCmd(ans, cmd, 0x00, 0);
  if (ans != 0)
  {
    if (ans[0] == 0x55 && ans[2] == setAd) return true;
    else errorReport("Improper response to set address");
  }
  else
  {
    errorReport("null response");
  }
  return false;
}


void VolzServo::sendCmd(unsigned char * ans, unsigned char * cmd, byte returnCode, short timeout = 6) // Send a command to the RS485 bus. Returns the response via ans pointer.
{ 
  byte buff[32] = {};
  ans[0] = 0;
  //Assemble and write command to serial bus
  short int crc = return_CRC(cmd); 
  byte lb = lowByte(crc), hb = highByte(crc); 
  byte cmdcrc[6] = {cmd[0], cmd[1], cmd[2], cmd[3], hb, lb}; 
  RS485bus.write(cmdcrc, 6); //write command to serial bus
  RS485bus.flush();
}



short int VolzServo::return_CRC(unsigned char cmd[4]) // Volz checksum method
{
  unsigned short int crc = 0; // Result

  char x, y;
  crc = 0xFFFF; //init value

  for (x = 0; x < 4; x++)
  {
    crc = ( ( cmd[x] << 8 ) ^ crc);
    for (y = 0; y < 8; y++ )
    {
      if ( crc & 0x8000 ) {
        crc = (crc << 1) ^ 0x8005;
      }
      else
      {
        crc = crc << 1;
      }
    }
  }
  return crc;
}


void VolzServo::errorReport(String code) // Throw given error
{ 
  usbSerial.print("ERROR: ");
  usbSerial.println(code);
}


void VolzServo::alert(String code) // Throw given alert
{
  usbSerial.print("Alert: ");
  usbSerial.println(code);
}
