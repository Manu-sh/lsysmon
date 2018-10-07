#pragma once

#include <vector>
#include <string>

namespace Cpu::Details::Proc::CpuInfo {

	/* they are guaranteed to be existing and non-empty */

	enum {
		PROCESSOR,
		VENDOR_ID,
		CPU_FAMILY,
		MODEL,
		PHYSICAL_ID,
		SIBILINGS,
		CORE_ID,
		CPU_CORES,
		CPU_MHZ,
		CACHE_SIZE,
		MODEL_NAME,
		FLAGS,

		ARRAY_LENGTH /* LENGTH = N + 1 */
	};

	struct CpuEntry {
		std::vector<std::string> entries;
	};

	std::ostream & operator<<(std::ostream &os, const CpuEntry &ref);
	std::vector<CpuEntry> get_cpu_info();

	/*
		example of output for vector<CpuEntry>[0]:

			PROCESSOR   0
			VENDOR_ID   GenuineIntel
			CPU_FAMILY  6
			MODEL       78
			PHYSICAL_ID 0
			SIBILINGS   4
			CORE_ID     0
			CPU_CORES   2
			CPU_MHZ     2300.006
			CACHE_SIZE  4096 KB
			MODEL_NAME  Intel(R) Core(TM) i5-6260U CPU @ 1.80GHz
			FLAGS       fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbepopcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp

	*/

}