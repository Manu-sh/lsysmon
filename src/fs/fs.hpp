#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#include <cassert>

#include <string>
#include <vector>
#include <functional>

#include <thread>
#include <filesystem>

extern "C" {
	#include <unistd.h>
	#include <sys/epoll.h>
	#include <mntent.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
}

#include <iostream>
using std::cout, std::endl;


// TODO filesystem details

struct Mounted {

	Mounted(struct mntent *mnt) {
		assert(mnt);
		this->dir    = mnt->mnt_dir;
		this->type   = mnt->mnt_type;
		this->fsname = mnt->mnt_fsname;
	}

	std::string type;
	std::string dir;
	std::string fsname;

};

static std::ostream & operator<<(std::ostream &os, const Mounted &m) {
	return os << "type: \"" << m.type << "\"\ndir: \"" << m.dir << "\"\nfsname: \"" << m.fsname << "\"";
}

void update_fs(std::function<void(std::vector<Mounted> &)> callback) {

std::thread{[&callback]{ /* create a new thread */

	constexpr static int EVENTS_SIZE = 1;
	std::vector<Mounted> mnt;
	char strbuf[4096];

	struct Fd final {
		Fd() = default;
		Fd(int fd) { if (fd < 0) throw std::system_error(errno, std::generic_category()); this->fd = fd; }
		~Fd() { close(fd); }
		int fd = -1;
	} epoll_fd{epoll_create(EVENTS_SIZE)}, file_fd{open("/etc/mtab", O_RDONLY)};

	struct epoll_event ev, events[EVENTS_SIZE];

	ev.events  = EPOLLPRI|EPOLLERR;
	ev.data.fd = file_fd.fd;

	if (epoll_ctl(epoll_fd.fd, EPOLL_CTL_ADD, file_fd.fd, &ev) == -1)
		throw std::system_error(errno, std::generic_category());

	while (1) {

		if (epoll_wait(epoll_fd.fd, events, EVENTS_SIZE, -1) == -1) 
			throw std::system_error(errno, std::generic_category());

		if (events[0].events & EPOLLERR) { /* get mnted fs */

			struct mntent mbuf{};
			FILE *file = setmntent("/etc/mtab", "r");

			assert(file);
			mnt.clear();

			for (struct mntent *e; (e = getmntent_r(file, &mbuf, strbuf, sizeof strbuf)); )
				mnt.emplace_back(e);

			endmntent(file);
		}

		callback(mnt);
	}

}}.detach(); /* detach */

}