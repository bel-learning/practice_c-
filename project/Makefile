CC = g++
CFLAGS = -std=c++17 -Wall -g -fsanitize=address 

SRC_DIR = src
BUILD_DIR = build

# Recursively find all directories in the source directory
SRC_DIRS = $(shell find $(SRC_DIR) -type d)

# Create a list of source files from the identified directories
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# Create a corresponding list of object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Specify the target executable
TARGET = baljibil

all: compile

compile: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -lncurses
	
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

$(BUILD_DIR):
	mkdir -p $(dir $(OBJS))


run:
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -rf doc/
doc:
	doxygen Doxyfile 
	mv doc/html/* doc/
