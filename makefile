# From: https://www.incredibuild.com/blog/how-to-create-a-g-makefile
CXX = g++
CXXFLAGS = -std=c++23 -Wall -O2

TARGET = open_render
OBJS = main.o

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS) *.png

push:
	make clean
	git commit -a -m "${M}"
	git push