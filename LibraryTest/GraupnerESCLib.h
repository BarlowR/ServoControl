#ifndef GraupnerESCLib_h
#define GraupnerESCLib_h
#define RS485bus Serial1
#define usbSerial Serial

#define GRAUPNER_SET_PWM         (0x00) // Set ESC speed. Silent (no response from controller)
#define GRAUPNER_SET_ID          (0x10) // Set bus ID. Silent (no response from controller)
#define GRAUPNER_GET_TELEMETRY   (0x50) // Fetch ESC status (and set ESC speed).

#include "Arduino.h"

class GraupnerESC
{
  public:
    GraupnerESC();
    void setPWM(byte esc, short pos); //set absolute servo position. Valid range is 1000-2000.
    void setAd(byte escID); //set bus ID
    void sendCmd(unsigned char * cmd, size_t cmd_length); // Send a command to the RS485 bus. Returns the response via ans pointer.
    uint8_t checksum(unsigned char * cmd, size_t cmd_length); // checksum method
    void errorReport(String code); // Throw given error
    void alert(String code); // Throw given alert
    bool checkConn(byte id); //Poll ESC and check for a response
    short motor_off = 1500;

};

#endif
