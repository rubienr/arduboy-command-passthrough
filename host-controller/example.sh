#!/bin/bash

ARDUBOY_SERIAL=/dev/ttyACM0

stty -F $ARDUBOY_SERIAL 9600 cs8 -cstopb -parenb
echo "arduboy.clear"                 > $ARDUBOY_SERIAL
echo "arduboy.setCursor %i %i 30 30" > $ARDUBOY_SERIAL
echo "arduboy.print %s Hello World!" > $ARDUBOY_SERIAL
echo "arduboy.display"               > $ARDUBOY_SERIAL
