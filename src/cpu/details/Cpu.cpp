#include "../ICpu.hpp"

#include "sysfs/cpu_n/Topology.hpp"
#include "sysfs/cpu_n/Freq.hpp"
#include "sysfs/cpu_n/Cache.hpp"

#include "../../utils/exceptions.hpp"
#include "../../utils/units.hpp"
#include "../../utils/utils_linux.hpp"

#include "proc/Stat.hpp"
#include "proc/CpuInfo.hpp"

using namespace Cpu::Details::Sysfs::CpuN::Topology;
using namespace Cpu::Details::Sysfs::CpuN::Freq;
using namespace Cpu::Details::Sysfs::CpuN::Cache;
using namespace Cpu::Details;

namespace Self = Cpu;
using namespace Self;

#include <atomic>
#include <utility>
#include <iostream>

using namespace std;

std::ostream & Self::operator<<(std::ostream &os, const ICpu &c) {

	using namespace units;

	const auto &v0 = c.cache();
	const auto &v1 = c.core_stats();

	for (const auto e : v0)  os << e << "\n";

	os << "\n";

	for (uint8_t i = 0; i < v1.size(); i++) {
		os << "core [ " << (short)i << " ] \n"
		"\tusage:              " << to_string(v1[i].first, 2) << "\n"
		"\tfreq:               " << Hz::to_si( kHz::to_Hz(v1[i].second) ) << "\n\n";
	}

	os << "\n";

	/* dosn't have usefull info about cpu usage a valid array contains
	at least 2 proc/stat entry: cpu & cpu0 */

	if (v1.size() > 1)
		os << 
		"tot_usage:          " << to_string(c.total_pct_usage(), 2) << "\n";

	return 	os << 
		"cpu_min_freq:       " << Hz::to_si( kHz::to_Hz(c.cpu_min_freq) ) << "\n"
		"cpu_max_freq:       " << Hz::to_si( kHz::to_Hz(c.cpu_max_freq) ) << "\n"
		"cpu_model_name:     " << c.cpu_model_name     << "\n"
		"cpu_virtualization: " << c.cpu_virtualization << "\n"
		"cpu_core:           " << (short)c.cpu_core    << "\n"
		"cpu_thread:         " << (short)c.cpu_thread
	;

}

class SingleSocketCpu: public ICpu {

	static __attribute__((always_inline)) inline std::vector<CoreStat> single_cpu_get_core_stats(const std::vector<float> &v);

	public:
		friend ICpu * Self::get_single_sk_cpu(const std::unique_ptr<Stat> &cstat);
		friend std::ostream & Self::operator<<(std::ostream &os, const ICpu &c);

		uint8_t ncores() 
			const override { return cpu_core; }

		uint8_t nthreads()
			const override { return cpu_thread; }

		const std::string & virtualization()
			const override { return cpu_virtualization; }

		const std::string & modelname()
			const override { return cpu_model_name; }

		const std::vector<Cache> & cache()
			const override { return cpu_cache;  }

		const std::vector<CoreStat> & core_stats()
			const override { return core_stat; }

		float total_pct_usage()
			const override { return tot_pct_usage; }

	protected:
		std::vector<CoreStat> core_stat;
		float tot_pct_usage{};

};


ICpu * Self::get_single_sk_cpu(const std::unique_ptr<ICpu::Stat> &cstat) {

	constexpr static uint8_t CPU_0{};
	static atomic_bool once_flag;
	static SingleSocketCpu cpu;

	const std::vector<float> &pct = cstat ? cstat->pct_usage() : std::vector<float>(1);
	cpu.tot_pct_usage = pct[0];
	cpu.core_stat = SingleSocketCpu::single_cpu_get_core_stats(pct);

	/* execute once: get cpu_model_name, cpu_cache, cpu_physical_core_n, cpu_logical_core_n... */
	if (once_flag) return &cpu;

	once_flag = true;

	try {

		const auto &cpu_info = Proc::CpuInfo::get_cpu_info();
		const auto &freq = Freq(CPU_0);

		cpu.cpu_min_freq     = freq.cpuinfo_min_freq;
		cpu.cpu_max_freq     = freq.cpuinfo_max_freq;
		cpu.cpu_cache        = get_cache(CPU_0);
		cpu.cpu_core         = (uint8_t)std::stoi(cpu_info[CPU_0].entries[Proc::CpuInfo::CPU_CORES]);
		cpu.cpu_thread       = (uint8_t)std::stoi(cpu_info[CPU_0].entries[Proc::CpuInfo::SIBILINGS]);
		cpu.cpu_model_name   = std::move(cpu_info[CPU_0].entries[Proc::CpuInfo::MODEL_NAME]);

		const auto &flags = cpu_info[CPU_0].entries[Proc::CpuInfo::FLAGS];

		if (flags.find("svm") != std::string::npos)
			cpu.cpu_virtualization = "AMD-V";
		else if (flags.find("vmx") != std::string::npos)
			cpu.cpu_virtualization = "INTEL VT-x";

	} catch (...) {
		once_flag = false; 
		throw;
	}

	return &cpu;
}


__attribute__((always_inline)) inline std::vector<ICpu::CoreStat> SingleSocketCpu::single_cpu_get_core_stats(const std::vector<float> &v) {

	std::vector<CoreStat> vct;

	// the arrays dosn't contain usefull info
	if (v.size() < 2) return vct;

	/*
		for each entry except the first (which is total cpu usage) 
		check if is a physical cpu looking to his physical id

		cpu  1412389 5522 355959 5012355 2286 0 10046 0 0 0
		cpu0 353112 1871 88862 1254402 505 0 3643 0 0 0
		cpu1 361975 1025 87601 1248184 565 0 3357 0 0 0
		cpu2 358717 2000 92583 1236957 565 0 1820 0 0 0
		cpu3 338583 624 86912 1272810 649 0 1224 0 0 0
	*/

	// cpu0 physical id
	const uint8_t CPU_0 = Topology(0).physical_package_id;
	for (uint8_t i = 1; i < v.size(); i++) {

		Topology t((uint8_t)(i - 1));

		if (t.physical_package_id != CPU_0) 
			THROW_UNIMPL_EX("Multiple cpu systems aren't supported: ");

		vct.push_back({v.at(i), Freq((uint8_t)(i-1)).scaling_cur_freq});
	}

	return vct;
}