#!/usr/bin/env bash

set -e

export TCFRAME_HOME=../../tcframe

g++ -o solution_rounded solution_rounded.cpp
$TCFRAME_HOME/scripts/tcframe grade --solution=./solution_rounded $@
