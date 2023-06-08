CC = g++
CFLAGS = -std=c++11 -Wall

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

all: test compile

compile: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses 

$(BUILD_DIR):
	mkdir -p $(dir $(OBJS))

test:	
	$(CC) $(CFLAGS) ./examples/unit.test.cpp ./src/datetime.cpp ./src/util.cpp -lncurses -o test
	./test
	rm -rf test
	@echo "\033[32mTest success\033[0m"
run:
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -rf doc/
doc:
	doxygen Doxyfile 
	
valgrind:
	valgrind -s --leak-check=full --show-leak-kinds=all ./baljibil