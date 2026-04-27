# Block Tactics - Sokoban Puzzle Game
# COMP2113 / ENGG1340 Group Project

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = blocktactics.exe

SRCS = main.cpp map.cpp player.cpp file_io.cpp renderer.cpp game.cpp terminal.cpp
OBJS = $(SRCS:.cpp=.o)

ifeq ($(OS),Windows_NT)
RUN_CMD = .\\$(TARGET)
CLEAN_CMD = del /Q /F $(subst /,\\,$(OBJS)) $(TARGET) 2>NUL || exit 0
else
RUN_CMD = ./$(TARGET)
CLEAN_CMD = rm -f $(OBJS) $(TARGET)
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(CLEAN_CMD)

run: $(TARGET)
	$(RUN_CMD)

.PHONY: all clean run
