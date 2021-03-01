# -*- Makefile -*-

# General
CC			= clang
LD			= clang
ST_AN       = #scan-build --force-analyze-debug-code -internal-stats --view -stats -internal-stats -enable-checker security.insecureAPI.strcpy -enable-checker security.insecureAPI.rand -enable-checker security.insecureAPI.decodeValueOfObjCType
DBG         = #gdb
RM			= rm -rdf
SHELL       = /bin/zsh
INSTALL		= install
DEBUG       = -O3 -g3
WARNING     = -Wall -Wextra -Wpedantic


## CHANGE THIS ##
BIN_DIR	= bin
CONSOLE_TARGET = client
SERVER_TARGET  = server

COMMON_SRC_DIR  = src/common
CONSOLE_SRC_DIR = src/console
SERVER_SRC_DIR  = src/server

COMMON_OBJ_DIR  = obj/common
CONSOLE_OBJ_DIR = obj/console
SERVER_OBJ_DIR  = obj/server

COMMON_DEP_DIR  = obj/common
CONSOLE_DEP_DIR = obj/console
SERVER_DEP_DIR  = obj/server
## CHANGE THIS ##



########################################################################
#
#                     Compilation Details
#
#########################################################################

# CFLAGS, LDFLAGS, CPPFLAGS, PREFIX can be overriden on CLI
CFLAGS		:= $(WARNING) $(DEBUG) -std=c17
CPPFLAGS	:= -MMD -MP
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



########################################################################
#
#                     Source, Binaries, Dependencies
#
#########################################################################

COMMON_SRC_FILES := $(shell find $(COMMON_SRC_DIR) -type f -name '*.c')
COMMON_OBJ_FILES := $(foreach file,$(notdir $(COMMON_SRC_FILES)),$(COMMON_OBJ_DIR)/$(file:.c=.o))
COMMON_DEP_FILES := $(foreach file,$(notdir $(COMMON_SRC_FILES)),$(COMMON_DEP_DIR)/$(file:.c=.d))

CONSOLE_SRC_FILES := $(shell find $(CONSOLE_SRC_DIR) -type f -name '*.c')
CONSOLE_OBJ_FILES := $(foreach file,$(notdir $(CONSOLE_SRC_FILES)),$(CONSOLE_OBJ_DIR)/$(file:.c=.o))
CONSOLE_DEP_FILES := $(foreach file,$(notdir $(CONSOLE_SRC_FILES)),$(CONSOLE_DEP_DIR)/$(file:.c=.d))

