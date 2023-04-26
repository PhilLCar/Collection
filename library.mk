# This is a template for every C/C++ library that conforms to [this] standard
# More information: github.com/PhilLCar (philippe-caron@hotmail.com)

# NOTE: that since all paths are included, files should have different names
# even if they are not in the same subfolder.

# (!) The user should define NAME, MAJOR, CFLAGS, LIBRARIES and DIRECTORIES
# according to their needs.

# General parameters
CC     = gcc
CCXX   = g++
CFLAGS = $(ADD_CFLAGS) -Wall

# Default to C
ifeq "$(LANG)" "C++"
	CMP     = $(CCXX)
	EXT_SRC = .cpp
	EXT_HDR = .hpp
	NAME   := $(NAME)++
else
	CMP     = $(CC)
	EXT_SRC = .c
	EXT_HDR = .h
endif

############################################################################################
# The goal of the following function is to flatten the "obj" folder
# This lookup function is taken from:
# https://stackoverflow.com/questions/9674711/makefile-find-a-position-of-word-in-a-variable
_pos = $(if $(findstring $1,$2),$(call _pos,$1,$(wordlist 2,$(words $2),$2),x $3),$3)
pos  = $(words $(call _pos,$1,$2))

lookup = $(word $(call pos,$1,$2),$3)
############################################################################################


# Compilation parameters
DIRECTORIES = $(PROJECT_ROOTS) .
LIBRARIES   = $(ADD_LIBRARIES)
INCLUDES    = $(patsubst %, -I%/inc, $(DIRECTORIES))
SOURCES     = $(foreach path, $(DIRECTORIES), $(wildcard $(path)/src/*$(EXT_SRC)))
OBJECTS     = $(patsubst %, obj/%.o, $(notdir $(SOURCES)))

# Add the debug flag if variable is defined
ifdef DEBUG
	CFLAGS += -g
endif

# Output directories might not exist if freshly pulled from git, or after clean
obj:
	mkdir obj

bin:
	mkdir bin

lib:
	mkdir lib

# Rules for language-specific object files
%.c.o: obj
	$(CC) $(CFLAGS) $(INCLUDES) -c $(call lookup, $@, $(OBJECTS), $(SOURCES)) -o $@ $(LIBRARIES)

%.cpp.o: obj
	$(CCXX) $(CFLAGS) $(INCLUDES) -c $(call lookup, $@, $(OBJECTS), $(SOURCES)) -o $@ $(LIBRARIES)

# Global rules
library: lib $(OBJECTS)
	ar rcs lib/lib$(NAME)$(MAJOR).a obj/*$(EXT_SRC).o

test: bin library
	$(CMP) $(CFLAGS) $(LIBRARIES) $(INCLUDES) -Llib test/main$(EXT_SRC) -o bin/$(NAME)$(MAJOR) -l$(NAME)$(MAJOR)

clean:
	rm -rf obj
	rm -rf lib
	rm -rf bin

