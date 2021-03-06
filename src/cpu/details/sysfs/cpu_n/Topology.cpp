#include "Topology.hpp"
#include "common/Common.hpp"

#include "../../../../utils/utils_line.hpp"

namespace Self = Cpu::Details::Sysfs::CpuN::Topology;
using namespace Self;

std::ostream & Self::operator<<(std::ostream &os, const Topology &t) {
	return os << "physical_package_id: " << (short)t.physical_package_id;
}

/* used only here for now */
static __attribute__((always_inline)) inline void get_topology(Topology &top, uint8_t cpu_n) {

	enum: uint8_t { PHYSICAL_PACKAGE_ID, ARRAY_LENGTH /*  N + 1 */ };
	const static std::string fname[] { "physical_package_id" };
	const std::string scan_path{SYSFS_CPU_PATH"/cpu" + std::to_string(cpu_n) + "/topology/"};

	utils::Line::ifstream_l ifs;
	ifs.default_exceptions();

	for (uint_fast8_t i = UINT8_C(0); i < ARRAY_LENGTH; i++) {

		const auto &p = fname[(int)i];
		ifs.open(scan_path + p);
		utils::Line::Line line; ifs >> line;

		switch (i) {
			case PHYSICAL_PACKAGE_ID:
				top.physical_package_id = std::stoi(line);
				break;
		}

		ifs.close();
	}

}

Topology::Topology(uint8_t cpu_n) {
	get_topology(*this, cpu_n);
}
