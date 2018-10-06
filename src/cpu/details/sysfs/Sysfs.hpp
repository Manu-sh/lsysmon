#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <utility>

#include "../../../utils/utils.hpp"
#include "../../../utils/exceptions.hpp"

#define SYSFS_CPU_PATH "/sys/devices/system/cpu"

using std::endl, std::cout;


/* https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-devices-system-cpu */
namespace Cpu::Details::Sysfs::CpuN::Cache {

	struct Cache {

		enum: uint8_t { INSTRUCTION, DATA, UNIFIED, ARRAY_LENGTH };
		const static std::string_view type_name[ARRAY_LENGTH];
		explicit Cache(uint8_t cpu_n, uint8_t index_n);

		uint32_t size; 		    /* size in kB 						        */
		uint8_t level: 4;           /* cache hierarchy in the multi-level cache configuration           */
		uint8_t type:  4;           /* Instruction, Data, Unified (holds both data and instructions)    */
	};

	const std::string_view Cache::type_name[Cache::ARRAY_LENGTH] {
		"Instruction",
		"Data",
		"Unified"
	};

	static std::ostream & operator<<(std::ostream &os, const Cache &c) {
		return os <<
			"level:              " << (short)c.level      	      << "\n" <<
			"type:               " << c.type_name[c.type]         << "\n" <<
			"size (kB):          " << c.size;
	}

}

Cpu::Details::Sysfs::CpuN::Cache::Cache::Cache(uint8_t cpu_n, uint8_t index_n) {

	enum: uint8_t { LEVEL, TYPE, SIZE, ARRAY_LENGTH /*  N + 1 */ };
	const static std::string fname[ARRAY_LENGTH] { "level", "type", "size" };

	const std::string scan_path{SYSFS_CPU_PATH"/cpu" + std::to_string(cpu_n) + "/cache/index" + std::to_string(index_n) + "/"};
	utils::Line::ifstream_l ifs;
	ifs.default_exceptions();

	for (uint_fast8_t i = UINT8_C(0); i < ARRAY_LENGTH; i++) {

		const auto &p = fname[i];

		ifs.open(scan_path + p);		
		utils::Line::Line line; ifs >> line;

		switch (i) {

			case LEVEL:
				this->level = std::stoi(line);
				break;
			case SIZE:
				this->size  = std::stoi(line);
				break;
			case TYPE:
				// std::transform(line.begin(), line.end(), line.begin(), ::tolower);

				if (line == type_name[UNIFIED]) /* common case */
					this->type = UNIFIED;

				else if (line == type_name[DATA])
					this->type = DATA;

				else if (line == type_name[INSTRUCTION])
					this->type = INSTRUCTION;

				else THROW_BUG_EX();
				break;
		}

		ifs.close(); // no exception
	}

}

/* https://www.kernel.org/doc/Documentation/cpu-freq/user-guide.txt */
namespace Cpu::Details::Sysfs::CpuN::Freq {

	struct Freq {
		explicit Freq(uint8_t cpu_n);
		uint32_t cpuinfo_min_freq; /* minimum operating frequency KHz */
		uint32_t cpuinfo_max_freq; /* maximum operating frequency KHz */
		uint32_t scaling_cur_freq; /* current cpu frequency determined by the governor in KHz,
					      when this file dosn't exist the value come from cpuinfo_cur_freq */
	};

	static std::ostream & operator<<(std::ostream &os, const Freq &f) {
		return os <<
			"cpuinfo_min_freq (KHz): " << f.cpuinfo_min_freq << "\n" <<
			"cpuinfo_max_freq (KHz): " << f.cpuinfo_max_freq << "\n" <<
			"scaling_cur_freq (KHz): " << f.scaling_cur_freq;
	}

}

Cpu::Details::Sysfs::CpuN::Freq::Freq::Freq(uint8_t cpu_n) {

	enum: uint8_t { CPUINFO_CUR_FREQ, SCALING_CUR_FREQ, CPUINFO_MIN_FREQ, CPUINFO_MAX_FREQ, ARRAY_LENGTH /*  N + 1 */ };
	const static std::string fname[ARRAY_LENGTH] { "cpuinfo_cur_freq", "scaling_cur_freq", "cpuinfo_min_freq", "cpuinfo_max_freq" };

	const std::string scan_path{SYSFS_CPU_PATH"/cpu" + std::to_string(cpu_n) + "/cpufreq/"};
	utils::Line::ifstream_l ifs;
	ifs.default_exceptions();

	{
		utils::Line::Line line;

		try {
			ifs.open(scan_path + fname[SCALING_CUR_FREQ]);
		} catch (...) {
			ifs.open(scan_path + fname[CPUINFO_CUR_FREQ]); /* throw */
		}

		this->scaling_cur_freq = std::stoi(   (ifs >> line, line)  );
		ifs.close();
	}

	for (uint_fast8_t i = CPUINFO_MIN_FREQ; i < ARRAY_LENGTH; i++) {

		const auto &p = fname[i];
		ifs.open(scan_path + p);

		utils::Line::Line line; ifs >> line;

		switch (i) {

			case CPUINFO_MIN_FREQ:
				this->cpuinfo_min_freq = std::stoi(line);
				break;
			case CPUINFO_MAX_FREQ:
				this->cpuinfo_max_freq = std::stoi(line);
				break;
		}

		ifs.close();
	}

}


/* https://www.kernel.org/doc/Documentation/cputopology.txt */
namespace Cpu::Details::Sysfs::CpuN::Topology {

	struct Topology {
		explicit Topology(uint8_t cpu_n);
		uint8_t physical_package_id; /* Typically corresponds to a physical socket number */
	};

	static std::ostream & operator<<(std::ostream &os, const Topology &t) {
		return os << "physical_package_id: " << (short)t.physical_package_id;
	}
}

// TODO expand range function !!! split(..., "-,"); ?
Cpu::Details::Sysfs::CpuN::Topology::Topology::Topology(uint8_t cpu_n) {

	enum: uint8_t { PHYSICAL_PACKAGE_ID, ARRAY_LENGTH /*  N + 1 */ };
	const static std::string fname[] { "physical_package_id" };

	const std::string scan_path{SYSFS_CPU_PATH"/cpu" + std::to_string(cpu_n) + "/topology/"};
	utils::Line::ifstream_l ifs;
	ifs.default_exceptions();

	for (uint_fast8_t i = UINT8_C(0); i < ARRAY_LENGTH; i++) {

		const auto &p = fname[i];
		ifs.open(scan_path + p);
		utils::Line::Line line; ifs >> line;

		switch (i) {
			case PHYSICAL_PACKAGE_ID:
				this->physical_package_id = std::stoi(line);
				break;
		}

		ifs.close();
	}

}