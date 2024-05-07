#######################################################################################################
# Copyright (C) Plog 2024
# Author: Gaina Stefan
# Date: 22.06.2023
# Description: This Makefile is used to invoke the Makefiles in the subdirectories.
#######################################################################################################

export LCOV		:= ../../vendor/lcov/$(BIN)/lcov.perl
export VALGRIND := valgrind --leak-check=summary --show-leak-kinds=all

export CXXFLAGS := -fpermissive -fno-inline -g -fprofile-arcs -ftest-coverage --coverage
export CFLAGS	:= -DNDEBUG
export LDFLAGS	:= -L../../../vendor/gtest/$(LIB) -lgtest -lgtest_main -lpthread -L../../../vendor/gmock/$(LIB) -lgmock -lgmock_main

export LCOV_BRANCH_FLAG := --rc lcov_branch_coverage=1
export COVERAGE_REPORT	:= ../coverage_report

GENHTML		  := ../vendor/lcov/$(BIN)/genhtml.perl
GENHTML_FLAGS := --branch-coverage --num-spaces=4 --output-directory $(COVERAGE_REPORT) --dark-mode

INFO_FILES := $(COVERAGE_REPORT)/configuration.info \
			  $(COVERAGE_REPORT)/plog_version.info	\
			  $(COVERAGE_REPORT)/plog.info			\
			  $(COVERAGE_REPORT)/queue.info			\
			  $(COVERAGE_REPORT)/vector.info

### MAKE SUBDIRECTORIES ###
all:
	$(MAKE)			  -C plog
	$(MAKE) run_tests -C plog
	$(MAKE) clean	  -C plog
	mkdir -p $(COVERAGE_REPORT)
	perl $(GENHTML) $(INFO_FILES) $(GENHTML_FLAGS)

### CLEAN SUBDIRECTORIES ###
clean:
	$(MAKE) clean -C plog
	rm -rf $(COVERAGE_REPORT)/*
