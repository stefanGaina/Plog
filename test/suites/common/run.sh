#!/bin/bash

#######################################################################################################
# Copyright (C) Heap of Battle 2024
# Author: Gaina Stefan
# Date: 11.02.2024
# Description: This script is used to execute the game with the test steps and invoke the verify
# script. It expects a command line argument with the name of the directory in test/ which contains
# the "steps.txt" file.
#######################################################################################################

test_directory=../test/$1
plog_conf=plog.conf

cd $1
cp conf/$plog_conf $plog_conf

./../../test-app/bin/plog-test commands.txt > output.txt

# perl ../test/common/verify.pl $test_directory
