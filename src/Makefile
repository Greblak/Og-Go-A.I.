CC = g++
#-g -pg
CC_FLAGS = -g #-O3  #-fno-rtti -funroll-loops 
LINKS = -pthread -lboost_thread -lboost_system
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDENCIES = $(wildcard *.d)
EXEC = ../bin/oggoai
all: default
default: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LINKS)
	chmod +x $(EXEC)
-include $(OBJECTS:.o=.d)
%.o: %.cpp 
	$(CC) -c $(CC_FLAGS) $*.cpp -o $*.o 
	$(CC) -MM $(CC_FLAGS) $*.cpp -o $*.d 
clean:
	rm -f $(EXEC) $(OBJECTS) $(DEPENDENCIES)