#pragma once

#include <vector>
#include <memory>

namespace Cpu::Details::Proc::Stat {

	struct Stat {

		/* [0] = cpu_tot, [1] = cpu0, [2] = cpu1 ... [size-1] = cpuX */
		virtual std::vector<float> pct_usage() const = 0;
		virtual ~Stat() = default;
	};

	std::unique_ptr<Stat> get_cpu_stat();
}