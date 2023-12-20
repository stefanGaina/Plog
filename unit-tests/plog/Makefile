#######################################################################################################
# Copyright Plog 2024                                                                                 #
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   15.12.2023  Gaina Stefan               Initial version.                                           #
#   20.12.2023  Gaina Stefan               Updated copyright.                                         #
# Description: This Makefile is used to invoke the Makefiles in the subdirectories.                   #
#######################################################################################################

export TESTED_FILE_DIR := ../../../plog/$(SRC)
export VALGRIND        := valgrind --leak-check=summary --show-leak-kinds=all

all:
	$(MAKE) -C configuration
	$(MAKE) -C plog
	$(MAKE) -C plog_version
	$(MAKE) -C queue
	$(MAKE) -C vector

### RUN TESTS ###
run_tests:
	$(MAKE) run_tests -C configuration
	$(MAKE) run_tests -C plog
	$(MAKE) run_tests -C plog_version
	$(MAKE) run_tests -C queue
	$(MAKE) run_tests -C vector

### CLEAN ###
clean:
	$(MAKE) clean -C configuration
	$(MAKE) clean -C plog
	$(MAKE) clean -C plog_version
	$(MAKE) clean -C queue
	$(MAKE) clean -C vector
