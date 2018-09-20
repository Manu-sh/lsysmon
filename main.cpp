#include "src/Linux.hpp"

extern "C" {
	#include <unistd.h>
}


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

int main() try {

#if 0
	Linux::Sysinfo sys;
	cout << sys.get_used_ram() << endl;
	cout << sys.get_free_ram() << endl;
#endif

#if 1
	Proc::CpuStat cpu_stat;

	while (1) {

		const auto &usage = cpu_stat.pct_usage();
		const uint8_t ncpu = (uint8_t)usage.size() - 1; // first field is the total cpus usage cpu_n = (size-1)
		std::vector<SingleSocket::Core> core;

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

#if 0

	while (1) {

		system("clear");
		cout << "CURRENT PID: " << getpid() << endl;

		const Linux::Sysinfo sysinfo;
		const Proc::CpuInfo cpuinfo;

		cout << "ram:" << endl;
		cout << "\ttotal " << sysinfo.get_total_ram() << endl;
		cout << "\tusage " << sysinfo.get_used_ram()  << endl;
		cout << "\tfree "  << sysinfo.get_free_ram()  << endl;
		cout << "uptime: " << sysinfo.get_uptime()    << endl;

		{
			const int cpu_n = 0;
			const uint8_t cache_idx = (uint8_t)utils::Linux::glob(
				"/sys/devices/system/cpu/cpu" 
				+ std::to_string(cpu_n) +
				"/cache/index[0-9]*"
			).size();

			for (uint_fast8_t j = UINT8_C(0); j < cache_idx; j++) {
				Sysfs::CpuN::Cache::Cache cache(cpu_n, j);
				cout << cache << endl << endl;
			}
		}

		const auto &v = cpu.pct_usage();

		// TODO progress-bar decente
		for (unsigned i = 1; i < v.size() - 1; i++) {

			const unsigned char pct = v[i];
			cout << "core [" <<i << "] [";
			for (int i = 0; i < pct; i++) cout << "#";
			for (int i = pct; i < 100; i++) cout << "_";
			cout << "] " << units::to_string(v[i], 2) << '%' << "\n";

			Sysfs::CpuN::Freq::Freq freq(i);
			cout << freq << endl;
		}

		cout << "CPU: " << units::to_string(v[0], 2) << '%' << "\n";
		sleep(1);

		/*
		const auto &info = Linux::Uname();

		cout 
			// << info.domainname << " " << info.nodename << " "
			<< info.sysname << " " << info.machine << " " 
		     	<< info.release << " " << info.version <<
		endl;
		*/
	}

	cout << endl;
#endif

} catch (std::exception &e) {

	std::cerr << e.what() << endl;

}
