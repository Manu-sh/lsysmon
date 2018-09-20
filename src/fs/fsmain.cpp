#include "fs.hpp"

#include <iostream>
using namespace std;


int main() {

	update_fs([](const auto &m){ for (const auto &e: m) cout << e << endl; });
	while (1); // cout << "SLEEPING" << endl;

}
