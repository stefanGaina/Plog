#######################################################################################################
# Copyright (C) Plog 2024
# Author: Gaina Stefan
# Date: 15.12.2023
# Description: This Makefile is used to generate an application that loads Plog library and gives an
# example of how the functions might get called.
#######################################################################################################

CFLAGS	:= `pkg-config --cflags glib-2.0` -Wno-format
# CFLAGS += -DPLOG_STRIP_ALL
LDFLAGS := -Wl,-Bdynamic,-rpath,'$$ORIGIN'/../../plog/$(LIB) -L../plog/$(LIB) -lplog

INCLUDES := -I../plog/include

SOURCES	   := $(wildcard $(SRC)/*.c)
OBJECTS	   := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
EXECUTABLE := plog-example

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
