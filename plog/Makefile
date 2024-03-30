#######################################################################################################
# Copyright (C) Plog 2024
# Author: Gaina Stefan
# Date: 29.06.2023
# Description: This Makefile is used to generate the Plog library.
#######################################################################################################

CFLAGS	:= `pkg-config --cflags glib-2.0` -Wextra -Wall -Werror -Wno-error=format-truncation -fanalyzer -fPIC
LDFLAGS := `pkg-config --libs glib-2.0`

INCLUDES := -Iinclude

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
LIBRARY := libplog.so

### DEVELOPMENT ###
all: CFLAGS += -O1 -g
all: build

### PRODUCTION ###
release: CFLAGS += -O2 -DNDEBUG
release: build

### BUILD ###
build: | create_dirs $(LIBRARY)

### CREATE DIRECTORIES ###
create_dirs:
	mkdir -p $(OBJ)
	mkdir -p $(LIB)

### BINARIES ###
$(LIBRARY): $(OBJECTS)
	$(CC) $(CFLAGS) -shared -o $(LIB)/$@ $^ $(LDFLAGS)

### OBJECTS ###
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

### CLEAN ###
clean:
	rm -rf $(OBJ)/*
	rm -rf $(LIB)/*
