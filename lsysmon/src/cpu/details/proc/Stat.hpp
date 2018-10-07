#pragma once

#include <vector>

namespace Cpu::Details::Proc::Stat {

	struct Stat {

		/* [0] = cpu_tot, [1] = cpu0, [2] = cpu1 ... [size-1] = cpuX */
		virtual std::vector<float> pct_usage() const = 0;
		virtual ~Stat() = 0;
	};

}