#!/usr/bin/env bash

set -ex

export TCFRAME_HOME=../../tcframe

slug=$1
g++ -o ${slug}_solution_alt ${slug}_solution_alt.cpp
./${slug} submit --solution=./${slug}_solution_alt
