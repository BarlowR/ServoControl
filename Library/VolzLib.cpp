#include "Arduino.h"
#include "VolzLib.h"

VolzServo::VolzServo(){}


void VolzServo::setPos(byte servo, short pos) //set absolute servo position. Valid range is +- 1934.
{
  byte cmd[4] = {0x76, servo, highByte(pos), lowByte(pos)};
  byte * ans = sendCmd(cmd);
  if (ans != 0)
  {
    if (ans[0] == 0x6D) return ans[2];
    else errorReport("Improper response to set position");
  }
  else
  {
    errorReport("null response");
  }
}


byte VolzServo::getAd() // Get the address of the servo currently attached to the RS485 bus. Only attach one servo at a time. Returns the servo address.
{ 
  byte cmd[4] = {0xDA, 0x1F, 0x00, 0x00};
  byte * ans = sendCmd(cmd);
  if (ans != 0)
  {
    if (ans[0] == 0x6D) return ans[2];
    else errorReport("Improper response to get addresss");
  }
  else
  {
    errorReport("null response");
  }
}


bool VolzServo::setAd(byte curAd, byte setAd) // Set the address of the servo at the given address. Returns true if successful. Use getAd to find the address.
{ 
  byte cmd[4] = {0xAA, curAd, setAd, setAd};
  byte * ans = sendCmd(cmd);
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


unsigned char * VolzServo::sendCmd(unsigned char * ans, unsigned char * cmd, short timeout = 10) // Send a command to the RS485 bus. Returns the response via ans pointer.
{ 
  //Assemble and write command to serial bus
  short int crc = return_CRC(cmd); 
  byte lb = lowByte(crc), hb = highByte(crc); 
  byte cmdcrc[6] = {cmd[0], cmd[1], cmd[2], cmd[3], hb, lb}; 
  servoBus.write(cmdcrc, 6); //write command to serial bus
 
  // Wait for response and alert if timeout
  unsigned long ts = millis(); 
  while (!servoBus.available() && (millis() - ts < timeout)) {} 
  if (millis() - ts >= timeout) alert("Response Timeout"); 
  
  //Return response, throw error if no response
  byte i = 0;
  while (servoBus.available() && i < 6) {
    rsp[i] = servoBus.read();
    i++;
  }
  if (i > 0) return ans;
  else
  {
    String erMsg = "Did not receive response from cmd: "
                   for (byte j = 0 ; j < 6; j++)
    {
      erMsg.concat(cmdcrc[j));
      erMsg.concat(" ");
    }
    errorReport(erMsg);
  }
  return 0;
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
