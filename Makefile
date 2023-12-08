#######################################################################################################
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   22.06.2023  Gaina Stefan               Initial version.                                           #
#   08.12.2023  Gaina Stefan               Added example.                                             #
# Description: This Makefile is used to invoke the Makefiles in the subdirectories.                   #
#######################################################################################################

export SRC := src
export OBJ := obj
export LIB := lib
export BIN := bin

export COVERAGE_REPORT := coverage_report

GENHTML       = vendor/lcov/genhtml.perl
GENHTML_FLAGS = --branch-coverage --num-spaces=4 --output-directory $(COVERAGE_REPORT)/

INFO_FILES = $(COVERAGE_REPORT)/plog.info \
			 $(COVERAGE_REPORT)/queue.info

### MAKE SUBDIRECTORIES ###
all:
	$(MAKE) -C plog
	$(MAKE) -C test
	$(MAKE) -C example

### CLEAN SUBDIRECTORIES ###
clean:
	$(MAKE) clean -C plog
	$(MAKE) clean -C test
	$(MAKE) clean -C example

### MAKE UNIT-TESTS ###
ut: create_dir
	$(MAKE) -C unit-tests
	perl $(GENHTML) $(INFO_FILES) $(GENHTML_FLAGS)

### CREATE DIRECTORY ###
create_dir:
	mkdir -p $(COVERAGE_REPORT)

### CLEAN UNIT-TESTS ###
ut-clean:
	rm -rf $(COVERAGE_REPORT)/*
	$(MAKE) clean -C unit-tests
