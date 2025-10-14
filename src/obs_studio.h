#pragma once
#include "obs_plugin.h"
class ObsStudio{
    public:
        static ObsStats* stats;
        static int isinit;
        static void atexit_func();
        static int init();
};
