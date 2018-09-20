#pragma once

#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>

#include <string>
#include <vector>
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


// #define MTAB "/etc/mtab"
#define MTAB "traceme"

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


namespace Fs {

	class Fs {

		inline std::vector<Mounted> get_mounted() {

			std::vector<Mounted> vmnt;
			FILE *file = setmntent(MTAB, "r");
			assert(file);

			for (struct mntent *mnt; (mnt = getmntent(file)); )
				vmnt.emplace_back(mnt);

			endmntent(file);
			return vmnt;
		}


		public:
			Fs();
			bool is_changed() const;
			// std::shared_ptr<const std::vector<const Mounted>> mounted_fs() const;

			~Fs();
			// TODO ~Fs(); inotify_rm_watch() && close()
		private:
			// std::shared_ptr<std::vector<Mounted>> fs;
			std::thread th;
			bool changed;
			int fd[3];
	};

}



Fs::Fs::~Fs() {
	// inotify_rm_watch();
	close(fd[0]);
}

Fs::Fs::Fs() {

	if ((fd[0] = inotify_init()) < 0) {
		perror("inotify_init1");
		exit(EXIT_FAILURE);
	}

	if ((fd[1] = inotify_add_watch(fd[0], MTAB, IN_CLOSE_WRITE)) <= 0) {
		perror("inotify_add_watch");
		exit(EXIT_FAILURE);
	}

	struct epoll_event ev, events[1];

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

	// TODO

	while (1) {

		int nfds = epoll_wait(fd[2], events, sizeof events / sizeof events[0], -1);
		puts("BLOCKING CALL");

		if (nfds == -1) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}

		char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
		memset(buf, 0, sizeof buf);
		ssize_t readed = read(fd[0], buf, sizeof buf);

		const struct inotify_event *event;
		for (char *ptr = buf; ptr < buf + readed; ptr += sizeof(struct inotify_event) + event->len) {

			event = (const struct inotify_event *)ptr;

			printf("fd: %d\n", event->wd);
			if (event->mask & IN_CLOSE_WRITE) 
				puts("IN_CLOSE_WRITE");
		}

	}

}
