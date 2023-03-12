BINARY := rhythm_game
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

SOURCES := main.cpp renderer.cpp camera.cpp game.cpp gl/buffer.cpp gl/vao.cpp shaders.cpp gl/program.cpp
DEPFILES := $(SOURCES:%.cpp=$(BUILD_DIR)/%.d)

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -Iinclude -std=c++17 -DDEBUG `pkg-config --cflags sdl2` 
LDFLAGS = 
LDLIBS = `pkg-config --libs sdl2` -lglbinding

.PHONY: clean build run

build: $(BUILD_DIR)/$(BINARY)

clean:
	$(RM) $(BUILD_DIR)/*.o
	$(RM) $(BUILD_DIR)/*.d
	$(RM) $(BUILD_DIR)/gl/*.o
	$(RM) $(BUILD_DIR)/gl/*.d
	$(RM) $(BUILD_DIR)/$(BINARY)

run: build
	@./$(BUILD_DIR)/$(BINARY)

$(BUILD_DIR)/$(BINARY): $(SOURCES:%.cpp=$(BUILD_DIR)/%.o) 
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(BUILD_DIR)/%.d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MT $@ -MMD -MP -MF $(BUILD_DIR)/$*.d -c -o $@ $<

$(DEPFILES):
include $(wildcard $(DEPFILES))