SERVER_SRC_FILES := $(SERVER_SRC_DIR)/$(wildcard *.c) $(SERVER_SRC_DIR)/$(wildcard */*.c)
SERVER_OBJ_FILES := $(foreach file,$(notdir $(SERVER_SRC_FILES)),$(SERVER_OBJ_DIR)/$(file:.c=.o))
SERVER_DEP_FILES := $(foreach file,$(notdir $(SERVER_SRC_FILES)),$(SERVER_DEP_DIR)/$(file:.c=.d))

CONSOLE_BIN_FILE = $(BIN_DIR)/$(CONSOLE_TARGET)
SERVER_BIN_FILE  = $(BIN_DIR)/$(SERVER_TARGET)

-include $(DEP_FILES)

ALL_GENERATED_DIRS := $(COMMON_OBJ_DIR) $(CONSOLE_OBJ_DIR) $(SERVER_OBJ_DIR)
ALL_GENERATED_DIRS := $(COMMON_DEP_DIR) $(CONSOLE_DEP_DIR) $(SERVER_DEP_DIR)
ALL_GENERATED_DIRS := $(BIN_DIR) obj



########################################################################
#
#                     Build Rules
#
#########################################################################

.PHONY: clean
.DEFAULT_GOAL := all

all: common client server run

common: directories_common

client: directories_console $(CONSOLE_BIN_FILE)

server: directories_server $(SERVER_BIN_FILE)

run: $(CONSOLE_TARGET)

start: $(SERVER_TARGET)

# Clean
clean:
	$(RM) $(ALL_GENERATED_DIRS)

# Install
install: $(BIN_FILE)
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) $(BIN_FILE) $(PREFIX)/bin

# Rebuild
remake: clean all

########################################################################
#
#                     COMMON
#
#########################################################################

# Directories
directories_common:
	@mkdir -p $(COMMON_OBJ_DIR)
	@mkdir -p $(COMMON_DEP_DIR)
	@mkdir -p $(BIN_DIR)

# Objects
$(COMMON_OBJ_DIR)/%.o: */*/%.c
	@echo ''
	@echo ''
	@echo '---------------------- COMMON COMPILER:' $(notdir $<)
	@echo ''
	@echo ''

	$(ST_AN) $(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -c -o $@ $<



########################################################################
#
#                     CLIENT
#
#########################################################################

# 
directories_console:
	@mkdir -p $(CONSOLE_OBJ_DIR)
	@mkdir -p $(CONSOLE_DEP_DIR)

# Linker
$(CONSOLE_BIN_FILE): $(COMMON_OBJ_FILES) $(CONSOLE_OBJ_FILES) 
	@echo ''
	@echo ''
	@echo '---------------------- CONSOLE LINKER'
	@echo ''
	@echo ''
	
	$(ST_AN) $(LD) $(ALL_LDFLAGS) $^ $(ALL_LDLIBS) -o $@

# Object
$(CONSOLE_OBJ_DIR)/%.o: */*/%.c
	@echo ''
	@echo ''
	@echo '---------------------- CONSOLE COMPILER:' $(notdir $<)
	@echo ''
	@echo ''

	$(ST_AN) $(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -c -o $@ $<

# Run
$(CONSOLE_TARGET):
	$(DBG) ./$(CONSOLE_BIN_FILE)



########################################################################
#
#                     SERVER
#
#########################################################################

# 
directories_server:
	@mkdir -p $(SERVER_OBJ_DIR)
	@mkdir -p $(SERVER_DEP_DIR)

# Linker
$(SERVER_BIN_FILE): $(COMMON_OBJ_FILES) $(SERVER_OBJ_FILES)
	@echo ''
	@echo ''
	@echo '---------------------- SERVER LINKER'
	@echo ''
	@echo ''
	
	$(ST_AN) $(LD) $(ALL_LDFLAGS) $^ $(ALL_LDLIBS) -o $@

# Object
$(SERVER_OBJ_DIR)/%.o: */*/%.c
	@echo ''
	@echo ''
	@echo '---------------------- SERVER COMPILER:' $(notdir $<)
	@echo ''
	@echo ''

	$(ST_AN) $(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -c -o $@ $<

# Run
$(SERVER_TARGET):
	$(DBG) ./$(SERVER_BIN_FILE)



########################################################################
#
#                     HELP
#
#########################################################################



########################################################################
#
#                     DEBUG
#
#########################################################################

# Print Variables For Debug
REAL_CC := $(CC)
REAL_LD := $(LD)

show:
	@echo ''
	@echo 'CONSOLE_TARGET:    ' $(CONSOLE_TARGET)
	@echo 'SERVER_TARGET:     ' $(SERVER_TARGET)
	@echo 'CURDIR:            ' $(CURDIR)
	@echo '-------------------'
	@echo 'COMPILER:          ' $(REAL_CC)
	@echo 'LINKER:            ' $(REAL_LD)
	@echo 'ST_AN:             ' $(ST_AN)
	@echo 'DBG:               ' $(DBG)
	@echo 'SHELL:             ' $(SHELL)
	@echo '-------------------'
	@echo 'PREFIX:            ' $(PREFIX)
	@echo 'TARGET_ARCH:       ' $(TARGET_ARCH)
	@echo '-------------------'
	@echo 'ALL_CFLAGS:        ' $(ALL_CFLAGS)
	@echo 'ALL_CPPFLAGS:      ' $(ALL_CPPFLAGS)
	@echo 'ALL_LDFLAGS:       ' $(ALL_LDFLAGS)
	@echo 'ALL_LDLIBS:        ' $(ALL_LDLIBS)
	@echo '-------------------'
	@echo 'COMMON_SRC_DIR:    ' $(COMMON_SRC_DIR)
	@echo 'COMMON_OBJ_DIR:    ' $(COMMON_OBJ_DIR)
	@echo 'COMMON_DEP_DIR:    ' $(COMMON_DEP_DIR)
	@echo 'CONSOLE_SRC_DIR:   ' $(CONSOLE_SRC_DIR)
	@echo 'CONSOLE_OBJ_DIR:   ' $(CONSOLE_OBJ_DIR)
	@echo 'CONSOLE_DEP_DIR:   ' $(CONSOLE_DEP_DIR)
	@echo 'SERVER_SRC_DIR:    ' $(SERVER_SRC_DIR)
	@echo 'SERVER_OBJ_DIR:    ' $(SERVER_OBJ_DIR)
	@echo 'SERVER_DEP_DIR:    ' $(SERVER_DEP_DIR)
	@echo 'BIN_DIR:           ' $(BIN_DIR)
	@echo '-------------------'
	@echo 'COMMON_SRC_FILES:  ' $(COMMON_SRC_FILES)
	@echo 'COMMON_OBJ_FILES:  ' $(COMMON_OBJ_FILES)
	@echo 'COMMON_DEP_FILES:  ' $(COMMON_DEP_FILES)
	@echo 'CONSOLE_SRC_FILES: ' $(CONSOLE_SRC_FILES)
	@echo 'CONSOLE_OBJ_FILES: ' $(CONSOLE_OBJ_FILES)
	@echo 'CONSOLE_DEP_FILES: ' $(CONSOLE_DEP_FILES)
	@echo 'SERVER_SRC_FILES:  ' $(SERVER_SRC_FILES)
	@echo 'SERVER_OBJ_FILES:  ' $(SERVER_OBJ_FILES)
	@echo 'SERVER_DEP_FILES:  ' $(SERVER_DEP_FILES)
	@echo '-------------------'
	@echo 'CONSOLE_BIN_FILE:  ' $(CONSOLE_BIN_FILE)
	@echo 'SERVER_BIN_FILE:   ' $(SERVER_BIN_FILE)
	@echo ''
