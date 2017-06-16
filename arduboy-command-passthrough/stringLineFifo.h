#pragma once

#include <Arduino.h>

//----------------------------------------------------------------------------------------------------
typedef class StringLineFifo {
  private:

    String  buffers[8];
    uint8_t startIdx   : 3;
    uint8_t endIdx     : 3;

  protected:

  public:

    StringLineFifo();
    bool isEmpty();
    bool isFull();
    String &currentReadBuffer();
    void nextReadBuffer();
    String &currentWriteBuffer();
    void nextWriteBuffer();

} StringLineFifo;
