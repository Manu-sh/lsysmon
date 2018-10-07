#pragma once

#include <vector>
#include <string>

namespace Cpu::Details::Proc::CpuInfo {

	/* they are guaranteed to be existing and non-empty */

	enum {
		PROCESSOR,
		VENDOR_ID,
		CPU_FAMILY,
		MODEL,
		PHYSICAL_ID,
		SIBILINGS,
		CORE_ID,
		CPU_CORES,
		CPU_MHZ,
		CACHE_SIZE,
		MODEL_NAME,
		FLAGS,

		ARRAY_LENGTH /* LENGTH = N + 1 */
	};

	struct CpuEntry {
		std::vector<std::string> entries;
		private: 
			CpuEntry() = default;
	};

	std::ostream & operator<<(std::ostream &os, const CpuEntry &ref);
	std::vector<CpuEntry> get_cpu_info();

}