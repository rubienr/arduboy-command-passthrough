#pragma once

#include <Arduino.h>

#define INTERPRETER_WORD_DELIMITER ' '
#define INTERPRETER_MODIFIER_PREFIX '%'

//----------------------------------------------------------------------------------------------------
typedef class Command {
  private:

    String mName;
    bool mIsEmpty;

  protected:

  public:

    uint8_t numArguments;
    char modifiers[8];
    String values[8];

    void addArgument(const char modifier, const String &value);
    void setName(const String &name);
    String &getName();
    bool isEmpty();
    void reset();

    Command() :
      mName(),
      mIsEmpty(false),
      numArguments(0),
      modifiers{},
      values{}
    {}

} Command;

//----------------------------------------------------------------------------------------------------
typedef class CommandInterpreter {
  private:

  protected:

  public:

    /**
      Parses the command and argumenta and stores it to a command object.
      @param command the command name
      @param arguments command arguments: %i %u %s -10 1 "string"
    */
    static bool parseCommand(const String &input, const String &arguments);

    /**
      Interprets and executes the command object.
    */
    static void interpretCommand(Command &command);
} CommandInterpreter;


