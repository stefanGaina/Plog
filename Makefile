#######################################################################################################
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   22.06.2023  Gaina Stefan               Initial version.                                           #
# Description: This Makefile is used to invoke the Makefiles in the subdirectories.                   #
#######################################################################################################

export CC = gcc

export SRC := src
export OBJ := obj
export LIB := lib
export BIN := bin

export COVERAGE_REPORT := coverage_report

GENHTML       = vendor/lcov/genhtml.perl
GENHTML_FLAGS = --branch-coverage --num-spaces=4 --output-directory coverage_report/

INFO_FILES = $(COVERAGE_REPORT)/plog.info

### MAKE SUBDIRECTORIES ###
all: detect_os
	$(MAKE) -C plog
	$(MAKE) -C test

### CLEAN SUBDIRECTORIES ###
clean: detect_os
	$(MAKE) clean -C plog
	$(MAKE) clean -C test

### MAKE UNIT-TESTS ###
ut: detect_os create_dir
	$(MAKE) -C unit-tests
	$(MAKE) run_tests -C unit-tests
	perl $(GENHTML) $(INFO_FILES) $(GENHTML_FLAGS)

### CREATE DIRECTORY ###
create_dir:
ifeq (Windows_NT, $(OS))
	if not exist "$(COVERAGE_REPORT)" mkdir $(COVERAGE_REPORT)
endif
ifeq (Linux, $(shell uname))
	mkdir -p $(COVERAGE_REPORT)
endif

### CLEAN UNIT-TESTS ###
ut-clean: detect_os
ifeq (Windows_NT, $(OS))
	$(RM) $(COVERAGE_REPORT)\*
	$(RM) $(COVERAGE_REPORT)\src\*
	rd /s /q $(COVERAGE_REPORT)\src
endif
ifeq (Linux, $(shell uname))
	$(RM) $(COVERAGE_REPORT)/*
endif

### DETECT OPERATING SYSTEM ###
detect_os:
ifeq (Windows_NT, $(OS))
export OS_DIR := windows
export LIB_EXTENSION := dll
export RM := del /f /q
else ifeq (Linux, $(shell uname))
export OS_DIR := linux
export LIB_EXTENSION := so
export RM := rm -rf
else
$(error Platform is not supported)
endif
