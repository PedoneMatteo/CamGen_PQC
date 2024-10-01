# Name of the executable
TARGET = benchmark

# Compiler
CC = gcc

# Compilation flags
CFLAGS = -g -Wall

# Include directories (note the -I before the path)
INCLUDES = -I/root/Project_FsMsGen/liboqs/build/include

# Required libraries
LIBS = -lssl -lcrypto
LIBS += -L/root/Project_FsMsGen/liboqs/build/lib -loqs

# Source file
SRC = benchmark.c

# Output directory
OUTDIR = build

# Command to create the directory if it doesn't exist
MKDIR_P = mkdir -p

# Default rule
all: $(OUTDIR) $(OUTDIR)/$(TARGET)

# Rule to create the executable
$(OUTDIR)/$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(OUTDIR)/$(TARGET) $(LIBS)

# Rule to create the output directory
$(OUTDIR):
	$(MKDIR_P) $(OUTDIR)

# Rule to clean generated files
clean:
	rm -rf $(OUTDIR)
