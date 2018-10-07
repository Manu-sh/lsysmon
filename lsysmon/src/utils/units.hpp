#pragma once

#include <cstdio>
#include <string>
#include <stdexcept>

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

	namespace kB {

		template <typename T>
		static inline constexpr auto to_B(T kB) { 
			return kB * 1000; 
		}

	}

	namespace KiB {

		template <typename T>
		static inline constexpr auto to_B(T KiB) { 
			return KiB * 1024; 
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
