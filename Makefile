CFLAGS=-Wall -Wextra -pedantic -march=native -std=c++11 -pthread -O3
CLFLAGS=-lOpenCl

all: spies.cpp Makefile
	$(CXX) $(CFLAGS) -g spies.cpp -o spies

profile: spies.cpp Makefile
	$(CXX) -DPROFILING=1 $(CFLAGS) -g -fprofile-generate -fprofile-correction spies.cpp -o spies-profile
	@echo "Run spies-profile and then make profiled"

profiled: spies.cpp Makefile spies.gcda
	$(CXX) $(CFLAGS) -fprofile-use -w spies.cpp -o spies
	@echo "Profile optimized build available as executable spies"

