#pragma once

#include <vector>
#include <string>
#include <unordered_map>

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


#if 0
	struct SysfsEntry {
		std::string path;
		std::string key;
		std::string value;
	};

	/* TODO
		/sys/block/sd[a-z]/device/model
		model:
			...
	*/

	std::unordered_map<std::string, std::string> map_sysfs(const std::string_view &path_glob);

#endif

}