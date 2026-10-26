CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
TARGET = lab2.exe

SRCS = main.cpp BitSequence.cpp Tests.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	cmd /C del /f /q $(OBJS) $(TARGET) 2>nul || exit 0