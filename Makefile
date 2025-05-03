CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
SFML_PATH = code
INCLUDE_DIRS = -I$(SFML_PATH)/include
LIB_DIRS = -L$(SFML_PATH)/lib
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

TARGET = $(BIN_DIR)/app.exe

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

.PHONY: all clean run

all: dirs $(TARGET)

dirs:
	-mkdir $(BUILD_DIR) 2>NUL
	-mkdir $(BIN_DIR) 2>NUL

$(TARGET): $(OBJ_FILES)
	$(CC) $^ -o $@ $(LIB_DIRS) $(SFML_LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

run: all
	$(TARGET)

clean:
	-rmdir /S /Q $(BUILD_DIR) 2>NUL
	-rmdir /S /Q $(BIN_DIR) 2>NUL 