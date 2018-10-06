#pragma once

#include "details/SingleSocket.hpp"

namespace Cpu {


	class Cpu {};

};


#if 0
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