# Compiler and flags
CC = gcc
CFLAGS = -g
LDFLAGS = -lm

# Targets
TARGETS = sfl

# Default target
all: $(TARGETS)

# Build the main target
sfl: sfl.c alloclist.o list.o
	$(CC) $(CFLAGS) sfl.c alloclist.o list.o -o sfl $(LDFLAGS)

# Compile the object files
alloclist.o: alloclist.c alloclist.h
	$(CC) $(CFLAGS) -c alloclist.c -o alloclist.o

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c -o list.o

# Run the executable
run: sfl
	./sfl

# Clean up build artifacts
clean:
	rm -f $(TARGETS) *.o

