#######################################################################################################
# Copyright (C) Plog 2024                                                                             #
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   15.12.2023  Gaina Stefan               Initial version.                                           #
#   20.12.2023  Gaina Stefan               Updated copyright.                                         #
# Description: This Makefile is used to generate an application that loads Plog library and gives an  #
# example of how the functions might get called.                                                      #
#######################################################################################################

CFLAGS = `pkg-config --cflags glib-2.0` -Wno-format
# CFLAGS += -DPLOG_STRIP_ALL
LDFLAGS = -Wl,-rpath='.' -L../plog/$(LIB) -lplog

INCLUDES = -I../plog/include

SOURCES    := $(wildcard $(SRC)/*.c)
OBJECTS    := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
EXECUTABLE := plog-example

all: | create_dirs $(EXECUTABLE) copy_library

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

### COPY LIBRARY ###
copy_library:
	cp ../plog/$(LIB)/libplog.so bin/
