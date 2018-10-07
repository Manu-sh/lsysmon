CXXFLAGS=-O3 -pipe -ffast-math -Wall -Wextra -D_REENTRANT -std=c++17 -Wno-unused-function -fstack-protector-all -fstack-protector-strong -funroll-loops -pedantic
.PHONY: all clean

lutils.a: lutils.a(*.o)
	rm *.o

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $^

clean:
	rm -f *.o *.a
