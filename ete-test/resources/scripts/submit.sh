#!/usr/bin/env bash

set -ex

export TCFRAME_HOME=../../tcframe

g++ -o solution_alt solution_alt.cpp
./runner submit --solution=./solution_alt
