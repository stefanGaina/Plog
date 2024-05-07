#######################################################################################################
# Copyright (C) Plog 2024
# Author: Gaina Stefan
# Date: 22.06.2023
# Description: This Makefile is used to generate an application that loads Plog library and tests its
# API using API-Test.
#######################################################################################################

all:
	$(MAKE) -C test-app

### CLEAN ###
clean:
	$(MAKE) clean -C test-app

### TEST ###
tst:
	$(MAKE) -C suites
#	./$(BIN)/plog-test test.txt > output.txt
