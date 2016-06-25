CFLAGS=-Wall -Wextra -pedantic -O3 -march=native -std=c++11

all:
	$(CC) $(CFLAGS) spies.cpp -o spies
