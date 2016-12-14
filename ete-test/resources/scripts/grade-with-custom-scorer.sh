#!/usr/bin/env bash

set -ex

export TCFRAME_HOME=../../tcframe

g++ -o solution_alt solution_alt.cpp
g++ -o scorer scorer.cpp
./runner grade --solution=./solution_alt --scorer=./scorer
