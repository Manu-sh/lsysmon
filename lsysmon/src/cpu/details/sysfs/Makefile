CXXFLAGS=-O3 -pipe -ffast-math -Wall -Wextra -D_REENTRANT -std=c++17 -Wno-unused-function -fstack-protector-all -fstack-protector-strong -funroll-loops #-pedantic

.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) -o main main.cpp cpu_n/Topology.cpp #cpu_n/Freq.cpp # cpu_n/Cache.cpp 

test: all
	./main

clean:
	rm -f *.o main