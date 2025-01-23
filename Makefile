# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`
# OUTPUT_DIR = bin
SRC_DIR = src

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/magazyn.c $(SRC_DIR)/gui.c
OBJS = $(SRCS:.c=.o)

# Output binary
# TARGET = $(OUTPUT_DIR)/magazyn_app
TARGET = magazyn_app

# Default rule
# all: directories $(TARGET)

# Compile the application
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create necessary directories
# directories:
# 	mkdir -p $(OUTPUT_DIR)

# Clean compiled files
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

# Run the application
run: all
	./$(TARGET)
