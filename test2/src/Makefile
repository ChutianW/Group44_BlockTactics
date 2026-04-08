# Block Tactics - Sokoban Puzzle Game
# COMP2113 / ENGG1340 Group Project
# Linux Build (CS Department Server)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = blocktactics

SRCS = main.cpp game.cpp map.cpp player.cpp file_io.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
