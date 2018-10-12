CXXFLAGS=-O3 -pipe -ffast-math -Wall -Wextra -D_REENTRANT -std=c++17 -Wno-unused-function -fstack-protector-all -fstack-protector-strong -funroll-loops
.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) -c details/Cpu.cpp details/proc/Stat.cpp details/proc/CpuInfo.cpp details/sysfs/cpu_n/Cache.cpp details/sysfs/cpu_n/Topology.cpp details/sysfs/cpu_n/Freq.cpp details/sysfs/cpu_n/Cache.cpp
	$(AR) -rv lcpu.a *.o

	rm *.o
	$(CXX) $(CXXFLAGS) -o main main.cpp lcpu.a ../utils/lutils.a

clean:
	rm -f *.o lcpu.a
