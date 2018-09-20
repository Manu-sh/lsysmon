#define _GNU_SOURCE

#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include <thread>
#include <mutex>
#include <atomic>

#include <filesystem>

extern "C" {
	#include <sys/inotify.h>
	#include <unistd.h>
	#include <sys/epoll.h>
	#include <mntent.h>
	#include <poll.h>
}

#include <iostream>
using std::cout, std::endl;

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

std::thread{[&callback]{

	constexpr static int EVENTS_SIZE = 1;

	std::vector<Mounted> mnt;
	struct epoll_event ev, events[EVENTS_SIZE];
	int fd[3] {-1};

	if ((fd[0] = inotify_init()) < 0) {
		perror("inotify_init1");
		exit(EXIT_FAILURE);
	}

	if ((fd[1] = inotify_add_watch(fd[0], "traceme", IN_CLOSE_WRITE)) <= 0) {
		perror("inotify_add_watch");
		exit(EXIT_FAILURE);
	}


	fd[2] = epoll_create1(0);

	if (fd[2] == -1) {
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}

	ev.events  = POLLIN;
	ev.data.fd = fd[0];

	if (epoll_ctl(fd[2], EPOLL_CTL_ADD, fd[0], &ev) == -1) {
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}


	while (1) {

		int nfds = epoll_wait(fd[2], events, EVENTS_SIZE, -1);
		// puts("BLOCKING CALL");

		if (nfds == -1) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}

		struct inotify_event buf{};

#if 1
		{
			ssize_t i, readed;
			for (i = 0, readed = 0; i < sizeof buf; i += readed) {
				readed = read(fd[0], &buf + i, sizeof buf - i);
				assert(readed >= 0);
			}

			assert(i == sizeof buf);
		}
#else
		for (ssize_t i = 0, readed = 0; i < sizeof buf; i += readed) {
			readed = read(fd[0], &buf + i, sizeof buf - i);
			assert(readed >= 0);
		}
#endif

		assert(buf.mask & IN_CLOSE_WRITE);

		{ /* get mnted fs */
			struct mntent mbuf;
			char strbuf[4096];
			FILE *file = setmntent("/etc/mtab", "r");

			assert(file);
			mnt.clear();

			for (struct mntent *e; (e = getmntent_r(file, &mbuf, strbuf, sizeof strbuf)); )
				mnt.emplace_back(e);

			endmntent(file);
		}

		callback(mnt);

	}

	inotify_rm_watch(fd[0], fd[1]);
	close(fd[0]); close(fd[1]); close(fd[2]);

}}.detach();

}