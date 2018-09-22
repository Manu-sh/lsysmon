#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#include "../../utils/utils.hpp"

#include <cassert>
#include <string>
#include <vector>
#include <functional>

#include <thread>

#include <stdexcept>
#include <filesystem>

extern "C" {
	#include <unistd.h>
	#include <mntent.h>
	#include <fcntl.h>
	#include <sys/epoll.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/sysmacros.h>
}


namespace Fs {

	struct Mounted {

		struct SpaceInfo {
			SpaceInfo(const std::filesystem::space_info &s)
			: capacity{s.capacity}, available{s.available} {}
			std::string get_capacity()  const { return units::byte::to_iec(capacity); }
			std::string get_used()      const { return units::byte::to_iec(capacity - available); }
			std::string get_available() const { return units::byte::to_iec(available); }
			const uintmax_t capacity, available;
		};

		Mounted(struct mntent *mnt) {
			assert(mnt);
			this->dir    = mnt->mnt_dir;
			this->type   = mnt->mnt_type;
			this->fsname = mnt->mnt_fsname;
		}

		SpaceInfo space() const { return SpaceInfo(std::filesystem::space(this->dir)); }

		std::string type;
		std::string dir;
		std::string fsname;
	};

	static std::ostream & operator<<(std::ostream &os, const Fs::Mounted &m) {
		return os << "type: \"" << m.type << "\"\ndir: \"" << m.dir << "\"\nfsname: \"" << m.fsname << "\"";
	}

	void on_mounted_fs(std::function<void(std::vector<Mounted> &)> callback);
}

// TODO mv .cpp ?
void Fs::on_mounted_fs(std::function<void(std::vector<Mounted> &)> callback) {

	using utils::Linux::Fd;
	using Epoll_event = struct epoll_event;

	// https://www.kernel.org/doc/Documentation/admin-guide/devices.txt
	const static auto &is_scsi_dev = [] (const char *mnt_point) {

		struct stat buf;

		if (stat(mnt_point, &buf) == -1)
			throw std::system_error(errno, std::generic_category());

		return major(buf.st_dev) == 0x8U;
	};

	const static auto &get_mounted = [](std::vector<Mounted> &mnt) {

		struct mntent mbuf{};
		char strbuf[4096];
		FILE *file = setmntent("/etc/mtab", "r");

		assert(file);
		mnt.clear();

		for (struct mntent *e; (e = getmntent_r(file, &mbuf, strbuf, sizeof strbuf)); ) {

			if (!is_scsi_dev(e->mnt_dir))
				continue;

			mnt.emplace_back(e);
		}

		endmntent(file);
	};

	std::thread{[&callback]{ /* create a new thread */

		constexpr static int EVENTS_SIZE = 1;
		std::vector<Fs::Mounted> mnt;
		Fd epoll_fd{epoll_create(EVENTS_SIZE)}, file_fd{open("/etc/mtab", O_RDONLY)};
		Epoll_event ev, events[EVENTS_SIZE];

		/* monitor /etc/mtab for changes (since actually mtab is a symlink to /proc/self/mounts inotify dosn't work) */
		ev.events = EPOLLPRI|EPOLLERR; 
		ev.data.fd = file_fd.fd;

		if (epoll_ctl(epoll_fd.fd, EPOLL_CTL_ADD, file_fd.fd, &ev) == -1)
			throw std::system_error(errno, std::generic_category());

		do {

			get_mounted(mnt);
			callback(mnt);

poll:
			if (epoll_wait(epoll_fd.fd, events, EVENTS_SIZE, -1) == -1) 
				throw std::system_error(errno, std::generic_category());

			if (!(events[0].events & EPOLLERR))
				goto poll;

		} while(1);

	}}.detach(); /* detach */

}