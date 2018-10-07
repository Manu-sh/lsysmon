#pragma once
#include <cstdint>
#include <ostream>

/* https://www.kernel.org/doc/Documentation/cpu-freq/user-guide.txt */
namespace Cpu::Details::Sysfs::CpuN::Freq {	

	struct Freq {

		uint32_t cpuinfo_min_freq; /* minimum operating frequency KHz */
		uint32_t cpuinfo_max_freq; /* maximum operating frequency KHz */
		uint32_t scaling_cur_freq; /* current cpu frequency determined by the governor in KHz,
					      when this file dosn't exist the value come from cpuinfo_cur_freq */
	};

	std::ostream & operator<<(std::ostream &os, const Freq &f);
	Freq get_freq(uint8_t cpu_n);
}