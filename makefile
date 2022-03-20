include makefiles/functions.mk

SRC_DIR=src
OBJ_DIR=obj
LIB_DIR=lib
BIN_DIR=bin

LIBS=$(call map_src_to_desc,$(LIB_DIR),$(LIB_DIR))
OBJS=$(call map_src_to_desc,$(SRC_DIR),$(OBJ_DIR))
BIN=$(BIN_DIR)/mem_test

CC=gcc
CFLAGS=-g -std=c99 -Wall -Wno-deprecated-declarations -I$(LIB_DIR)

all:$(BIN)

$(BIN): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $(BIN)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/* $(LIB_DIR)/*.o

build:
	@make

rebuild:
	@make clean
	@make build

run: 
	./$(BIN)
	

