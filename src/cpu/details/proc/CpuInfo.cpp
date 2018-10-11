#include "CpuInfo.hpp"

#include "../../../utils/utils_regex.hpp"
#include "../../../utils/utils_line.hpp"

namespace Self = Cpu::Details::Proc::CpuInfo;
using namespace Self;

using utils::Regex::operator""_ri;

/* We want slicing */
struct _CpuEntry: CpuEntry {

	const static std::regex line_reg[ARRAY_LENGTH];
	const static char *entry_name[ARRAY_LENGTH];

	explicit _CpuEntry(const std::vector<utils::Line::Line> &line) {

		entries.reserve(ARRAY_LENGTH);
		for (uint32_t i = 0; i < ARRAY_LENGTH; i++) {

			std::smatch match;
			for (uint32_t j = 0; j < line.size(); j++)
				if (std::regex_search(line[j], match, line_reg[i]))
					break;

			if (match.size() <= 1) 
				throw std::runtime_error("missing cpu entry " + std::string{entry_name[i]});

			entries.push_back(match[1].str());
		}

	}

};


std::vector<CpuEntry> Self::get_cpu_info() {

	std::vector<CpuEntry> vct;
	std::vector<utils::Line::Line> cpu_n_entry_lines(12 * 4); /* cpu_N lines into /proc/cpuinfo */
	utils::Line::ifstream_l proc_cpuinfo{"/proc/cpuinfo"};
	proc_cpuinfo.default_exceptions();

	for (const auto &l : proc_cpuinfo) {

		cpu_n_entry_lines.emplace_back(l);

		if (l.empty()) { /* end of cpu_N lines (\n) */
			vct.push_back( _CpuEntry{cpu_n_entry_lines} ); /* Slicing */
			cpu_n_entry_lines.clear();
		}
	}

	return vct;
}


std::ostream & Self::operator<<(std::ostream &os, const CpuEntry &ref) {

	return os <<
		_CpuEntry::entry_name[PROCESSOR]   << ref.entries[PROCESSOR]   << "\n" <<
		_CpuEntry::entry_name[VENDOR_ID]   << ref.entries[VENDOR_ID]   << "\n" <<
		_CpuEntry::entry_name[CPU_FAMILY]  << ref.entries[CPU_FAMILY]  << "\n" <<
		_CpuEntry::entry_name[MODEL]       << ref.entries[MODEL]       << "\n" <<
		_CpuEntry::entry_name[PHYSICAL_ID] << ref.entries[PHYSICAL_ID] << "\n" <<
		_CpuEntry::entry_name[SIBILINGS]   << ref.entries[SIBILINGS]   << "\n" <<
		_CpuEntry::entry_name[CORE_ID]     << ref.entries[CORE_ID]     << "\n" <<
		_CpuEntry::entry_name[CPU_CORES]   << ref.entries[CPU_CORES]   << "\n" <<
		_CpuEntry::entry_name[CPU_MHZ]     << ref.entries[CPU_MHZ]     << "\n" <<
		_CpuEntry::entry_name[CACHE_SIZE]  << ref.entries[CACHE_SIZE]  << "\n" <<
		_CpuEntry::entry_name[MODEL_NAME]  << ref.entries[MODEL_NAME]  << "\n" <<
		_CpuEntry::entry_name[FLAGS]       << ref.entries[FLAGS]       << "\n";

}


const std::regex _CpuEntry::line_reg[ARRAY_LENGTH] {
	[PROCESSOR]   = R"(^processor\s*:\s*(.*))"_ri,
	[VENDOR_ID]   = R"(^vendor_id\s*:\s*(.*))"_ri,
	[CPU_FAMILY]  = R"(^cpu\s+family\s*:\s*(.*))"_ri,
	[MODEL]       = R"(^model\s*:\s*(.*))"_ri,
	[PHYSICAL_ID] = R"(^physical\s+id\s*:\s*(.*))"_ri,
	[SIBILINGS]   = R"(^siblings\s*:\s*(.*))"_ri,
	[CORE_ID]     = R"(^core\s+id\s*:\s*(.*))"_ri,
	[CPU_CORES]   = R"(^cpu\s+cores\s*:\s*(.*))"_ri,
	[CPU_MHZ]     = R"(^cpu\s+mhz\s*:\s*(.*))"_ri,
	[CACHE_SIZE]  = R"(^cache\s+size\s*:\s*(.*))"_ri,
	[MODEL_NAME]  = R"(^model\s+name\s*:\s*(.*))"_ri,
	[FLAGS]       = R"(^flags\s*:\s*(.*))"_ri
};


const char *_CpuEntry::entry_name[ARRAY_LENGTH] {
	[PROCESSOR]   = "PROCESSOR   ",
	[VENDOR_ID]   = "VENDOR_ID   ",
	[CPU_FAMILY]  = "CPU_FAMILY  ",
	[MODEL]       = "MODEL       ",
	[PHYSICAL_ID] = "PHYSICAL_ID ",
	[SIBILINGS]   = "SIBILINGS   ",
	[CORE_ID]     = "CORE_ID     ",
	[CPU_CORES]   = "CPU_CORES   ",
	[CPU_MHZ]     = "CPU_MHZ     ",
	[CACHE_SIZE]  = "CACHE_SIZE  ",
	[MODEL_NAME]  = "MODEL_NAME  ",
	[FLAGS]       = "FLAGS       "
};