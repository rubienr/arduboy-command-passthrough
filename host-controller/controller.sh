#!/bin/bash

PATH=`dirname $(readlink -f $0)`

/usr/bin/python3 $PATH/controller/controller-gui.py

