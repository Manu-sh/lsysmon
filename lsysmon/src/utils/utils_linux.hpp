#pragma once

#include <vector>
#include <string>

namespace utils::Linux {

	/* https://stackoverflow.com/questions/8401777/simple-glob-in-c-on-unix-system (MT-Unsafe) */
	std::vector<std::string> glob(const std::string &pattern);

	struct Fd {

		Fd() = default;
		Fd(int fd);
		~Fd();

		Fd(const Fd &) = delete;
		Fd & operator=(const Fd &) = delete;
		int fd = -1;
	};

}