#!/usr/bin/env bash

set -e

export TCFRAME_HOME=../../tcframe

g++ -o solution_alt solution_alt.cpp
./runner grade --solution=./solution_alt $@
