#include "common/Common.hpp"
#include "Cache.hpp"

#include "../../../../utils/utils_linux.hpp"
#include "../../../../utils/utils_line.hpp"
#include "../../../../utils/exceptions.hpp"

#include <string_view>

namespace Self = Cpu::Details::Sysfs::CpuN::Cache;
using namespace Self; /* this namespace */

const std::string_view type_name[ARRAY_LENGTH] {
	"Instruction",
	"Data",
	"Unified"
};

static inline Cache _todo(Cache &cache, uint8_t cpu_n, uint8_t index_n) {

	enum: uint8_t { LEVEL, TYPE, SIZE, ARRAY_LENGTH /*  N + 1 */ };
	const static std::string fname[ARRAY_LENGTH] { "level", "type", "size" };

	const std::string scan_path{SYSFS_CPU_PATH"/cpu" + std::to_string(cpu_n) + "/cache/index" + std::to_string(index_n) + "/"};
	utils::Line::ifstream_l ifs;
	ifs.default_exceptions();

	for (uint_fast8_t i = UINT8_C(0); i < ARRAY_LENGTH; i++) {

		const auto &p = fname[(int)i];

		ifs.open(scan_path + p);		
		utils::Line::Line line; ifs >> line;

		switch (i) {

			case LEVEL:
				cache.level = std::stoi(line);
				break;
			case SIZE:
				cache.size  = std::stoi(line);
				break;
			case TYPE:

				if (line == type_name[UNIFIED]) /* common case */
					cache.type = UNIFIED;

				else if (line == type_name[DATA])
					cache.type = DATA;

				else if (line == type_name[INSTRUCTION])
					cache.type = INSTRUCTION;

				else THROW_BUG_EX();
				break;
		}

		ifs.close(); // no exception
	}

	return cache;
}

#if 0
bool Self::operator==(const Cache &a, const Cache &b) { return a.id == b.id && a.level == b.level; }
bool Self::operator!=(const Cache &a, const Cache &b) { return ! (a == b); }
#endif

std::ostream & Self::operator<<(std::ostream &os, const Cache &c) {
	return os <<
		"level:              " << (short)c.level     << "\n" <<
		"type:               " << type_name[c.type]  << "\n" <<
		"size (kB):          " << c.size << "\n";
}

std::vector<Cache> Self::get_cache(uint8_t cpu_n) {

	std::vector<Cache> cpu_cache;

	const uint8_t cache_idx = (uint8_t)utils::Linux::glob(
		"/sys/devices/system/cpu/cpu" 
		+ std::to_string(cpu_n) +
		"/cache/index[0-9]*"
	).size();

	for (uint_fast8_t j = UINT8_C(0); j < cache_idx; j++) {
		Cache cache;
		cpu_cache.push_back( _todo(cache, cpu_n, j) );
	}

	return cpu_cache;
}