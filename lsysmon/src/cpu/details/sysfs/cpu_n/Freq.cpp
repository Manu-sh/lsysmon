#include "common/Common.hpp"
#include "Freq.hpp"

#include "../../../../utils/utils_line.hpp"

namespace Self = Cpu::Details::Sysfs::CpuN::Freq;
using namespace Self;

std::ostream & Self::operator<<(std::ostream &os, const Freq &f) {
	return os <<
		"cpuinfo_min_freq (KHz): " << f.cpuinfo_min_freq << "\n" <<
		"cpuinfo_max_freq (KHz): " << f.cpuinfo_max_freq << "\n" <<
		"scaling_cur_freq (KHz): " << f.scaling_cur_freq;
}

/* used only here for now */
static inline void get_freq(Freq &freq, uint8_t cpu_n) {

	enum: uint8_t { CPUINFO_CUR_FREQ, SCALING_CUR_FREQ, CPUINFO_MIN_FREQ, CPUINFO_MAX_FREQ, ARRAY_LENGTH /*  N + 1 */ };
	const static std::string fname[ARRAY_LENGTH] { "cpuinfo_cur_freq", "scaling_cur_freq", "cpuinfo_min_freq", "cpuinfo_max_freq" };

	const std::string scan_path{SYSFS_CPU_PATH"/cpu" + std::to_string(cpu_n) + "/cpufreq/"};
	utils::Line::ifstream_l ifs;
	ifs.default_exceptions();

	{
		utils::Line::Line line;

		try {
			ifs.open(scan_path + fname[SCALING_CUR_FREQ]);
		} catch (...) {
			ifs.open(scan_path + fname[CPUINFO_CUR_FREQ]); /* throw */
		}

		freq.scaling_cur_freq = std::stoi(   (ifs >> line, line)  );
		ifs.close();
	}

	for (uint_fast8_t i = CPUINFO_MIN_FREQ; i < ARRAY_LENGTH; i++) {

		const auto &p = fname[(int)i];
		ifs.open(scan_path + p);

		utils::Line::Line line; ifs >> line;

		switch (i) {

			case CPUINFO_MIN_FREQ:
				freq.cpuinfo_min_freq = std::stoi(line);
				break;
			case CPUINFO_MAX_FREQ:
				freq.cpuinfo_max_freq = std::stoi(line);
				break;
		}

		ifs.close();
	}

}

Freq::Freq(uint8_t cpu_n) {
	get_freq(*this, cpu_n);
}