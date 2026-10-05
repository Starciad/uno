# Variables
CC = gcc
CFLAGS = -Wall -Werror -g -Iinclude -std=c11 -pedantic-errors
BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
INCLUDE_DIR = include

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Targets and rules
all: $(BIN_DIR)/uno

$(BIN_DIR)/uno: $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

help:
	@echo "Makefile for Uno Project"
	@echo "Targets:"
	@echo "  all      - Compile and link the Uno program"
	@echo "  clean    - Remove compiled files"
	@echo "  help     - Show this help message"

.PHONY: all clean help
