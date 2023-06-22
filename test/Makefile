#######################################################################################################
# Module history:                                                                                     #
#   Date:       Author:                    Reason:                                                    #
#   22.06.2023  Gaina Stefan               Initial version.                                           #
# Description: This Makefile is used to generate an application that loads Plog library and tests its #
# API using API-Test.                                                                                 #
#######################################################################################################

CFLAGS  = -Wall -Werror
LDFLAGS = -Wl,-rpath='.' -L../vendor/apitest/lib/$(OS_DIR)/ -lapitest \
		  -Wl,-rpath='.' -L../plog/lib/ -lplog

INCLUDES = -I../vendor/apitest/include/ \
		   -I../plog/include/

SOURCES    := $(wildcard $(SRC)/*.c)
OBJECTS    := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
EXECUTABLE := plog-test.exe

all: | create_dirs $(EXECUTABLE) copy_libraries

### CREATE DIRECTORIES ###
create_dirs:
ifeq (Windows_NT, $(OS))
	if not exist "$(OBJ)" mkdir $(OBJ)
	if not exist "$(BIN)" mkdir $(BIN)
endif
ifeq (Linux, $(shell uname))
	mkdir -p $(OBJ)
	mkdir -p $(BIN)
endif

### BINARIES ###
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^ $(LDFLAGS)

### OBJECTS ###
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

### CLEAN ###
clean:
ifeq (Windows_NT, $(OS))
	$(RM) $(OBJ)\*
	$(RM) $(BIN)\*
endif
ifeq (Linux, $(shell uname))
	$(RM) $(OBJ)/*
	$(RM) $(BIN)/*
endif

### COPY LIBRARIES ###
copy_libraries:
ifeq (Windows_NT, $(OS))
	copy ..\vendor\apitest\lib\$(OS_DIR)\libapitest.$(LIB_EXTENSION) bin
	copy ..\plog\lib\libplog.$(LIB_EXTENSION) bin
endif
ifeq (Linux, $(shell uname))
	cp ../vendor/apitest/lib/$(OS_DIR)/libapitest.$(LIB_EXTENSION) bin/
	cp ../plog/lib/libplog.$(LIB_EXTENSION) bin/
endif