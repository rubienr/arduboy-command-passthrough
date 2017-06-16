#include "stringLineFifo.h"
#include "globals.h"

extern Globals g;

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
    struct tmp {
      uint8_t value: 3;
    } tmp;
    tmp.value = startIdx;
                g.log.print("increment rx buffer read idx %d -> %d\n", startIdx, ++tmp.value);
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
    struct tmp {
      uint8_t value: 3;
    } tmp;
    tmp.value = endIdx;
                g.log.print("increment rx buffer write idx %d -> %d\n", endIdx, ++tmp.value);
    );
    ++endIdx;
  }
  buffers[endIdx] = "";
}

