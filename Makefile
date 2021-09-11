SRC_DIR := src

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp or $(SRC_DIR)/glad/*.c or $(SRC_DIR)/stb/*.cpp)
TARGET := output

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
# -ffast-math -O3 # optimization
#CXX_FLAGS = -g -Wall -Werror -Wpedantic -pedantic -Wextra -lglfw -lGL -lGLU -ldl

CXX_FLAGS = -O3 -ffast-math -lglfw -lGL -lGLU -ldl

CXX = g++

.DEFAULT_GOAL := default
.PHONY: default
default:
	$(CXX) $(CXX_FLAGS) $(LDFLAGS) $(SRC_FILES) -o $(TARGET)

.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET)
