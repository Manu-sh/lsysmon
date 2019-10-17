#include "src/Linux.hpp"
#include "src/utils/utils_string.hpp"

#include <iostream>
#include <stdexcept>

extern "C" {
	#include <unistd.h>
}

using namespace std;

template <typename Ch>
static void progress(uint_fast32_t width, uint_fast32_t pct_progress, std::ostream_iterator<Ch> it, Ch blank, Ch fill, Ch oframe = '[', Ch cframe = ']') {

	pct_progress *= (uint_fast32_t) width / 100;
	*it = oframe; ++it;

	for (uint_fast32_t i = 0; i <= pct_progress; ++i, ++it)
		*it = fill;

	for (--width; pct_progress < width; ++pct_progress, ++it)
		*it = blank;

	*it = cframe; ++it;
}


#include "src/utils/units.hpp"
#include "src/cpu/details/proc/CpuInfo.hpp"
#include "src/cpu/details/proc/Stat.hpp"

#include "src/cpu/details/sysfs/cpu_n/Topology.hpp"
#include "src/cpu/details/sysfs/cpu_n/Cache.hpp"
#include "src/cpu/details/sysfs/cpu_n/Freq.hpp"

using namespace Cpu::Details::Proc::CpuInfo;
using namespace Cpu::Details::Proc::Stat;
using namespace Cpu::Details::Sysfs::CpuN::Freq;
using namespace Cpu::Details::Sysfs::CpuN::Cache;
using namespace Cpu::Details::Sysfs::CpuN::Topology;
using namespace Cpu::Details;

#include "src/cpu/ICpu.hpp"

#include <iostream>

extern "C" {
	#include <unistd.h>
}

using namespace std;
int main() try {

	for (;; sleep(2)) {

// TODO: im not sure that work properly

		const auto &stat_mon = Proc::Stat::get_cpu_stat();

		auto ptr = Cpu::get_single_sk_cpu(stat_mon);	
		const auto &usage = stat_mon->pct_usage();
		const auto ncpu = usage.size();

		// cout << *ptr << endl;

		for (uint_fast8_t i = UINT8_C(0); i < ncpu; i++) {
			progress(100, usage.at(i), std::ostream_iterator<char>(cout), '_', '#');
			cout << units::to_string(usage.at(i), 2) << '%' << "\n";
		}

	}

} catch (exception &e) {
	::cerr << e.what() << endl;
}


	// cout << utils::String::map_file( std::string("/home/user/.bashrc") )<< endl;

	//const auto ptr = Cpu::get_single_sk_cpu();

#if 0
	const auto &cpu_stat = Cpu::Details::Proc::Stat::get_cpu_stat();

	while (1) {

		const auto &usage = cpu_stat.pct_usage();
		const uint8_t ncpu = (uint8_t)usage.size() - 1; // first field is the total cpus usage cpu_n = (size-1)
		std::vector<Cpu::Details::SingleSocket::Core> core;

		// TODO controllare thread_per_core
		for (uint_fast8_t i = UINT8_C(0); i < ncpu; i++) {
			core.emplace_back(i);
			cout << core[i] << "\n";
			progress(100, usage.at(i+1), std::ostream_iterator<char>(cout), '_', '#');
			cout << units::to_string(usage.at(i+1), 2) << '%' << "\n";
		}

		sleep(1);

	}
#endif
