#######################################################################################################
# Copyright (C) Plog 2024                                                                             #
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   22.06.2023  Gaina Stefan               Initial version.                                           #
#   29.06.2023  Gaina Stefan               Changed plog library name.                                 #
#   13.09.2023  Gaina Stefan               Fixed warnings on Linux.                                   #
#   20.12.2023  Gaina Stefan               Updated copyright.                                         #
#   24.01.2024  Gaina Stefan               Updated for API-Test 2.0.0.                                #
# Description: This Makefile is used to generate an application that loads Plog library and tests its #
# API using API-Test.                                                                                 #
#######################################################################################################

CFLAGS  = `pkg-config --cflags glib-2.0` -Wall -Werror -Wno-format-security
LDFLAGS = `pkg-config --libs glib-2.0`                                                                       \
		  -Wl,-Bdynamic,-rpath,'$$ORIGIN'/../../vendor/API-Test/$(LIB) -L../vendor/API-Test/$(LIB) -lapitest \
		  -Wl,-Bdynamic,-rpath,'$$ORIGIN'/../../plog/$(LIB) -L../plog/$(LIB) -lplog

INCLUDES = -I../vendor/API-Test/include \
		   -I../plog/include

SOURCES    := $(wildcard $(SRC)/*.c)
OBJECTS    := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
EXECUTABLE := plog-test

all: | create_dirs $(EXECUTABLE)

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
