CC=g++
CFLAGS=-c -Wall -fopenmp
#LDFLAGS=-fopenmp
SOURCES=licsim.c++ GaussianProcess.c++
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=licsim

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)