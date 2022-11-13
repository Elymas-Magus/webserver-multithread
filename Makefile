# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -fexceptions -g -Iincludes
SRC = src
DEBUG = obj/Debug
BIN = bin

# the build target executable:
TARGET = main
UTILS_MOD = utils
SERVER_MOD = server
CONNECTION_MOD = connection
VALIDATION_MOD = socket_validation

all: $(TARGET)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c -o $(DEBUG)/$(TARGET).o
	$(CC) $(CFLAGS) -c $(SRC)/$(VALIDATION_MOD).c -o $(DEBUG)/$(VALIDATION_MOD).o
	$(CC) $(CFLAGS) -c $(SRC)/$(UTILS_MOD).c -o $(DEBUG)/$(UTILS_MOD).o
	$(CC) $(CFLAGS) -c $(SRC)/$(SERVER_MOD).c -o $(DEBUG)/$(SERVER_MOD).o
	$(CC) $(CFLAGS) -c $(SRC)/$(CONNECTION_MOD).c -o $(DEBUG)/$(CONNECTION_MOD).o
	$(CC) -o $(BIN)/$(TARGET) $(DEBUG)/$(TARGET).o $(DEBUG)/$(SERVER_MOD).o $(DEBUG)/$(CONNECTION_MOD).o $(DEBUG)/$(VALIDATION_MOD).o $(DEBUG)/$(UTILS_MOD).o

clean:
	$(RM) $(TARGET)