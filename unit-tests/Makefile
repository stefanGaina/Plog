#######################################################################################################
# Copyright (C) Plog 2024                                                                             #
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   22.06.2023  Gaina Stefan               Initial version.                                           #
#   20.12.2023  Gaina Stefan               Updated copyright.                                         #
#   15.12.2023  Gaina Stefan               Move unit tests in different directories.                  #
#   13.01.2024  Gaina Stefan               Clean unnecessary exports.                                 #
#   24.01.2024  Gaina Stefan               Moved logic from main Makefile.                            #
# Description: This Makefile is used to invoke the Makefiles in the subdirectories.                   #
#######################################################################################################

export LCOV     := ../../vendor/lcov/lcov.perl
export VALGRIND := valgrind --leak-check=summary --show-leak-kinds=all

export CXXFLAGS := -fpermissive -fno-inline -g -fprofile-arcs -ftest-coverage --coverage
export LDFLAGS  := -L../../../vendor/gtest/$(LIB) -lgtest -lgtest_main -lpthread -L../../../vendor/gmock/$(LIB) -lgmock -lgmock_main

export LCOV_BRANCH_FLAG := --rc lcov_branch_coverage=1
export COVERAGE_REPORT  := ../coverage_report

GENHTML       = ../vendor/lcov/genhtml.perl
GENHTML_FLAGS = --branch-coverage --num-spaces=4 --output-directory $(COVERAGE_REPORT)

INFO_FILES = $(COVERAGE_REPORT)/configuration.info \
			 $(COVERAGE_REPORT)/plog_version.info  \
			 $(COVERAGE_REPORT)/plog.info          \
			 $(COVERAGE_REPORT)/queue.info         \
			 $(COVERAGE_REPORT)/vector.info

### MAKE SUBDIRECTORIES ###
all:
	$(MAKE)           -C plog
	$(MAKE) run_tests -C plog
	$(MAKE) clean     -C plog
	mkdir -p $(COVERAGE_REPORT)
	perl $(GENHTML) $(INFO_FILES) $(GENHTML_FLAGS)

### CLEAN SUBDIRECTORIES ###
clean:
	$(MAKE) clean -C plog
	rm -rf $(COVERAGE_REPORT)/*
