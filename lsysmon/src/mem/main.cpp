
#include "details/proc/MemInfo.hpp"

#include <iostream>

extern "C" {
	#include <unistd.h>
}

using namespace Mem::Details::Proc::MemInfo;
using namespace std;

int main() {

	cout << get_meminfo() << endl;

}
