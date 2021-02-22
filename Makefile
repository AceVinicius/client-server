# -*- Makefile -*-

# General
CC			= clang
LD			= clang
DBG         = 
RM			= rm -rdf
SHELL       = /bin/zsh
INSTALL		= install
DEBUG       = -O3 -g3 
WARNING     = -Wall -Wextra -Wpedantic


## CHANGE THIS ##
TARGET		= client
SRC_DIR		= src
OBJ_DIR		= obj
DEP_DIR     = obj
BIN_DIR		= bin
## CHANGE THIS ##


# CFLAGS, LDFLAGS, CPPFLAGS, PREFIX can be overriden on CLI
CFLAGS		:= $(WARNING) $(DEBUG) -std=c17
CPPFLAGS	:= -MMD -MP -fPIE
LDFLAGS		:= 
PREFIX		:= /usr/local
TARGET_ARCH :=

# Compiler Flags
ALL_CFLAGS := $(CFLAGS)

# Preprocessor Flags
ALL_CPPFLAGS	:= $(CPPFLAGS)

# Linker Flags
ALL_LDFLAGS		:= $(LDFLAGS)
ALL_LDLIBS		:= -lc -lreadline -lm

# Source, Binaries, Dependencies
SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES := $(foreach file,$(notdir $(SRC_FILES)),$(OBJ_DIR)/$(file:.c=.o))
DEP_FILES := $(foreach file,$(notdir $(SRC_FILES)),$(DEP_DIR)/$(file:.c=.d))
BIN_FILE  := $(BIN_DIR)/$(TARGET)
-include $(DEP_FILES)



# Build Rules
.PHONY: clean
.DEFAULT_GOAL := all


all: directories $(BIN_FILE) execute


directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DEP_DIR)
	@mkdir -p $(BIN_DIR)

# Linker
$(BIN_FILE): $(OBJ_FILES)
	@echo '\n\n---------------------- LINKER\n\n'
	
	$(LD) $(ALL_LDFLAGS) $^ $(ALL_LDLIBS) -o $@

# Object
objs: $(OBJ_FILES)

$(OBJ_DIR)/%.o:*/*/%.c
	@echo '\n\n---------------------- COMPILER:' $< '\n\n'
	
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -c -o $@ $<


# Run
execute:
	@echo '\n\n---------------------- RUN\n\n'

	$(DBG) ./bin/client


# Install
install: $(BIN_FILE)
	@echo '\n\n---------------------- INSTALLER\n\n'
	
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) $(BIN_FILE) $(PREFIX)/bin


# Clean and Compile
remake: clean all


# Clean
clean:
	@echo '\n\n---------------------- CLEANER\n\n'

	$(RM) $(OBJ_DIR) $(BIN_DIR)



# Print Variables For Debug
REAL_CC := $(CC)
REAL_LD := $(LD)

show:
	@echo ''
	@echo 'TARGET:       ' $(TARGET)
	@echo 'CURDIR:       ' $(CURDIR)
	@echo '--------------'
	@echo 'COMPILER:     ' $(REAL_CC)
	@echo 'LINKER:       ' $(REAL_LD)
	@echo 'DBG:          ' $(DBG)
	@echo 'SHELL:        ' $(SHELL)
	@echo '--------------'
	@echo 'PREFIX:       ' $(PREFIX)
	@echo 'TARGET_ARCH:  ' $(TARGET_ARCH)
	@echo '--------------'
	@echo 'ALL_CFLAGS:   ' $(ALL_CFLAGS)
	@echo 'ALL_CPPFLAGS: ' $(ALL_CPPFLAGS)
	@echo 'ALL_LDFLAGS:  ' $(ALL_LDFLAGS)
	@echo 'ALL_LDLIBS:   ' $(ALL_LDLIBS)
	@echo '--------------'
	@echo 'SRC_DIR:      ' $(SRC_DIR)
	@echo 'OBJ_DIR:      ' $(OBJ_DIR)
	@echo 'DEP_DIR:      ' $(DEP_DIR)
	@echo 'BIN_DIR:      ' $(BIN_DIR)
	@echo '--------------'
	@echo 'SRC_FILES:    ' $(SRC_FILES)
	@echo 'OBJ_FILES:    ' $(OBJ_FILES)
	@echo 'DEP_FILES:    ' $(DEP_FILES)
	@echo 'BIN_FILE:     ' $(BIN_FILE)
	@echo ''


# Help
help:
# 	@echo ''
# 	@echo 'TARGET:       ' $(TARGET)
# 	@echo 'CURDIR:       ' $(CURDIR)
# 	@echo '--------------'
# 	@echo 'COMPILER:     ' $(REAL_CC)
# 	@echo 'LINKER:       ' $(REAL_LD)
# 	@echo 'SHELL:        ' $(SHELL)
# 	@echo '--------------'
# 	@echo 'PREFIX:       ' $(PREFIX)
# 	@echo 'TARGET_ARCH:  ' $(TARGET_ARCH)
# 	@echo '--------------'
# 	@echo 'ALL_CFLAGS:   ' $(ALL_CFLAGS)
# 	@echo 'ALL_CPPFLAGS: ' $(ALL_CPPFLAGS)
# 	@echo 'ALL_LDFLAGS:  ' $(ALL_LDFLAGS)
# 	@echo 'ALL_LDLIBS:   ' $(ALL_LDLIBS)
# 	@echo '--------------'
# 	@echo 'SRC_DIR:      ' $(SRC_DIR)
# 	@echo 'OBJ_DIR:      ' $(OBJ_DIR)
# 	@echo 'BIN_DIR:      ' $(BIN_DIR)
# 	@echo '--------------'
# 	@echo 'SRC_FILES:    ' $(SRC_FILES)
# 	@echo 'OBJ_FILES:    ' $(OBJ_FILES)
# 	@echo 'DEP_FILES:    ' $(DEP_FILES)
# 	@echo 'BIN_FILE:     ' $(BIN_FILE)
# 	@echo ''