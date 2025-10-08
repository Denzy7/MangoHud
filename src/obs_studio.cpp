#include "obs_studio.h"
#include <cstdio>
#include <sys/mman.h>

ObsStats* ObsStudio::stats;
void ObsStudio::atexit_func()
{
    if(!stats)
        return;

    stats->running_mangohud = 0;
    if(!stats->running_obs)
    {
        if(shm_unlink(MANGOHUD_OBS_STATS_SHM) < 0)
            perror("shm_unlink");
    }
}
