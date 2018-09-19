#pragma once

#include <cstdio>
#include <cstdint>
#include <cassert>
#include <string>
#include <vector>
#include <memory>

#include <thread>
#include <mutex>
#include <atomic>

#include <filesystem>

extern "C" {
	#include <sys/inotify.h>
	#include <sys/epoll.h>
	#include <mntent.h>
}


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
			FILE *file = setmntent("/etc/mtab", "r");
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
			// TODO ~Fs(); inotify_rm_watch() && close()
		private:
			// std::shared_ptr<std::vector<Mounted>> fs;
			std::thread th;
			bool changed;
			int fd[2];
	};

}

Fs::Fs::Fs() {

	assert((fd[0] = inotify_init()) >= 0);
	assert((fd[1] = inotify_add_watch(fd[0], "/etc/mtab", UINT32_C(0))) >= 0);

}
