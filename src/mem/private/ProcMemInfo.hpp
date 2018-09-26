#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <regex>

#include "../../utils/utils.hpp"
#include "../../utils/regex_utils.hpp"

namespace Proc {

	// literal constructor for case insensitive regex
	using regex_utils::operator""_ri;

	enum {
		MEMTOTAL,
		MEMFREE,
		MEMAVAILABLE,
		BUFFERS,
		CACHED,
		SWAPCACHED,
		SWAPTOTAL,
		SWAPFREE,

		ARRAY_LENGTH /* LENGTH = N + 1 */
	};

#if 0
	struct MemInfo {

		const static char *entry_name[ARRAY_LENGTH];
		const static std::regex line_reg[ARRAY_LENGTH];

		explicit MemInfo(const std::vector<utils::Line::Line> &line) {

			entries.reserve(ARRAY_LENGTH);
			for (uint32_t i = 0; i < ARRAY_LENGTH; i++) {

				std::smatch match;
				for (uint32_t j = 0; j < line.size(); j++)
					if (std::regex_search(line[j], match, line_reg[i]))
						break;

				if (match.size() <= 1) 
					throw std::runtime_error("missing cpu entry " + std::string{entry_name[i]});

				entries.push_back( (uint32_t)std::stoul(match[1].str()) );
			}

		}

		friend std::ostream & operator<<(std::ostream &os, const MemInfo &ref) {

			return os <<
				entry_name[PROCESSOR]   << ref.entries[PROCESSOR]   << "\n" <<
				entry_name[VENDOR_ID]   << ref.entries[VENDOR_ID]   << "\n" <<
				entry_name[CPU_FAMILY]  << ref.entries[CPU_FAMILY]  << "\n" <<
				entry_name[MODEL]       << ref.entries[MODEL]       << "\n" <<
				entry_name[PHYSICAL_ID] << ref.entries[PHYSICAL_ID] << "\n" <<
				entry_name[SIBILINGS]   << ref.entries[SIBILINGS]   << "\n" <<
				entry_name[CORE_ID]     << ref.entries[CORE_ID]     << "\n" <<
				entry_name[CPU_CORES]   << ref.entries[CPU_CORES]   << "\n" <<
				entry_name[CPU_MHZ]     << ref.entries[CPU_MHZ]     << "\n" <<
				entry_name[CACHE_SIZE]  << ref.entries[CACHE_SIZE]  << "\n" <<
				entry_name[MODEL_NAME]  << ref.entries[MODEL_NAME]  << "\n" <<
				entry_name[FLAGS]       << ref.entries[FLAGS]       << "\n";

		}

		std::vector<uint32_t> entries;

	};

	// TODO replace with real suitables names || delete
	const char *MemInfo::entry_name[ARRAY_LENGTH] {
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

	const std::regex MemInfo::line_reg[ARRAY_LENGTH] {
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
#endif

}