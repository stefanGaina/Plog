#######################################################################################################
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   22.06.2023  Gaina Stefan               Initial version.                                           #
# Description: This Makefile is used to invoke the Makefiles in the subdirectories.                   #
#######################################################################################################

export CXX   = g++
export LCOV := ../../vendor/lcov/lcov.perl

export CXXFLAGS = -fpermissive
export CFLAGS   = -fpermissive -fno-inline -g -fprofile-arcs -ftest-coverage --coverage

export LCOV_BRANCH_FLAG = --rc lcov_branch_coverage=1

all: lib_path
	$(MAKE) -C plog

### LIBRARY PATH ###
lib_path:
export LDFLAGS  = -L../../vendor/googletest/lib/$(OS_DIR)/ -lgtest -lgtest_main -lpthread

### RUN TESTS ###
run_tests:
	$(MAKE) run_tests -C plog

### CLEAN ###
clean:
	$(MAKE) clean -C plog
