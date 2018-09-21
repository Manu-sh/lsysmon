#include "fs.hpp"

#include <iostream>
using namespace std;

int main() {

	// yes cout isn't syncronized it's just a test
	update_fs([](const auto &m){ for (const auto &e: m) cout << e << endl; });
	while (1); // cout << "SLEEPING" << endl;

}
