#ifndef MEM_USAGE_H
#define MEM_USAGE_H

#include <cstddef>

#if defined(_WIN32)
    #include <windows.h>
    #include <psapi.h>
#elif defined(__APPLE__) && defined(__MACH__)
    #include <sys/resource.h>
    #include <unistd.h>
#else
    #include <sys/resource.h>
    #include <unistd.h>
#endif

// Returns memory usage in BYTES
std::size_t get_memory_usage() {
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(),
                             (PROCESS_MEMORY_COUNTERS*)&pmc,
                             sizeof(pmc))) {
        return pmc.WorkingSetSize;   // RSS in bytes
    }
    return 0;

#elif defined(__APPLE__) && defined(__MACH__)
    // macOS: getrusage reports RSS *in bytes* already
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return usage.ru_maxrss;
    }
    return 0;

#else
    // Linux: getrusage reports RSS in *kilobytes*
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return usage.ru_maxrss * 1024UL;  // convert KB → bytes
    }
    return 0;

#endif
}

#endif /* MEM_USAGE_H */
