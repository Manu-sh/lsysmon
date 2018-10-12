#include "src/Linux.hpp"

#include <iostream>
#include <stdexcept>

extern "C" {
	#include <unistd.h>
}

using namespace std;

int main() try {

} catch (exception &e) {
	cerr << e.what() << endl;
}