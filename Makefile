CFLAGS=-Wall -Wextra -pedantic -march=native -std=c++11 -pthread -g -O3

all: spies.cpp Makefile
	$(CXX) $(CFLAGS) spies.cpp -o spies
