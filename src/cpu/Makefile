CXXFLAGS=-O3 -pipe -ffast-math -Wall -Wextra -D_REENTRANT -std=c++17 -Wno-unused-function -fstack-protector-all -fstack-protector-strong -funroll-loops

all:
	# make -C ../utils
	$(CXX) $(CXXFLAGS) -o main main.cpp details/Cpu.cpp details/proc/CpuInfo.cpp details/proc/Stat.cpp details/sysfs/cpu_n/Cache.cpp details/sysfs/cpu_n/Topology.cpp details/sysfs/cpu_n/Freq.cpp lutils.a
