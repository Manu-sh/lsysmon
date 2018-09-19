#pragma once

extern "C" {
	#include <sys/utsname.h>
	#include <sys/sysinfo.h>
}

#include "utils/utils.hpp"
#include "cpu/Cpu.hpp"

#include <iostream>
using std::endl, std::cout;

namespace Linux {

	struct Uname: utsname {
		Uname() { ::uname((struct utsname *)this); }
	};

	/* TODO buggy free % man free man 5 proc */
	class Sysinfo: sysinfo {

		public:
			Sysinfo() { ::sysinfo((struct sysinfo *)this); }
			std::string get_total_ram()  const { return units::byte::to_iec(totalram  * mem_unit); }
			std::string get_free_ram()   const { return units::byte::to_iec(freeram   * mem_unit); }
			std::string get_shared_ram() const { return units::byte::to_iec(sharedram * mem_unit); }
			std::string get_buffer_ram() const { return units::byte::to_iec(bufferram * mem_unit); }
			std::string get_total_swap() const { return units::byte::to_iec(totalswap * mem_unit); }
			std::string get_free_swap()  const { return units::byte::to_iec(freeswap  * mem_unit); }
			std::string get_total_high() const { return units::byte::to_iec(totalhigh * mem_unit); }
			std::string get_free_high()  const { return units::byte::to_iec(freehigh  * mem_unit); }
			std::string get_used_ram()   const { return units::byte::to_iec((totalram - freeram) * mem_unit); }

			std::string get_uptime() const {
				char buf[80];
				std::strftime(buf, sizeof buf, "days %d, hours %H, minutes %M", gmtime(&this->uptime));
				return buf;
			}

			auto get_procs() { return procs; }

	};

}
