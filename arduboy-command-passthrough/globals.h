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
typedef class Globals {
  private:

  protected:

  public:
    Arduboy2 arduboy;
    SoftwareSerial serial;
    Command command;
    DebugLevel debugLevel;
    TickerSymbol ticker;

    Globals() :
      arduboy(),
      serial(10, 11),
      command(),
      debugLevel(DEBUG_OFF),
      //debugLevel(DEBUG_ON),
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

