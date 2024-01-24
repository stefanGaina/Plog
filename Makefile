#######################################################################################################
# Copyright (C) Plog 2024                                                                             #
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   22.06.2023  Gaina Stefan               Initial version.                                           #
#   08.12.2023  Gaina Stefan               Added example.                                             #
#   15.12.2023  Gaina Stefan               Added unit test.                                           #
#   20.12.2023  Gaina Stefan               Updated copyright.                                         #
#   13.01.2024  Gaina Stefan               Added the generation of the doxygen files.                 #
#   18.01.2024  Gaina Stefan               Added compilation timer.                                   #
#   24.01.2024  Gaina Stefan               Moved logic to unit-tests Makefile.                        #
# Description: This Makefile is used to invoke the Makefiles in the subdirectories.                   #
#######################################################################################################

export SRC := src
export OBJ := obj
export LIB := lib
export BIN := bin

COMPILATION_TIMER = cd vendor/Compilation-Timer && ./compilation-timer

### MAKE SUBDIRECTORIES ###
all: start_timer build doxygen end_timer

build:
	$(MAKE) -C plog
	$(MAKE) -C test
	$(MAKE) -C example

### CLEAN SUBDIRECTORIES ###
clean: start_timer
	$(MAKE) clean -C plog
	$(MAKE) clean -C test
	$(MAKE) clean -C example
	$(COMPILATION_TIMER) end

### MAKE UNIT-TESTS ###
ut: start_timer ut-clean
	$(MAKE) -C unit-tests
	$(COMPILATION_TIMER) end

### CLEAN UNIT-TESTS ###
ut-clean:
	$(MAKE) clean -C unit-tests

### MAKE DOXYGEN ###
doxygen:
	doxygen docs/doxygen.conf

### START TIMER ###
start_timer:
	$(COMPILATION_TIMER) start

### END TIMER ###
end_timer:
	$(COMPILATION_TIMER) end
