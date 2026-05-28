# ==============================================================================
# PROFESSIONAL KEY-VALUE DATABASE MAKEFILE (MinGW Compatible)
# ==============================================================================

# Compiler definition
CC = gcc

# Compiler flags for warnings and robust compilation
CFLAGS = -Wall -Wextra

# Target executable name
TARGET = db.exe

# Source files and Object files
SRCS = main.c parser.c database.c storage.c
OBJS = $(SRCS:.c=.o)

# Default target: Compile standard build
all: $(TARGET)

# Linking step: Combines all object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation step: Compiles each .c file into a .o file
# $< is the first prerequisite (.c file), $@ is the target (.o file)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Debug target: Includes debugging symbols (-g) and the DEBUG macro
# This allows verbose printing during runtime for educational/debugging purposes
debug: CFLAGS += -g -DDEBUG
debug: clean all

# Clean target (MinGW Compatible)
# Uses 'del' instead of Linux 'rm' to remove compiled objects and database dumps
clean:
	del /Q /F $(OBJS) $(TARGET) database.dat 2>nul || exit 0
