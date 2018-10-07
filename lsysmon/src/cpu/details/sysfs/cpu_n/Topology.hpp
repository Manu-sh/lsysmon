#pragma once
#include <cstdint>
#include <ostream>

/* https://www.kernel.org/doc/Documentation/cputopology.txt */
namespace Cpu::Details::Sysfs::CpuN::Topology {	

	struct Topology {
		explicit Topology(uint8_t cpu_n);
		uint8_t physical_package_id; /* Typically corresponds to a physical socket number */
	};

	std::ostream & operator<<(std::ostream &os, const Topology &t);
}
