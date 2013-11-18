CC=g++
CFLAGS=-c -Wall -fopenmp
SOURCES=licsim_example.c++ LightCurve.c++
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=licsim_example

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)
