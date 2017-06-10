#!/usr/bin/env bash

set -e

export TCFRAME_HOME=../../tcframe

$TCFRAME_HOME/scripts/tcframe build
./runner
