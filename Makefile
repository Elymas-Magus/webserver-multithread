# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -fexceptions -g -Iincludes
SRC = src
OBJ = obj
BIN = bin

TARGET = main

C_FILENAMES = utils.c server.c connection.c validation.c threadpool.c list.c queue.c request.c throwable.c throws.c timer.c uri.c stream.c
OBJ_FILES = $(patsubst %.c, $(OBJ)/%.o, $(C_FILENAMES) )

all: $(TARGET)

$(OBJ)/$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET): $(BIN) $(OBJ) clean $(OBJ)/$(TARGET).o $(OBJ_FILES) 
	$(CC) -o $(BIN)/$(TARGET) $(OBJ)/$(TARGET).o $(OBJ_FILES) 

$(OBJ):
	mkdir -p $@

$(BIN):
	mkdir -p $@

clean:
	$(RM) -rf $(BIN)/$(TARGET)
	$(RM) -rf $(OBJ)/*.o