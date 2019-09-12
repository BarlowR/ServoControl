#ifndef VolzLib_h
#define VolzLib_h
#define servoBus Serial1
#define usbSerial Serial

#include "Arduino.h"

class VolzServo
{
  public:
    VolzServo();
    void setPos(byte servo, short pos); //set absolute servo position. Valid range is +- 1934.
    byte getAd(); // Get the address of the servo currently attached to the RS485 bus. Only attach one servo at a time. Returns the servo address.
    bool setAd(byte curAd, byte setAd); // Set the address of the servo at the given address. Returns true if successful. Use getAd to find the address.
    void sendCmd(unsigned char * ans, unsigned char * cmd, short timeout = 10); // Send a command to the RS485 bus. Returns the response via ans pointer.
    short return_CRC(unsigned char cmd[4]); // Volz checksum method
    void errorReport(String code); // Throw given error
    void alert(String code); // Throw given alert
};

#endif
