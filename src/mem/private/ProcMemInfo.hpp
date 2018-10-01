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
		SHMEM,

		ARRAY_LENGTH /* LENGTH = N + 1 */
	};

	struct MemInfo {

		const static char *entry_name[ARRAY_LENGTH];
		const static std::regex line_reg[ARRAY_LENGTH];

		explicit MemInfo() {

			utils::Line::ifstream_l proc_meminfo{"/proc/meminfo"};
			proc_meminfo.default_exceptions();

			for (uint_fast8_t i = 0; i < ARRAY_LENGTH; i++) {

				std::smatch match;

				for (const auto &line : proc_meminfo)
					if (std::regex_search(line, match, line_reg[i]))
						break;
				
				if (match.size() <= 1) 
					throw std::runtime_error("missing meminfo entry " + std::string{entry_name[i]});

				entries.push_back((uint64_t)std::stoul(match[1].str()));
			}

		}

		friend std::ostream & operator<<(std::ostream &os, const MemInfo &ref) {

			const static auto &to_human = [](uint64_t kB) { 
				return units::byte::to_iec(
					units::KiB::to_B(kB) /* kB -> KiB */
				); 
			};

			return os <<
				entry_name[MEMTOTAL]      << to_human(ref.entries[MEMTOTAL])     << "\n" <<
				entry_name[MEMFREE]       << to_human(ref.entries[MEMFREE])      << "\n" <<
				entry_name[MEMAVAILABLE]  << to_human(ref.entries[MEMAVAILABLE]) << "\n" <<
				entry_name[BUFFERS]       << to_human(ref.entries[BUFFERS])      << "\n" <<
				entry_name[CACHED]        << to_human(ref.entries[CACHED])       << "\n" <<
				entry_name[SWAPCACHED]    << to_human(ref.entries[SWAPCACHED])   << "\n" <<
				entry_name[SWAPTOTAL]     << to_human(ref.entries[SWAPTOTAL])    << "\n" <<
				entry_name[SWAPFREE]      << to_human(ref.entries[SWAPFREE])     << "\n" <<
				entry_name[SHMEM]         << to_human(ref.entries[SHMEM])        << "\n";

		}

		std::vector<uint64_t> entries;

	};

	const char *MemInfo::entry_name[ARRAY_LENGTH] {
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

	const std::regex MemInfo::line_reg[ARRAY_LENGTH] {
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

}
