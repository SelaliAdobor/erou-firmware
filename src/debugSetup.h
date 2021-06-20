#pragma once
#include "RemoteDebug.h"
#include "config.h"
static RemoteDebug Debug;

inline void debugSetup()
{
    Debug.begin(config::hostname, Debug.VERBOSE);
    Debug.setResetCmdEnabled(true); // Enable the reset command

    Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)

    Debug.showColors(true);
}

inline void debugLoop()
{
    Debug.handle();
}