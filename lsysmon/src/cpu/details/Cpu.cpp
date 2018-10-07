#include "Cpu.hpp"

#include "sysfs/cpu_n/Topology.hpp"
#include "sysfs/cpu_n/Freq.hpp"
#include "sysfs/cpu_n/Cache.hpp"
#include <unordered_map>

using namespace Cpu::Details::Cpu;
using namespace Cpu::Details::Sysfs::CpuN::Topology;
using namespace Cpu::Details::Sysfs::CpuN::Freq;

using Stat  = typename MCpu::Stat;
using Cache = typename MCpu::Cache;