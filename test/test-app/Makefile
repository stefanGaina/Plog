#######################################################################################################
# Copyright (C) Plog 2024
# Author: Gaina Stefan
# Date: 22.06.2023
# Description: This Makefile is used to generate an application that loads Plog library and tests its
# API using API-Test.
#######################################################################################################

CFLAGS	:= `pkg-config --cflags glib-2.0` -Wall -Werror -Wno-format-security
LDFLAGS := `pkg-config --libs glib-2.0`																				\
		   -Wl,-Bdynamic,-rpath,'$$ORIGIN'/../../../vendor/API-Test/$(LIB) -L../../vendor/API-Test/$(LIB) -lapitest \
		   -Wl,-Bdynamic,-rpath,'$$ORIGIN'/../../../plog/$(LIB) -L../../plog/$(LIB) -lplog

INCLUDES := -I../../vendor/API-Test/include \
			-I../../plog/include

SOURCES	   := $(wildcard $(SRC)/*.c)
OBJECTS	   := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
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
