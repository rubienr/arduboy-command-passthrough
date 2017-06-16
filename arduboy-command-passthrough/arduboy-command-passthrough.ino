/*
  June , 2017
  Version 0.1
  Copyright (C) 2015 Raoul Rubien
  All rights reserved.

  This implementation reads from serial, interprets the input and executes the mapped command with the given arguments so that the Arduboy
  can be controlled via serial connection.


  Example commands:
  Connect to the arduboy and send
  debug %i 0
  arduboy.clear
  arduboy.setCursor %i %i 10 10
  arduboy.print %s Hello World!
  arduboy.display


  Command syntax:
  <command name> <arguments modifier 1 ... n> <arguments 1 ... n>

  To set the cursor at the position x=10, y=10  on the screen:
  arduboy.setCursor %i %i 10 10


  Caveat!
  The implementation does not verify the modifier yet, which sould map as follows:
  %s string
  %c char
  %i int
  %u unsigned int

*/


#include "globals.h"
#include "interpreter.h"

typedef CommandInterpreter Interpreter;
Globals g;

//----------------------------------------------------------------------------------------------------
void readInput()
{
  while (Serial.available())
  {
    char inByte = Serial.read();
    Serial.write(inByte);
    if (inByte != '\n')
    {
      g.serialRxLineBuffer.currentWriteBuffer() += inByte;
    } else {
      g.serialRxLineBuffer.nextWriteBuffer();
    }
  }
}

//----------------------------------------------------------------------------------------------------
void dispatchInput()
{
  if (g.serialRxLineBuffer.isEmpty()) 
  {
    return;
  }

  ifDebug(g.log.print("interpret buffer -%s-\n", g.serialRxLineBuffer.currentReadBuffer().c_str()));
  
  String &input = g.serialRxLineBuffer.currentReadBuffer();

  int endIdx = input.indexOf(INTERPRETER_WORD_DELIMITER);
  String command = input.substring(0, endIdx);
  input.remove(0, command.length() + 1);
  String arguments = input;

  if (Interpreter::parseCommand(command, arguments))
    Interpreter::interpretCommand(g.command);

  g.serialRxLineBuffer.nextReadBuffer();
}

//----------------------------------------------------------------------------------------------------
void setup() {
  g.init();
}

//----------------------------------------------------------------------------------------------------
void loop() {
  //if (!(g.arduboy.nextFrame()))
  // return;
  readInput();
  dispatchInput();
}


