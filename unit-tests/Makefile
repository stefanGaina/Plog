#######################################################################################################
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   22.06.2023  Gaina Stefan               Initial version.                                           #
#   15.12.2023  Gaina Stefan               Move unit tests in different directories.                  #
# Description: This Makefile is used to invoke the Makefiles in the subdirectories.                   #
#######################################################################################################

export CXX   = g++
export LCOV := ../../vendor/lcov/lcov.perl

export CXXFLAGS = -fpermissive -fno-inline -g -fprofile-arcs -ftest-coverage --coverage
export LDFLAGS  = -L../../../vendor/gtest/$(LIB) -lgtest -lgtest_main -lpthread -L../../../vendor/gmock/$(LIB) -lgmock -lgmock_main

export LCOV_BRANCH_FLAG = --rc lcov_branch_coverage=1

### MAKE SUBDIRECTORIES ###
all:
	$(MAKE)           -C plog
	$(MAKE) run_tests -C plog
	$(MAKE) clean     -C plog

### CLEAN SUBDIRECTORIES ###
clean:
	$(MAKE) clean -C plog
