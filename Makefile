# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Source and header files
SRC = front.c parser.c
HEADERS = parser.h front.h

# Output executable
TARGET = cooke_parser

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up the executable
clean:
	rm -f $(TARGET)
