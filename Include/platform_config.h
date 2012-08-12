#ifndef _PLATFORM_CONFIG_H
#define _PLATFORM_CONFIG_H

#if defined(__WIN__) || defined(_WIN32) || defined(_WIN64)
#include "windows/platform_util.h"
#else
#include "linux/platform_util.h"
#endif

#endif /* _PLATFORM_CONFIG_H */
