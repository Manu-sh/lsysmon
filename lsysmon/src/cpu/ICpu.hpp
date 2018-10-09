#pragma once
#include <string>
#include <memory>
#include <utility>
#include <cstdint>
#include <vector>
#include <ostream>

#include "details/sysfs/cpu_n/Cache.hpp"
#include "details/proc/Stat.hpp"

namespace Cpu {

	class ICpu {
 
		public:

			/* core_pct_usage, core_freq_khz */
			using CoreStat = std::pair<float, uint32_t>;
			using Cache = ::Cpu::Details::Sysfs::CpuN::Cache::Cache;
			using Stat  = ::Cpu::Details::Proc::Stat::Stat;

			/* if get_single_sk_cpu is called with nullptr dosn't return info about cpu and core usages 
			so and total_pct_usage() and core_stats() dons't provide any info about cpu usage */
		
			friend ICpu * get_single_sk_cpu(const std::unique_ptr<Stat> &cstat = nullptr);
			friend std::ostream & operator<<(std::ostream &os, const ICpu &c);

			virtual ~ICpu() = default;

			virtual uint8_t ncores() const = 0;
			virtual uint8_t nthreads() const = 0;
			virtual const std::string & virtualization() const = 0;
			virtual const std::string & modelname() const = 0;
			virtual const std::vector<Cache> & cache() const = 0;
			virtual const std::vector<CoreStat> & core_stats() const = 0;
			virtual float total_pct_usage() const = 0;

		protected:
			std::string cpu_model_name;
			std::string cpu_virtualization;
			std::vector<Cache> cpu_cache;

			uint32_t cpu_min_freq{}; /* kHz */
			uint32_t cpu_max_freq{}; /* kHz */
			uint8_t  cpu_core{};     /* core per cpu */
			uint8_t  cpu_thread{};   /* thread per cpu */
	};

	std::ostream & operator<<(std::ostream &os, const ICpu &c);
	ICpu * get_single_sk_cpu(const std::unique_ptr<ICpu::Stat> &cstat);

}
