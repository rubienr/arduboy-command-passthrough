#pragma once
#include <Arduino.h>

//----------------------------------------------------------------------------------------------------
typedef class SerialLogger {
  
  private:
  
    char buffer[64];
    
  protected:
    
  public:

    SerialLogger();
    void print(const char *message, ...);

} SerialLogger;

