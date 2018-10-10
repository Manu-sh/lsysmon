#include <string>
#include <vector>
#include <functional>

namespace Fs::Details {

	struct Mounted {

		using SpaceInfo = struct {
			const uintmax_t capacity, available; 
		};

		SpaceInfo space() const;
		std::string type;
		std::string dir;
		std::string fsname;
	};

	std::ostream & operator<<(std::ostream &os, const Fs::Details::Mounted &m);
	void on_mounted_fs(std::function<void(std::vector<Mounted> &)> callback);
}