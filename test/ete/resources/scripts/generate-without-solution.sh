#!/usr/bin/env bash

set -ex

export TCFRAME_HOME=../../tcframe

$TCFRAME_HOME/scripts/tcframe build
./runner
