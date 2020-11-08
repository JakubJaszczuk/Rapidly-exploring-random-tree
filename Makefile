CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -march=native -std=c++17
LDFLAGS = -lGLEW -lGL -lsfml-system -lsfml-window
TARGET = rrt
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DBJS = $(SRCS:.cpp=.d)

.PHONY: all
all: $(TARGET)

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< > $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

-include $(DBJS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) $(DBJS)

.PHONY: run
run:
	./$(TARGET)
