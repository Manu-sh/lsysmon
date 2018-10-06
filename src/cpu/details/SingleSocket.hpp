#pragma once

#include "proc/ProcCpuInfo.hpp"
#include "proc/ProcCpuStat.hpp"
#include "sysfs/Sysfs.hpp"

#include <cassert>
#include <memory>
#include <vector>
#include <mutex>

namespace Cpu::Details::SingleSocket {

	using Sysfs::CpuN::Cache::Cache;
	using Sysfs::CpuN::Topology::Topology;
	using Sysfs::CpuN::Freq::Freq;

	class Core: Freq, Topology {

		public:
			friend std::ostream & operator<<(std::ostream &os, const SingleSocket::Core &c) {

				using namespace units;

				return 	os <<
					(Topology &)c          << "\n" <<
					"scaling_cur_freq:   " << Hz::to_si( kHz::to_Hz(c.scaling_cur_freq) ) << "\n"
					"core_min_freq:      " << Hz::to_si( kHz::to_Hz(c.cpuinfo_min_freq) ) << "\n"
					"core_max_freq:      " << Hz::to_si( kHz::to_Hz(c.cpuinfo_max_freq) ) << "\n"
					"cpu_model_name:     " << Core::cpu_model_name     << "\n" <<
					"cpu_virtualization: " << Core::cpu_virtualization << "\n" <<
					"cores:              " << (short)Core::cpu_core    << "\n" <<
					"threads per core:   " << (short)Core::cpu_thread  << "\n"
				;
			}

			explicit Core(uint8_t cpu_n);

		private:
			static std::mutex mut;
			static uint8_t once_flag;

			static std::vector<Cache> cpu_cache;
			static uint8_t cpu_core;
			static uint8_t cpu_thread;
			static std::string cpu_model_name;
			static std::string cpu_virtualization;
	};

	std::mutex Core::mut;
	uint8_t Core::once_flag;

	std::vector<Cache> Core::cpu_cache;
	std::string Core::cpu_model_name;
	std::string Core::cpu_virtualization = "NONE/UNKNOW";
	uint8_t Core::cpu_core;
	uint8_t Core::cpu_thread;

}

Cpu::Details::SingleSocket::Core::Core(uint8_t cpu_n): Freq{cpu_n}, Topology{cpu_n} { /* for slightly better reliability freq. is first */

	constexpr uint8_t CPU_N = UINT8_C(0); /* physical cpu id */

	if (this->physical_package_id != CPU_N)
		THROW_UNIMPL_EX("Multiple cpu systems aren't supported: ");

	{ /* execute once: get cpu_model_name, cpu_cache, cpu_physical_core_n, cpu_logical_core_n */
		std::lock_guard<std::mutex> glock(Core::mut);

		if (Core::once_flag == UINT8_C(0)) {

			const uint8_t cache_idx = (uint8_t)utils::Linux::glob(
				"/sys/devices/system/cpu/cpu" 
				+ std::to_string(CPU_N) +
				"/cache/index[0-9]*"
			).size();

			for (uint_fast8_t j = UINT8_C(0); j < cache_idx; j++)
				Core::cpu_cache.emplace_back(CPU_N, j);

			Proc::CpuInfo cpu_info;
			Core::cpu_core         = (uint8_t)std::stoi(cpu_info.vct[CPU_N].entries[Proc::CPU_CORES]);
			Core::cpu_thread       = (uint8_t)std::stoi(cpu_info.vct[CPU_N].entries[Proc::SIBILINGS]) / Core::cpu_core;
			Core::cpu_model_name   = std::move(cpu_info.vct[CPU_N].entries[Proc::MODEL_NAME]);

			const auto &flags = cpu_info.vct[CPU_N].entries[Proc::FLAGS];
			
			if (flags.find("svm") != std::string::npos)
				Core::cpu_virtualization = "AMD-V";
			else if (flags.find("vmx") != std::string::npos)
				Core::cpu_virtualization = "INTEL VT-x";

			Core::once_flag = UINT8_C(1);
		}

	}

}