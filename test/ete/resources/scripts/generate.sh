#!/usr/bin/env bash

set -ex

export TCFRAME_HOME=../../tcframe
export TCFRAME_CXX_FLAGS="-I .."

g++ -o solution solution.cpp
$TCFRAME_HOME/scripts/tcframe build
./runner --solution=./solution
