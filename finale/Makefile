CXX=g++
SOURCES=main.cpp aux.cpp io.cpp
INCLUDES=aux.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main
CFLAGS=-c -Wall -std=c++11 -O3 
TO_UPLOAD=to_upload
INPUT=task.txt
SOL=sol$(SOLN).txt
LOG=log$(SOLN).txt
DEBUG=-check

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(INCLUDES)
	$(CXX) -o $@ $(OBJECTS)

%.o: %.cpp $(INCLUDES)
	$(CXX) $(CFLAGS) $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE) $(DEBUG) < $(INPUT)

sol: $(EXECUTABLE)
	./$(EXECUTABLE) 2> $(TO_UPLOAD)/$(LOG) > $(TO_UPLOAD)/$(SOL) < $(INPUT)

clean:
	rm -rf *.dSYM $(EXECUTABLE) $(OBJECTS)
cleanall: clean
	rm -rf $(TO_UPLOAD)/*
