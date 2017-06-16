#include "stringLineFifo.h"
#include "globals.h"

extern Globals g;

typedef union Bitfields {
  uint8_t bits1 : 1;
  uint8_t bits2 : 2;
  uint8_t bits3 : 3;
  uint8_t bits4 : 4;
  uint8_t bits5 : 5;
  uint8_t bits6 : 6;
  uint8_t bits7 : 7;
  uint8_t bits8 : 8;
} Bitfields;


typedef struct LineBufferCounter {
  uint8_t idx : 4;
} LineBufferCounter;



//----------------------------------------------------------------------------------------------------
StringLineFifo::StringLineFifo():
  buffers {"", "", "", "", "", "", "", ""},
  startIdx(0),
  endIdx(1)
{}

//----------------------------------------------------------------------------------------------------
bool StringLineFifo::isEmpty()
{
  return startIdx == endIdx;
}

//----------------------------------------------------------------------------------------------------
bool StringLineFifo::isFull() {
  if (++endIdx == startIdx) {
    --endIdx;
    return true;
  }
  --endIdx;
  return false;
}

//----------------------------------------------------------------------------------------------------
String &StringLineFifo::currentReadBuffer()
{
  return buffers[startIdx];
}

//----------------------------------------------------------------------------------------------------
void StringLineFifo::nextReadBuffer()
{
  if (!isEmpty())
  {
    ifDebug(
      LineBufferCounter tmp{.idx = startIdx};
      g.log.print("increment rx buffer read idx %d -> %d\n", startIdx, ++tmp.idx);
    );

    ++startIdx;
  }
}

//----------------------------------------------------------------------------------------------------
String &StringLineFifo::currentWriteBuffer()
{
  return buffers[endIdx];
}

//----------------------------------------------------------------------------------------------------
void StringLineFifo::nextWriteBuffer()
{
  if (!isFull())
  {

    ifDebug(
      LineBufferCounter tmp{.idx = endIdx};
      g.log.print("increment rx buffer write idx %d -> %d\n", endIdx, ++tmp.idx);
    );

    ++endIdx;

    ifDebug(
    
    for (LineBufferCounter tmp{.idx = startIdx}; tmp.idx != endIdx; ++tmp.idx) {
    g.log.print("idx: %d line: %s\n", tmp.idx, buffers[tmp.idx].c_str());
    }
    );
  }
  buffers[endIdx] = "";
}

