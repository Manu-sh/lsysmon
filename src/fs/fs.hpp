#define _GNU_SOURCE

#include <cassert>

#include <string>
#include <vector>
#include <functional>

#include <thread>
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


// TODO inotify on procfs seems that doesn't work
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

	struct Inotify final {
	
		struct Fd final {
			Fd() = default;
			Fd(int fd) { if (fd < 0) throw std::system_error(errno, std::generic_category()); this->fd = fd; }
			~Fd() { close(fd); }
			int fd = -1;
		};

		Inotify(const char *filename) {
			if ((this->fd[0].fd = inotify_init1(IN_NONBLOCK)) < 0) 
				throw std::system_error(errno, std::generic_category());

			if ((this->fd[1].fd = inotify_add_watch(fd[0].fd, filename, IN_CLOSE_WRITE)) <= 0)
				throw std::system_error(errno, std::generic_category());
		}

		~Inotify() {

			if (fd[0].fd >= 0 && fd[1].fd >= 0)
				inotify_rm_watch(fd[0].fd, fd[1].fd);

		}

		int get_fd() const { return fd[0].fd; }
		private: Fd fd[2];

	} inotify{"/etc/mtab"};

	std::vector<Mounted> mnt;
	struct epoll_event ev, events[EVENTS_SIZE];
	Inotify::Fd epoll_fd{epoll_create1(0)};

	ev.events  = POLLIN;
	ev.data.fd = inotify.get_fd();

	if (epoll_ctl(epoll_fd.fd, EPOLL_CTL_ADD, inotify.get_fd(), &ev) == -1)
		throw std::system_error(errno, std::generic_category());

	while (1) {

		if (epoll_wait(epoll_fd.fd, events, EVENTS_SIZE, -1) == -1) 
			throw std::system_error(errno, std::generic_category());

		struct inotify_event buf{};

#if 1
		{
			ssize_t i, readed;
			for (i = 0, readed = 0; i < sizeof buf; i += readed) {
				readed = read(inotify.get_fd(), &buf + i, sizeof buf - i);
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

//			cout << "FUCKYOU" << endl;

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

}}.detach(); /* detach */

}