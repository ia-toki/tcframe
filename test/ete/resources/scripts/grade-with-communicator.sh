#!/usr/bin/env bash

set -ex

export TCFRAME_HOME=../../tcframe

g++ -o solution_alt solution_alt.cpp
g++ -o communicator communicator.cpp
./runner grade --solution=./solution_alt --communicator=./communicator
