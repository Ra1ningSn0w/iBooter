#ifndef _PLATFORM_DEF_H
#define _PLATFORM_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define ARR_CAPACITY(a) (sizeof(a)/sizeof(a[0]))
/*
int utf8_to_wc(const char *in, __wchar_t *out, int cch_buf);
struct FILE* fopen_(const char *filename, const char *mode);
int rename_(const char *oldname, const char *newname);
int remove_(const char *path);
int stat_(const char *path, struct _stat *buffer);

*/
#ifdef __cplusplus
}
#endif

#endif /* _PLATFORM_DEF_H */
