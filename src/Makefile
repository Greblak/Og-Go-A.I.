CC = g++
CC_FLAGS = -g -Wall
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = oggoai

default: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)
	chmod +x $(EXEC)
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@
clean:
	rm -f $(EXEC) $(OBJECTS)