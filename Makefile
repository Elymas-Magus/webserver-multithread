# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -fexceptions -g -Iincludes
SRC = src
OBJ = obj
BIN = bin

# the build target executable:
TARGET = main
UTILS_MOD = utils
SERVER_MOD = server
CONNECTION_MOD = connection
VALIDATION_MOD = socket_validation
THREADPOOL_MOD = threadpool
LIST_MOD = list
QUEUE_MOD = queue

all: $(TARGET)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET): $(TARGET).c $(OBJ)
	$(CC) $(CFLAGS) -c $(TARGET).c -o $(OBJ)/$(TARGET).o
	$(CC) $(CFLAGS) -c $(SRC)/$(VALIDATION_MOD).c -o $(OBJ)/$(VALIDATION_MOD).o
	$(CC) $(CFLAGS) -c $(SRC)/$(LIST_MOD).c -o $(OBJ)/$(LIST_MOD).o
	$(CC) $(CFLAGS) -c $(SRC)/$(QUEUE_MOD).c -o $(OBJ)/$(QUEUE_MOD).o
	$(CC) $(CFLAGS) -c $(SRC)/$(UTILS_MOD).c -o $(OBJ)/$(UTILS_MOD).o
	$(CC) $(CFLAGS) -c $(SRC)/$(SERVER_MOD).c -o $(OBJ)/$(SERVER_MOD).o
	$(CC) $(CFLAGS) -c $(SRC)/$(THREADPOOL_MOD).c -o $(OBJ)/$(THREADPOOL_MOD).o
	$(CC) $(CFLAGS) -c $(SRC)/$(CONNECTION_MOD).c -o $(OBJ)/$(CONNECTION_MOD).o
	$(CC) -o $(BIN)/$(TARGET) $(OBJ)/$(TARGET).o $(OBJ)/$(UTILS_MOD).o $(OBJ)/$(SERVER_MOD).o $(OBJ)/$(VALIDATION_MOD).o $(OBJ)/$(CONNECTION_MOD).o $(OBJ)/$(THREADPOOL_MOD).o $(OBJ)/$(LIST_MOD).o $(OBJ)/$(QUEUE_MOD).o

$(OBJ):
	mkdir -p obj

run: clean $(TARGET)
	bin/main

clean:
	$(RM) -rf $(BIN)/$(TARGET)
	$(RM) -rf $(OBJ)/*.o