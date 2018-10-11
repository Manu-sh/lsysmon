#include "details/Mounted.hpp"
#include <iostream>
#include <chrono>
#include <thread>

extern "C" {
	#include <unistd.h>
}

using namespace Fs::Details::Mounted;
using namespace std;
using namespace std::chrono_literals;

int main() {

	cout << "checking in progress..." << endl;

	on_mounted_fs([] (const auto &vct){
		for (const auto &e : vct) 
			cout << e << "\n\n" << e.space() << endl;
	});

	while (1) std::this_thread::sleep_for(30s); /* waiting for events */
}
