BINARY := rhythm_game
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

SOURCES := $(shell find src/ -name *.cpp | cut -c 5-)
DEPFILES := $(SOURCES:%.cpp=$(BUILD_DIR)/%.d)

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -Iinclude -std=c++17 -DDEBUG -D_REENTRANT
LDFLAGS = 
LDLIBS = -lSDL2 -lglbinding

.PHONY: clean build run

build: $(BUILD_DIR)/$(BINARY)

clean:
	$(RM) $(BUILD_DIR)/*.o
	$(RM) $(BUILD_DIR)/*.d
	$(RM) $(BUILD_DIR)/gl/*.o
	$(RM) $(BUILD_DIR)/gl/*.d
	$(RM) $(BUILD_DIR)/graphics/*.o
	$(RM) $(BUILD_DIR)/graphics/*.d
	$(RM) $(BUILD_DIR)/$(BINARY)

run: build
	@./$(BUILD_DIR)/$(BINARY)

$(BUILD_DIR)/$(BINARY): $(SOURCES:%.cpp=$(BUILD_DIR)/%.o) 
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(BUILD_DIR)/%.d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MT $@ -MMD -MP -MF $(BUILD_DIR)/$*.d -c -o $@ $<

$(DEPFILES):
include $(wildcard $(DEPFILES))
