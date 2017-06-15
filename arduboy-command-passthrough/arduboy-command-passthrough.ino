/*
  June , 2017
  Version 0.1
  Copyright (C) 2015 Raoul Rubien
  All rights reserved.

  This implementation reads from serial, parses the input and executes mapped commands with the given arguments so that the Arduboy
  can be controlled via serial connection.


  Example commands:
  Connect to the arduboy and send
  debug 0
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

#include <Arduboy2.h>
#include <SoftwareSerial.h>
#include "interpreter.h"
#include "globals.h"

typedef CommandInterpreter Interpreter;
Globals g;





//----------------------------------------------------------------------------------------------------
void setup() {
  g.init();
}

//----------------------------------------------------------------------------------------------------
void dispatchInput(String &input);
void readInput(String &inBuffer);

//----------------------------------------------------------------------------------------------------
void loop() {
  if (!(g.arduboy.nextFrame()))
    return;
    
  String inBuffer;
  readInput(inBuffer);
  dispatchInput(inBuffer);
}

//----------------------------------------------------------------------------------------------------
void dispatchInput(String &input)
{
  int endIdx = input.indexOf(INTERPRETER_WORD_DELIMITER);
  String command = input.substring(0., endIdx);
  input.remove(0, command.length() + 1);
  String arguments = input;

  if ( 0 == Interpreter::parseCommand(command, arguments)) {
    Interpreter::interpretCommand(g.command);
  }
}

//----------------------------------------------------------------------------------------------------
void readInput(String& inBuffer) {
  while (Serial.available()) {

    char inByte = Serial.read();
    if (inByte != '\n')
    {
      inBuffer += inByte;
    }
    Serial.write(inByte);
  }

  inBuffer.trim();
}


