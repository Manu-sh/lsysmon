#pragma once

#include <string>
#include <vector>
#include <functional>

namespace Fs::Details::Mounted {

	struct Mounted {

		struct SpaceInfo {
			const uintmax_t capacity, available; /* in bytes */
		};

		SpaceInfo space() const;

		std::string type;
		std::string dir;
		std::string fsname;
	};

	std::ostream & operator<<(std::ostream &os, const Mounted::SpaceInfo &s);
	std::ostream & operator<<(std::ostream &os, const Mounted &m);

	/* scsi only */
	void on_mounted_fs(std::function<void(std::vector<Mounted> &)> callback);
}