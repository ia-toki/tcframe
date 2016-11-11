#!/usr/bin/env bash

set -ex

export TCFRAME_HOME=../../tcframe

g++ -o solution solution.cpp
$TCFRAME_HOME/scripts/tcframe build
./runner
