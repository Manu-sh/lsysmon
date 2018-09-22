#include "fs.hpp"

#include <iostream>
using namespace std;

int main() {

	// yes cout isn't syncronized it's just a test
	Fs::on_mounted_fs([](const auto &m){

		for (const auto &e: m) {
			cout << e << endl;
			const auto &space = e.space();
			cout << "used:      " << space.get_used()      << endl;
			cout << "available: " << space.get_available() << endl;
			cout << "capacity:  " << space.get_capacity()  << endl;
		} 

	});

	while (1); // cout << "SLEEPING" << endl;

}
