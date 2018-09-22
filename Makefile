CXXFLAGS=-O3 -pipe -ffast-math -Wall -Wextra -D_REENTRANT -std=c++17 -Wno-unused-function -fstack-protector-all -fstack-protector-strong -funroll-loops #-pedantic
#LDLIBS=-lstdc++fs

.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) -o main main.cpp $(LDLIBS)

test: all
	./main

clean:
	rm -f *.o main
