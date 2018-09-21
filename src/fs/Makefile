CXXFLAGS=-O3 -pipe -ffast-math -Wall -Wextra -D_REENTRANT -std=c++17 -Wno-unused-function -fstack-protector-all -fstack-protector-strong -funroll-loops # -pedantic
LDLIBS=-lpthread -lstdc++fs

.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) -o fsmain fsmain.cpp $(LDLIBS)

test: all
	./fsmain

clean:
	rm -f *.o fsmain
