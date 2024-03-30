#######################################################################################################
# Copyright (C) Plog 2024
# Author: Gaina Stefan
# Date: 22.06.2023
# Description: This Makefile is used to invoke the Makefiles in the subdirectories.
#######################################################################################################

export SRC := src
export OBJ := obj
export LIB := lib
export BIN := bin

FORMAT			  := clang-format -i
COMPILATION_TIMER := cd vendor/Compilation-Timer && ./compilation-timer

### MAKE SUBDIRECTORIES ###
all: start_timer format debug doxygen end_timer
production: start_timer format release doxygen end_timer

debug:
	$(MAKE) -C plog
	$(MAKE) -C test
	$(MAKE) -C example

release:
	$(MAKE) release -C plog

### CLEAN SUBDIRECTORIES ###
clean: start_timer
	$(MAKE) clean -C plog
	$(MAKE) clean -C test
	$(MAKE) clean -C example
	$(COMPILATION_TIMER) end

### MAKE DOXYGEN ###
doxygen:
	doxygen docs/doxygen.conf

### MAKE FORMAT ###
format:
	$(FORMAT) plog/$(SRC)/*.c
	$(FORMAT) plog/include/*.h
	$(FORMAT) plog/include/*/*.h
	$(FORMAT) example/$(SRC)/*.c
	$(FORMAT) test/test-app/$(SRC)/*.c

### MAKE UNIT-TESTS ###
ut: start_timer ut-clean
	$(FORMAT) unit-tests/*/*/*/*.cpp
	$(FORMAT) unit-tests/*/*.hpp
	$(MAKE) -C unit-tests
	$(COMPILATION_TIMER) end

### CLEAN UNIT-TESTS ###
ut-clean:
	$(MAKE) clean -C unit-tests

### TEST ###
tst: start_timer
	$(MAKE) tst -C test
	$(COMPILATION_TIMER) end

### START TIMER ###
start_timer:
	$(COMPILATION_TIMER) start

### END TIMER ###
end_timer:
	$(COMPILATION_TIMER) end
