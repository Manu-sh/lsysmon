#include "Stat.hpp"

#include "../../../utils/regex_utils.hpp"
#include "../../../utils/utils.hpp"

#include <cstdint>
#include <cassert>
#include <utility>
#include <numeric>

using namespace Cpu::Details::Proc::Stat;
using regex_utils::operator""_ri;

// indexs to access map_proc_stat result (see man 5 proc)
enum: uint8_t { USER, NICE, SYSTEM, IDLE, IO_WAIT, IRQ, SOFT_IRQ };
static inline std::vector<std::vector<uint32_t>> map_proc_stat();

class _Stat: public Stat {

	public:
		std::vector<float> pct_usage() const override;

	private:
		// prev_idle, prev_total
		mutable std::vector<std::pair<uint32_t,uint32_t>> hold_cpu_stats;
};

std::vector<float> _Stat::pct_usage() const {

	// const static float USER_HZ = sysconf(_SC_CLK_TCK);

	std::vector<float> usage(0);
	const auto &mtx = map_proc_stat();
	hold_cpu_stats.resize(mtx.size(), {0U, 0U}); // make_pair(0u, 0u)

	for (uint32_t cpu = 0; cpu < mtx.size(); cpu++) {

		const uint32_t idle    = mtx[cpu].at(IDLE);
		const uint32_t total   = std::accumulate(mtx[cpu].begin(), mtx[cpu].end(), 0U);
		const float diff_idle  = idle  - hold_cpu_stats.at(cpu).first;
		const float diff_total = total - hold_cpu_stats[cpu].second;

		// always call sleep(1) between 2 pct_usage()
		assert(diff_total);

		const float diff_usage = 100.0f * (1.0f - diff_idle / diff_total);
		usage.emplace_back( diff_usage );

		hold_cpu_stats[cpu].first  = idle;
		hold_cpu_stats[cpu].second = total;
	}

	return usage;

}

static inline std::vector<std::vector<uint32_t>> map_proc_stat() {

	const static auto &rows = "^cpu[0-9]{0,250}\\s.*"_ri;
	const static auto &cols = "\\s([0-9]+)"_ri;

	utils::Line::ifstream_l proc_stat{"/proc/stat"};
	proc_stat.default_exceptions();

	std::vector<std::vector<uint32_t>> mtx(0);

	for (const auto &line : proc_stat) {

		if (!std::regex_match(line, rows)) 
			continue;

		mtx.emplace_back(0);
		std::for_each(
			std::sregex_iterator{line.begin(), line.end(), cols},
			std::sregex_iterator{},
			[&mtx] (const std::smatch &s) { mtx.back().emplace_back(std::stoul(s[1].str())); }
		);
	}

	return mtx;
}