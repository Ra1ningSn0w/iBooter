#ifndef _PLATFORM_UTIL_H
#define _PLATFORM_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "platform_def.h"

#define fopen(fn, mode) fopen_(fn, mode)
#define rename(oldname, newname) rename_(oldname, newname)
#define remove(path) remove_(path)
#define stat(path, buffer) stat_(path, buffer)

#ifdef __cplusplus
}
#endif

#endif /* _PLATFORM_UTIL_H */
