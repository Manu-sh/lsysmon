#pragma once
#include <string>
#include <cstdint>
#include <vector>

#include "sysfs/cpu_n/Cache.hpp"
#include "proc/Stat.hpp"

namespace Cpu::Details::Cpu {

	class MCpu {

		public:
			using Cache = ::Cpu::Details::Sysfs::CpuN::Cache::Cache;
			using Stat  = ::Cpu::Details::Proc::Stat::Stat;

			friend std::vector<MCpu> get_cpu_stat(const Stat &cstat);

			uint8_t   ncores() const;
			uint8_t nthreads() const;

			const std::string & virtualization() const;
			const std::string & modelname() const;
			const std::vector<Cache> & cache() const;

			const std::vector<float> & core_pct_usage() const;
			float total_pct_usage() const;
 
		private:
			MCpu() = default;

		private:
			std::string cpu_model_name;
			std::string cpu_virtualization;
			std::vector<Cache> cpu_cache;

			uint8_t cpu_core;
			uint8_t cpu_thread;
	};

	std::vector<MCpu> get_cpu_stat(const MCpu::Stat &cstat);

}