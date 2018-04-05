CXX         := -c++
CXXFLAGS    := -pedantic-errors -Wall -Wextra -Werror
LDFLAGS     := -L/usr/lib -lstdc++ -lm
BUILD       := ./build
OBJ_DIR     := $(BUILD)/objects
APP_DIR     := $(BUILD)/apps
TARGET      := program
INCLUDE     := -Iinclude/
SRC         := $(wildcard src/*.cpp)

OBJECTS     := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_SIR): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR) $(OBJECTS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir --p $(OBJ_DIR)

debug: CXXFLAGS += -DEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rf $(OBJ_DIR)/*
	-@rm -rf $(APP_DIR)/*
