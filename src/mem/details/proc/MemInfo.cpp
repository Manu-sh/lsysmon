#include "MemInfo.hpp"

#include "../../../utils/utils_regex.hpp"
#include "../../../utils/utils_line.hpp"
#include "../../../utils/units.hpp"

#include <stdexcept>

namespace Self = Mem::Details::Proc::MemInfo;
using namespace Self;

using utils::Regex::operator""_ri;

struct _MemInfo: MemInfo {
	const static std::regex line_reg[ARRAY_LENGTH];
	const static char *entry_name[ARRAY_LENGTH];
	_MemInfo() { get_meminfo(*this); }

};

std::ostream & Self::operator<<(std::ostream &os, const MemInfo &ref) {

	const static auto &to_human = [](uint64_t kB) { 
		return units::byte::to_iec(
			units::KiB::to_B(kB) /* kB -> KiB */
		); 
	};

	return os <<
		_MemInfo::entry_name[MEMTOTAL]      << to_human(ref.entries[MEMTOTAL])     << "\n" <<
		_MemInfo::entry_name[MEMFREE]       << to_human(ref.entries[MEMFREE])      << "\n" <<
		_MemInfo::entry_name[MEMAVAILABLE]  << to_human(ref.entries[MEMAVAILABLE]) << "\n" <<
		_MemInfo::entry_name[BUFFERS]       << to_human(ref.entries[BUFFERS])      << "\n" <<
		_MemInfo::entry_name[CACHED]        << to_human(ref.entries[CACHED])       << "\n" <<
		_MemInfo::entry_name[SWAPCACHED]    << to_human(ref.entries[SWAPCACHED])   << "\n" <<
		_MemInfo::entry_name[SWAPTOTAL]     << to_human(ref.entries[SWAPTOTAL])    << "\n" <<
		_MemInfo::entry_name[SWAPFREE]      << to_human(ref.entries[SWAPFREE])     << "\n" <<
		_MemInfo::entry_name[SHMEM]         << to_human(ref.entries[SHMEM])        << "\n";

}

MemInfo Self::get_meminfo() { return _MemInfo(); }
void Self::get_meminfo(MemInfo &mem) {

	utils::Line::ifstream_l proc_meminfo{"/proc/meminfo"};
	proc_meminfo.default_exceptions();

	for (uint_fast8_t i = 0; i < ARRAY_LENGTH; i++) {

		std::smatch match;

		for (const auto &line : proc_meminfo)
			if (std::regex_search(line, match, _MemInfo::line_reg[i]))
				break;

		if (match.size() <= 1) 
			throw std::runtime_error("missing meminfo entry " + std::string{_MemInfo::entry_name[i]});

		mem.entries.push_back((uint64_t)std::stoul(match[1].str()));
	}
}


const char *_MemInfo::entry_name[ARRAY_LENGTH] {
	[MEMTOTAL]     = "MEMTOTAL     ",
	[MEMFREE]      = "MEMFREE      ",
	[MEMAVAILABLE] = "MEMAVAILABLE ",
	[BUFFERS]      = "BUFFERS      ",
	[CACHED]       = "CACHED       ",
	[SWAPCACHED]   = "SWAPCACHED   ",
	[SWAPTOTAL]    = "SWAPTOTAL    ",
	[SWAPFREE]     = "SWAPFREE     ",
	[SHMEM]        = "SHMEM        "
};

/* regexs like R"(^memtotal\s*:\s*(\d+)\s*kb)"_ri making all slower */
const std::regex _MemInfo::line_reg[ARRAY_LENGTH] {
	[MEMTOTAL]     = R"(^memtotal\s*:\s*(.*)kb)"_ri,
	[MEMFREE]      = R"(^memfree\s*:\s*(.*)kb)"_ri,
	[MEMAVAILABLE] = R"(^memavailable\s*:\s*(.*)kb)"_ri,
	[BUFFERS]      = R"(^buffers\s*:\s*(.*)kb)"_ri,
	[CACHED]       = R"(^cached\s*:\s*(.*)kb)"_ri,
	[SWAPCACHED]   = R"(^swapcached\s*:\s*(.*)kb)"_ri,
	[SWAPTOTAL]    = R"(^swaptotal\s*:\s*(.*)kb)"_ri,
	[SWAPFREE]     = R"(^swapfree\s*:\s*(.*)kb)"_ri,
	[SHMEM]        = R"(^shmem\s*:\s*(.*)kb)"_ri
};