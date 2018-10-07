#pragma once
#include <cstdint>
#include <ostream>

/* https://www.kernel.org/doc/Documentation/cputopology.txt */
namespace Cpu::Details::Sysfs::CpuN::Topology {	

	struct Topology {

		friend Topology get_topology(uint8_t cpu_n);

		uint8_t physical_package_id; /* Typically corresponds to a physical socket number */

		private:
			Topology() = default;
	};
	
	std::ostream & operator<<(std::ostream &os, const Topology &t);
	bool operator==(const Topology &a, const Topology &b);
	bool operator!=(const Topology &a, const Topology &b);
	Topology get_topology(uint8_t cpu_n);

}