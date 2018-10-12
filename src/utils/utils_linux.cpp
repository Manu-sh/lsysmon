#include "utils_linux.hpp"
#include "utils_string.hpp"

#include <cstring>
#include <stdexcept>
#include <system_error>

extern "C" {
	#include <glob.h>
	#include <unistd.h>
}

using namespace utils::Linux;

std::vector<std::string> utils::Linux::glob(const std::string &pattern) {

	// glob struct resides on the stack
	glob_t glob_result;
	memset(&glob_result, 0, sizeof(glob_result));

	// do the glob operation
	if (auto err = glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result)) {
		globfree(&glob_result);
		throw std::runtime_error("glob() failed with return_value " + err);
	}

	std::vector<std::string> filenames;
	for(decltype(glob_result.gl_pathc) i = 0; i < glob_result.gl_pathc; ++i)
		filenames.emplace_back(glob_result.gl_pathv[i]);

	// cleanup
	globfree(&glob_result);
	return filenames;
}

Fd::Fd(int fd) { if (fd < 0) throw std::system_error(errno, std::generic_category()); this->fd = fd; }
Fd::~Fd() { close(fd); }