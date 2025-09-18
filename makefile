# From: https://www.incredibuild.com/blog/how-to-create-a-g-makefile
CXX = g++
CXXFLAGS = -std=c++23 -Wall -O2

TARGET = myprogram
OBJS = main.o

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
