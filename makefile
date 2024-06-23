# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -Wextra -g
# Libraries
LIBS = 

# Source files directory
SRC_DIR = src
# Object files directory
BUILD_DIR = build

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# List of object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
# Output executable
TARGET = main

# Rule to make the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to compile each source file into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Target to run the executable
run: $(TARGET)
	./$(TARGET)

# Clean up intermediate build files
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

# Phony targets
.PHONY: run clean

