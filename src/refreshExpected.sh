#!/bin/sh


# This script copies all output files as expected results. It should
# be executed from the directory where the expected result files
# should be refreshed.


for i in `ls *.p`; do
    echo $i;
    cp $i.output $i.expected
done
