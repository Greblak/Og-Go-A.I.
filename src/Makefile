CC = g++
CC_FLAGS = -g -O3 -funroll-loops
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDENCIES = $(wildcard *.d)
EXEC = ../bin/oggoai
all: default
default: $(OBJECTS)
	$(CC) $(OBJECTS) -pg -o $(EXEC)
	chmod +x $(EXEC)
-include $(OBJECTS:.o=.d)
%.o: %.cpp 
	$(CC) -c $(CC_FLAGS) $*.cpp -o $*.o
	$(CC) -MM $(CC_FLAGS) $*.cpp -o $*.d
clean:
	rm -f $(EXEC) $(OBJECTS) $(DEPENDENCIES)