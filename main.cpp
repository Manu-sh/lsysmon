#include "src/Linux.hpp"


#if 0
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

	Cpu::Details::Proc::CpuStat cpu_stat;

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


} catch (std::exception &e) {
	std::cerr << e.what() << endl;
}
#endif