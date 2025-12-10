CC = gcc

PROGRAM_NAME = main
SRC_DIR = ./src
INCLUDE_DIR = ./include
LIB_DIR = ./lib
BUILD_DIR = ./build
BIN_DIR = ./bin
LIBS_FLAGS = -lpthread
CFLAGS = -Wall -std=c23 -O0 -I$(INCLUDE_DIR) $(LIBS_FLAGS)



SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

TARGET = $(BIN_DIR)/$(PROGRAM_NAME)



all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

cleanobj:
	rm -rf $(BUILD_DIR)

.PHONY: all clean cleanobj