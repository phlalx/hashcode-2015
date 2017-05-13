CXX=g++

all: main

main: main.cpp aux.cpp aux.h io.cpp
	$(CXX)  -Wall -std=c++11 -g -o main main.cpp aux.cpp io.cpp

opt:
	g++ -o main main.cpp aux.cpp io.cpp -Wall -std=c++0x -O3

run: main 
	cat task.txt | ./main -check

sol: main
	cat task.txt | ./main > sol.txt

test: main
	scripts/runtests.sh main tests

clean:
	rm -rf *.dSYM main
