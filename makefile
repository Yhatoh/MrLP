CC = g++
CFLAGS = -Wall -I./include

SRC_DIR = src
BUILD_DIR = build
TEST_DIR = test

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

EXEC = myprogram
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_EXECS = $(patsubst $(TEST_DIR)/%.cpp, $(TEST_DIR)/%, $(TEST_SOURCES))

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJECTS)
		$(CC) $(OBJECTS) -o $(EXEC)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
		$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/%: $(TEST_DIR)/%.cpp $(OBJECTS)
		$(CC) $(CFLAGS) $< $(OBJECTS) -o $@

test: $(TEST_EXECS)
		for test in $(TEST_EXECS); do ./$$test; done

clean:
		rm -f $(EXEC) $(OBJECTS)

