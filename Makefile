CFLAGS=-Wall -Wextra -pedantic -march=native -std=c++11 -O3 -pthread

all: spies.cpp Makefile
	$(CXX) $(CFLAGS) spies.cpp -o spies
