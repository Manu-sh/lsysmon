#pragma once
#include <vector>
#include <cstdint>
#include <ostream>

/* https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-devices-system-cpu */
namespace Cpu::Details::Sysfs::CpuN::Cache {

	enum: uint8_t { INSTRUCTION, DATA, UNIFIED, ARRAY_LENGTH };

	/* the member Cache::type can assume one of following values, 
	all except ARRAY_LENGTH which is used internally by Cache.cpp */

	struct Cache {
		uint32_t size; 		    /* size in kB 						        */
		uint8_t id;                 /* an unique number for a specific instance of a cache              */
		uint8_t level: 4;           /* cache hierarchy in the multi-level cache configuration           */
		uint8_t type:  4;           /* Instruction, Data, Unified (holds both data and instructions)    */
	};


#if 0
	/* operator==() use the id field & type to understand 
	when two caches are two physically different caches */

	bool operator==(const Cache &a, const Cache &b);
	bool operator!=(const Cache &a, const Cache &b);
#endif

	std::ostream & operator<<(std::ostream &os, const Cache &f);
	std::vector<Cache> get_cache(uint8_t cpu_n);
}