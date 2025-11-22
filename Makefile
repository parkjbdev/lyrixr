# Generic Makefile for C Project
# Folders: src/ for .c files, include/ for .h files, build/ for objects and binary

TARGET := app
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

CFLAGS := -std=c11 -Wall -Wextra -g
CFLAGS += -I$(INC_DIR)
LDFLAGS :=

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $(BUILD_DIR)/$(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(BUILD_DIR)/$(TARGET)

.PHONY: all clean run
