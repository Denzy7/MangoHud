#include "obs_studio.h"
#include <cstdio>
#include <cstring>
#include <spdlog/spdlog.h>
#include "obs_shared.h"
#if __linux__
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#endif

ObsStats* ObsStudio::stats;
int ObsStudio::isinit;
void ObsStudio::atexit_func()
{
    if(!stats)
        return;

    stats->running_mangohud = 0;
    if(!stats->running_obs)
    {
#if __linux__
        char shmpath[1024];
        mangohud_obs_get_shmpath(shmpath, sizeof(shmpath));
        if(shm_unlink(shmpath) < 0)
            perror("shm_unlink");
#endif
    }
}

int ObsStudio::init()
{
    isinit = -1;
#ifdef __linux__
    int fd;
    char shmpath[1024];
    mangohud_obs_get_shmpath(shmpath, sizeof(shmpath));
    if ((fd = shm_open(shmpath, O_CREAT | O_RDWR, 0666)) < 0)
    {
        SPDLOG_ERROR("shm_open {}", strerror(errno));
    }
    if(fd > 0 && ftruncate(fd, sizeof(ObsStats)) < 0)
    {
        SPDLOG_ERROR( "ftruncate {}", strerror(errno));
    }
    if ((stats = static_cast<ObsStats*>(mmap(nullptr, sizeof(ObsStats), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == MAP_FAILED)
    {
        SPDLOG_ERROR( "mmap {}", strerror(errno));
    }else {
        atexit(atexit_func);
        isinit = 1;
    }
#endif
    return isinit;
}
