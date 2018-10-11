#include "details/Mounted.hpp"
#include <iostream>

extern "C" {
	#include <unistd.h>
}


using namespace Fs::Details::Mounted;
using namespace std;

int main() {

	cout << "checking in progress..." << endl;

	on_mounted_fs([] (const auto &vct){
		for (const auto &e : vct) 
			cout << e << "\n\n" << e.space() << endl;
	});

	while (1); /* waiting for events */

}
