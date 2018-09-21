#pragma once

#include <iostream>
#include <cassert>
#include <cstring>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>

using std::cout, std::endl;

extern "C" {
	#include <glob.h>
	#include <unistd.h>
}

#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

namespace utils::Linux {

	// https://stackoverflow.com/questions/8401777/simple-glob-in-c-on-unix-system
	//  MT-Unsafe !!!

	std::vector<std::string> glob(const std::string &pattern) {

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


	struct Fd {
		Fd() = default;

		Fd(const Fd &) 		   = delete;
		Fd & operator=(const Fd &) = delete;

		Fd(int fd) { if (fd < 0) throw std::system_error(errno, std::generic_category()); this->fd = fd; }
		~Fd() { close(fd); }
		int fd = -1;
	};

}

namespace utils::String {


	// return the pattern itself ( v.size() == 1 ) if there is nothing to split es. "hello" -> "hello", "  hello " -> "hello"
	static std::vector<std::string> split(const std::string_view &s, const std::string_view &set) noexcept {

		std::vector<std::string> vct(0);
		std::string_view::iterator it[2] {s.begin(), s.end()};

		const static auto &e_contained     = [&set](const char e) { return set.find(e) == std::string_view::npos; };
		const static auto &e_not_contained = [&set](const char e) { return set.find(e) != std::string_view::npos; };

		while (1) {

			if ((it[0] = std::find_if(it[0], s.end(), e_contained)) == s.end())
				return vct;

			if ((it[1] = std::find_if(it[0], s.end(), e_not_contained)) == s.end()) {
				vct.emplace_back(it[0], s.end());
				return vct;
			}

			vct.emplace_back(it[0], it[1]);
			it[0] = it[1];
		}

		return vct;
	}

}

namespace utils::Line {

	struct Line: std::string {};
	const auto &npos = std::string::npos;

	// getline from istream
	static std::istream & operator>>(std::istream &is, Line &l) { 
		return std::getline(is, l), is; 
	}

	// is can't be const they aren't const iterator
	static std::istream_iterator<Line> begin(std::istream &is) {
		return std::istream_iterator<Line>(is);
	}

	static const std::istream_iterator<Line> end([[maybe_unused]] const std::istream &dummy) {
		const static auto end = std::istream_iterator<Line>();
		return end;
	}

	class ifstream_l: public std::ifstream {

		public:
			using std::ifstream::basic_ifstream;
			ifstream_l(const ifstream_l &) = delete;
			ifstream_l & operator=(const ifstream_l &) = delete;

			auto begin()     { return utils::Line::begin(*this); }
			const auto end() { return utils::Line::end(*this);   }
			// void each_line(std::function<void(Line &)> f) { std::for_each(begin(), end(), f); }
	};

}

namespace units {

	/* TODO units::kHz::cast_to<hertz>(1000); */

	/* printf style double formatting:
		double d = 0.12345;
		to_string(d, 2) -> "0.12"
		to_string(d, 3) -> "0.123"
	*/


	static std::string to_string(double d, int precision) {

		using std::string_literals::operator""s;
		switch (precision) {
			case 0: return std::to_string((int)d);
			default:
			{
				char buf[50];
				return std::string(buf, snprintf(buf, sizeof buf, std::string("%."s + std::to_string(precision) + "lf"s).c_str(), d));
			}
		}
	}

	namespace byte::magnitude_order {

		template <typename T>
		constexpr T pebibyte = 1125899906842624;

		template <typename T>
		constexpr T tebibyte = 1099511627776;

		template <typename T>
		constexpr T gibibyte = 1073741824;

		template <typename T>
		constexpr T mebibyte = 1048576;

		template <typename T>
		constexpr T kibibyte = 1024;


		template <typename T>
		constexpr T petabyte = 1000000000000000;

		template <typename T>
		constexpr T terabyte = 1000000000000;

		template <typename T>
		constexpr T gigabyte = 1000000000;

		template <typename T>
		constexpr T megabyte = 1000000;

		template <typename T>
		constexpr T kilobyte = 1000;
	}

	namespace Hz::magnitude_order {

		template<typename T>
		constexpr T terahertz = 1000000000000;

		template<typename T>
		constexpr T gigahertz = 1000000000;

		template<typename T>
		constexpr T megahertz = 1000000;

		template<typename T>
		constexpr T kilohertz = 1000;
	}


	namespace kHz {

		template <typename T>
		static inline constexpr auto to_Hz(T khz) { 
			return khz * 1000; 
		}

	}



	namespace byte {

		static std::string to_iec(uint64_t bytes, int precision = 2) {

			using namespace byte::magnitude_order;
			using type = decltype(bytes);

			if (bytes >= pebibyte<type>)
				throw std::invalid_argument("TODO !!!");
			else if (bytes >= tebibyte<type>)
				return units::to_string(bytes / tebibyte<float>, precision) + "TiB";
			else if (bytes >= gibibyte<type>)
				return units::to_string(bytes / gibibyte<float>, precision) + "GiB";
			else if (bytes >= mebibyte<type>)
				return units::to_string(bytes / mebibyte<float>, precision) + "MiB";
			else if (bytes >= kibibyte<type>)
				return units::to_string(bytes / kibibyte<float>, precision) + "KiB";

			return units::to_string(bytes, precision) + "B";
		}

		static std::string to_si(uint64_t bytes, int precision = 2) {

			using namespace byte::magnitude_order;
			using type = decltype(bytes);

			if (bytes >= petabyte<type>)
				throw std::invalid_argument("TODO !!!");
			else if (bytes >= terabyte<type>)
				return units::to_string(bytes / terabyte<float>, precision) + "TB";
			else if (bytes >= gigabyte<type>)
				return units::to_string(bytes / gigabyte<float>, precision) + "GB";
			else if (bytes >= megabyte<type>)
				return units::to_string(bytes / megabyte<float>, precision) + "MB";
			else if (bytes >= kilobyte<type>)
				return units::to_string(bytes / kilobyte<float>, precision) + "KB";

			return units::to_string(bytes, precision) + "B";
		}
	}


	namespace Hz {

		static std::string to_si(uint64_t hz, int precision = 2) {

			using namespace Hz::magnitude_order;
			using type = decltype(hz);

			if (hz >= terahertz<type>)
				throw std::invalid_argument("TODO !!!");
			else if (hz >= gigahertz<type>)
				return units::to_string(hz / gigahertz<float>, precision) + "GHz";
			else if (hz >= megahertz<type>)
				return units::to_string(hz / megahertz<float>, precision) + "MHz";
			else if (hz >= kilohertz<type>)
				return units::to_string(hz / kilohertz<float>, precision) + "kHz";

			return units::to_string(hz, precision) + "Hz";
		}

	}

}
