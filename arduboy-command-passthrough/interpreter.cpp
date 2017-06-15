#include "interpreter.h"
#include "globals.h"

extern Globals g;

//----------------------------------------------------------------------------------------------------
bool CommandInterpreter::parseCommand(String &command, String &arguments)
{
  if (command.length() <= 0)
    return false;

  g.command.setName(command);
  int separatorIdx = arguments.lastIndexOf(INTERPRETER_MODIFIER_PREFIX) + 1;
  String modifiers = arguments.substring(0, separatorIdx + 1);
  modifiers.trim();
  String values = arguments.substring(separatorIdx + 1);
  values.trim();


  while (modifiers.length() > 1 && values.length() > 0)
  {
    int startIdx = modifiers.indexOf(INTERPRETER_MODIFIER_PREFIX);
    char modifier = modifiers.charAt(startIdx + 1);
    modifiers.remove(0, startIdx + 1);


    startIdx = values.indexOf(INTERPRETER_WORD_DELIMITER);
    String value;
    if ( startIdx >= 0)
    {
      if (modifiers.length() <= 1)
        value = values;
      else
        value = values.substring(0, startIdx + 1);
    }
    else
    {
      value = values;
    }
    values.remove(0, startIdx + 1);
    value.trim();
    g.command.addArgument(modifier, value);
  }

  return true;
}

//----------------------------------------------------------------------------------------------------
void CommandInterpreter::interpretCommand(Command &command) {
  if (command.isEmpty())
    return;

  if (DEBUG_ON == g.debugLevel) {
    char buffer[30];
    sprintf(buffer, "#: %d\n", command.numArguments);
    Serial.write(buffer);
    for (uint8_t idx = 0; idx < command.numArguments; idx++) {
      sprintf(buffer, "i: %d m: %c v: %s-\n", idx, command.modifiers[idx], command.values[idx].c_str());
      Serial.write(buffer);
    }
  }

  if (0 == command.getName().compareTo("arduboy.setCursor") && command.numArguments == 2) {
    int x = command.values[0].toInt();
    int y = command.values[1].toInt();
    g.arduboy.setCursor(x, y);
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

//----------------------------------------------------------------------------------------------------
void Command::addArgument(char modifier, String &value) {
  modifiers[numArguments] = modifier;
  values[numArguments] = value;
  ++numArguments;
  mIsEmpty = false;
}

//----------------------------------------------------------------------------------------------------
void Command::setName(String &name) {
  reset();
  mName = name;
  mIsEmpty = false;
}

//----------------------------------------------------------------------------------------------------
String &Command::getName() {
  return mName;
}

//----------------------------------------------------------------------------------------------------
bool Command::isEmpty() {
  return mIsEmpty;
}

//----------------------------------------------------------------------------------------------------
void Command::reset() {
  numArguments = 0;
  mIsEmpty = true;
}

