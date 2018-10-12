CXXFLAGS=-O3 -pipe -ffast-math -Wall -Wextra -D_REENTRANT -std=c++17 -Wno-unused-function -fstack-protector-all -fstack-protector-strong -funroll-loops
LDLIBS=-lpthread -lstdc++fs

all:
	$(CXX) $(CXXFLAGS) -o main main.cpp details/proc/MemInfo.cpp ../utils/lutils.a $(LDLIBS)
