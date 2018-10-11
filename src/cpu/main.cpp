#include "details/proc/CpuInfo.hpp"
#include "details/proc/Stat.hpp"

#include "details/sysfs/cpu_n/Topology.hpp"
#include "details/sysfs/cpu_n/Cache.hpp"
#include "details/sysfs/cpu_n/Freq.hpp"

using namespace Cpu::Details::Proc::CpuInfo;
using namespace Cpu::Details::Proc::Stat;
using namespace Cpu::Details::Sysfs::CpuN::Freq;
using namespace Cpu::Details::Sysfs::CpuN::Cache;
using namespace Cpu::Details::Sysfs::CpuN::Topology;
using namespace Cpu::Details;

#include "ICpu.hpp"

#include <iostream>

extern "C" {
	#include <unistd.h>
}

using namespace std;
int main() {

	const auto &stat_mon = Proc::Stat::get_cpu_stat();
	for (;; sleep(1)) cout << *Cpu::get_single_sk_cpu(stat_mon) << endl;

}
