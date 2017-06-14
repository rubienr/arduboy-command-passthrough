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

#define WORD_DELIMITER ' '
#define MODIFIER_PREFIX '%'

typedef class Command {
  String mName;
  bool mIsEmpty;
  
public:

  uint8_t numArguments;
  char modifiers[8];
  String values[8];

  void addArgument(char modifier, String &value) {
    modifiers[numArguments] = modifier;
    values[numArguments] = value;
    ++numArguments;
    mIsEmpty = false;
  }

  void setName(String &name) {
    reset();
    mName = name;
    mIsEmpty = false;
  }

  String &getName() {
    return mName;
  }
  
  bool isEmpty() {
    return mIsEmpty;
  }
  
  void reset() {
    numArguments = 0;
    mIsEmpty = true;
  }
  
  Command() : numArguments(0), mIsEmpty(false) {}
  
} Command;

typedef enum DebugLevel { DEBUG_OFF, DEBUG_ON };

typedef class Globals {
public:
  Arduboy2 arduboy;
  SoftwareSerial serial;
  Command command;
  DebugLevel debugLevel;
  
  
  const char ticker[4];
  uint8_t tickCounter : 2;

  Globals() :
  serial(10,11),
  ticker{'|','\\','-','/'},
  tickCounter(0),
  debugLevel(DEBUG_OFF)
  //debugLevel(DEBUG_ON)
  {
    
  }

  void init() {
    arduboy.begin();
    arduboy.setFrameRate(5);
    serial.begin(9600);  
    arduboy.setCursor(4, 9);
  }
  
} Globals;

Globals g;

void setup() {
  g.init();
}

void dispatchInput(String &input);
uint8_t parseCommand(String &input, String &arguments);
void interpretCommand(Command &command);
void readInput(String &inBuffer);

void loop() {
  if (!(g.arduboy.nextFrame()))
    return;
  String inBuffer;
  readInput(inBuffer);   
  dispatchInput(inBuffer);
}

void dispatchInput(String &input) 
{
  int endIdx = input.indexOf(WORD_DELIMITER);
  String command = input.substring(0.,endIdx);
  input.remove(0, command.length() + 1);
  String arguments = input;
  
  if ( 0 == parseCommand(command, arguments)) {
    interpretCommand(g.command);  
  }
}

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

/**
 * Parses the command and argumenta and stores it to a command object.
 * @param command the command name
 * @param arguments command arguments: %i %u %s -10 1 "string"
 */
uint8_t parseCommand(String &command, String &arguments) 
{
  if (command.length() <= 0)
    return -1;
    
  g.command.setName(command);
  int separatorIdx = arguments.lastIndexOf(MODIFIER_PREFIX) + 1;
  String modifiers = arguments.substring(0, separatorIdx + 1);
  modifiers.trim();
  String values = arguments.substring(separatorIdx + 1);
  values.trim();

  
  while (modifiers.length() > 1 && values.length() > 0) 
  {   
    int startIdx = modifiers.indexOf(MODIFIER_PREFIX);
    char modifier = modifiers.charAt(startIdx + 1);
    modifiers.remove(0, startIdx + 1);
        
    
    startIdx = values.indexOf(WORD_DELIMITER);
    String value;
    if ( startIdx >= 0) 
    {
      if (modifiers.length() <= 1)
        value = values;
      else
        value = values.substring(0, startIdx+1);
    }
    else 
    {
      value = values;
    }
    values.remove(0, startIdx + 1);
    value.trim();
    g.command.addArgument(modifier, value);  
  }

  return 0;
}

/**
 * Interprets and executes the command object.
 */
void interpretCommand(Command &command) {
  if (command.isEmpty()) 
    return;

  if (DEBUG_ON == g.debugLevel) {
    char buffer[30];  
    sprintf(buffer,"#: %d\n", command.numArguments);
    Serial.write(buffer);
    for (uint8_t idx = 0; idx < command.numArguments; idx++) {
      sprintf(buffer, "i: %d m: %c v: %s-\n", idx, command.modifiers[idx], command.values[idx].c_str());
      Serial.write(buffer);
    }
  }
  
  if (0 == command.getName().compareTo("arduboy.setCursor") && command.numArguments == 2) {
      int x = command.values[0].toInt();
      int y = command.values[1].toInt();
      g.arduboy.setCursor(x,y);
      Serial.write("OK");
  }

  else if (0 == command.getName().compareTo("arduboy.clear") && command.numArguments == 0) {
    g.arduboy.clear();
    Serial.write("OK");
  }

  else if (0 == command.getName().compareTo("arduboy.display") && command.numArguments == 0) {
    g.arduboy.display();
    Serial.write("OK");
  }

  else if (0 == command.getName().compareTo("arduboy.print") && command.numArguments == 1) {
    g.arduboy.print(g.command.values[0]);
    Serial.write("OK");
  }

  else if (0 == command.getName().compareTo("debug") && command.numArguments == 1) {
    if ( 0 == command.values[0].compareTo("0")) {
      g.debugLevel = DEBUG_OFF;
    } else {
      g.debugLevel = DEBUG_ON;
    }
    Serial.write("OK");
  }

  else {
    Serial.write("NOK");
  }

  Serial.write('\n');
  command.reset();
}


