#!/usr/bin/env bash

set -ex

export TCFRAME_HOME=../../tcframe

slug=$1
g++ -o ${slug}_solution ${slug}_solution.cpp
$TCFRAME_HOME/scripts/tcframe
./${slug} --solution=./${slug}_solution
