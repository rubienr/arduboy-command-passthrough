#pragma once

#include <Arduino.h>

//----------------------------------------------------------------------------------------------------
typedef class StringLineFifo {
  private:

    String  buffers[16];
    uint8_t startIdx   : 4;
    uint8_t endIdx     : 4;

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
