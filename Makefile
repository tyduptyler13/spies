CFLAGS=-Wall -Wextra -pedantic -march=native -std=c++11 -g -O

all: spies.cpp Makefile
	$(CXX) $(CFLAGS) spies.cpp -o spies
