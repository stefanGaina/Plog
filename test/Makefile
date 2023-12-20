#######################################################################################################
# Copyright (C) Plog 2024                                                                             #
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   22.06.2023  Gaina Stefan               Initial version.                                           #
#   29.06.2023  Gaina Stefan               Changed plog library name.                                 #
#   13.09.2023  Gaina Stefan               Fixed warnings on Linux.                                   #
#   20.12.2023  Gaina Stefan               Updated copyright.                                         #
# Description: This Makefile is used to generate an application that loads Plog library and tests its #
# API using API-Test.                                                                                 #
#######################################################################################################

CFLAGS  = `pkg-config --cflags glib-2.0` -Wall -Werror -Wno-format-security
LDFLAGS = -Wl,-rpath='.' -L../vendor/apitest/$(LIB) -lapitest \
		  -Wl,-rpath='.' -L../plog/$(LIB) -lplog

INCLUDES = -I../vendor/apitest/include \
		   -I../plog/include

SOURCES    := $(wildcard $(SRC)/*.c)
OBJECTS    := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
EXECUTABLE := plog-test.exe

all: | create_dirs $(EXECUTABLE) copy_libraries

### CREATE DIRECTORIES ###
create_dirs:
	mkdir -p $(OBJ)
	mkdir -p $(BIN)

### BINARIES ###
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^ $(LDFLAGS)

### OBJECTS ###
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

### CLEAN ###
clean:
	rm -rf $(OBJ)/*
	rm -rf $(BIN)/*

### COPY LIBRARIES ###
copy_libraries:
	cp ../vendor/apitest/$(LIB)/libapitest.so bin/
	cp ../plog/$(LIB)/libplog.so bin/
