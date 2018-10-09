#pragma once

#include <vector>
#include <cstdint>
#include <ostream>

namespace Mem::Details::Proc::MemInfo {

	/* they are guaranteed to be existing and non-empty */

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
		std::vector<uint64_t> entries; /* all entries in kB */
	};

	MemInfo get_meminfo();
	void get_meminfo(MemInfo &mem);

	/* entries are printed in KiB instead of kB */
	std::ostream & operator<<(std::ostream &os, const MemInfo &ref);
}