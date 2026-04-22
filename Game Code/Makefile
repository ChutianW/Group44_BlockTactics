# Block Tactics - Sokoban Puzzle Game
# COMP2113 / ENGG1340 Group Project

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = blocktactics
TARGET_WIN = blocktactics.exe

SRCS = main.cpp map.cpp player.cpp file_io.cpp renderer.cpp game.cpp terminal.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

windows: $(TARGET_WIN)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(TARGET_WIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_WIN) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET_WIN)

run: $(TARGET)
	./$(TARGET)

.PHONY: all windows clean run
