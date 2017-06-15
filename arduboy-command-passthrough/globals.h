#pragma once

//----------------------------------------------------------------------------------------------------
typedef enum DebugLevel { DEBUG_OFF, DEBUG_ON } DebugLevel;


//----------------------------------------------------------------------------------------------------
typedef class TickerSymbol {
  private:

  protected:

  public:
    const char ticker[4];
    uint8_t tickCounter : 2;

    TickerSymbol() :
      ticker{'|', '\\', '-', '/'},
      tickCounter(0)
    {}

} TickerSymbol;


//----------------------------------------------------------------------------------------------------
typedef class StringLineFifo {
  private:

    String  buffers[8];
    uint8_t startIdx   : 3;
    uint8_t endIdx     : 3;

  protected:

  public:

    StringLineFifo():
      buffers {"", "", "", "", "", "", "", ""},
      startIdx(0),
      endIdx(1)
    {}

    bool isEmpty()
    {
      return startIdx == endIdx;
    }

    bool isFull() {
      if (++endIdx == startIdx) {
        --endIdx;
        return true;
      }
      --endIdx;
      return false;
    }

    String &currentReadBuffer()
    {
      return buffers[startIdx];
    }

    void nextReadBuffer()
    {
      if (!isEmpty())
      {
        ++startIdx;
      }
    }


    String &currentWriteBuffer()
    {
      return buffers[endIdx];
    }

    void nextWriteBuffer()
    {
      if (!isFull())
      {
        ++endIdx;
      }
      buffers[endIdx] = "";
    }

    void clear()
    {
      startIdx = 0;
      endIdx = 0;
    }

} StringLineBuffer ;

//----------------------------------------------------------------------------------------------------
typedef class Globals {
  private:

  protected:

  public:
    Arduboy2 arduboy;
    SoftwareSerial serial;
    Command command;
    DebugLevel debugLevel;
    StringLineFifo serialRxLineBuffer;
    TickerSymbol ticker;

    Globals() :
      arduboy(),
      serial(10, 11),
      command(),
      debugLevel(DEBUG_OFF),
      //debugLevel(DEBUG_ON),
      serialRxLineBuffer(),
      ticker()
    {
    }

    void init() {
      arduboy.begin();
      arduboy.setFrameRate(5);
      serial.begin(9600);
      arduboy.setCursor(4, 9);
    }

} Globals;

