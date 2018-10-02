#pragma once

#include <ctime>

extern "C" {
	#include <sys/utsname.h>
}

#include "utils/utils.hpp"
#include "cpu/Cpu.hpp"

#include <iostream>
using std::endl, std::cout;

namespace Linux {

	struct Uname: utsname {
		Uname() { ::uname((struct utsname *)this); }
	};

	time_t get_uptime_millisec() {
		double uptime{};
		std::ifstream proc_uptime{"/proc/uptime"};
		proc_uptime.exceptions(std::ifstream::failbit|std::ifstream::badbit);
		return (proc_uptime >> uptime), (time_t)uptime;
	}

	std::string get_uptime(time_t uptime = get_uptime_millisec()) {
		char buf[80];
		std::strftime(buf, sizeof buf, "up hours %H, minutes %M", gmtime(&uptime));
		return buf;
	}

}