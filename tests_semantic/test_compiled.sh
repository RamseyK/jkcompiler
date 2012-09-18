#!/bin/sh

. ../tests_lib.inc

$BINARY --help
RETVAL=$?
if [ $RETVAL == 3 ]; then
    echo PASSED
else
    echo FAILED
fi
