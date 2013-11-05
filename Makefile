CC=g++
CFLAGS=-c -Wall -fopenmp
#LDFLAGS=-fopenmp
SOURCES=licsim_class.c++ LightCurve.c++
#SOURCES=licsim.c++ gaussian_process.c++ transfer_convolutions.c++
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=licsim_class

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)