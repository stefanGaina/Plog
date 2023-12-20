#######################################################################################################
# Copyright (C) Plog 2024                                                                             #
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   29.06.2023  Gaina Stefan               Initial version.                                           #
#   08.12.2023  Gaina Stefan               Added glib.                                                #
#   20.12.2023  Gaina Stefan               Updated copyright.                                         #
# Description: This Makefile is used to generate the Plog library.                                    #
#######################################################################################################

CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Werror -Wpointer-arith -fPIC
LDFLAGS = `pkg-config --libs glib-2.0`

INCLUDES = -Iinclude

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
LIBRARY := libplog.so

all: | create_dirs $(LIBRARY)

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
