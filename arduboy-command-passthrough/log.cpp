#include "log.h"
#include <SoftwareSerial.h>

//----------------------------------------------------------------------------------------------------
SerialLogger::SerialLogger()
{
  buffer[0] = 0;
}

//----------------------------------------------------------------------------------------------------
void SerialLogger::print(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message, args);
    va_end(args);
    Serial.write(buffer);
}


