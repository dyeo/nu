#ifndef __NU_UTF8_H__
#define __NU_UTF8_H__

#if __cplusplus
extern "C" {
#endif

#include "utils.h"
#include <stdint.h>
#include <string.h>

/// <summary>
/// Decodes the length of a single UTF-8 codepoint.
/// </summary>
/// <param name="c">The codepoint to decode.</param>
/// <returns>1 for a single-byte char or a continued codepoint (denoted by 10xxxxxx). Otherwise, the number of bytes for any other length codepoint.</returns>
inline static size_t cplen(const char c)
{
    return 1 + ((c & 0xC0) == 0xC0) + ((c & 0xE0) == 0xE0) + ((c & 0xF0) == 0xF0);
}

/// <summary>
/// Compares two UTF-8 codepoints of variable length.
/// </summary>
/// <param name="l">The first variable-length UTF-8 codepoint.</param>
/// <param name="r">The second variable-length UTF-8 codepoint.</param>
/// <returns>1 if lengths and codepoints are equivalent, 0 otherwise.</returns>
inline static int cpcmp(const char *l, const char *r)
{
    uint8_t n = cplen(*l);
    return (l[0] == r[0])
    && (n < 2 || l[1] == r[1])
    && (n < 3 || l[2] == r[2])
    && (n < 4 || l[3] == r[3]);
}

/// <summary>
/// Counts the number of literal characters in a UTF-8 string, not the number of individual codepoints.
/// </summary>
/// <param name="str">The UTF-8 string to count.</param>
/// <returns>The number of characters in the string. For example, "\xe4\xbd\xa0\xe5\xa5\xbd" (你好) would return 2, not 6.</returns>
inline static size_t utfclen(const char *str)
{
    size_t i = 0;
    const char *c = str;
    while (*c != 0)
    {
        i++;
        c += cplen(*c);
    }
    return i;
}

inline static void utfdlen(const char *str, size_t *blen, size_t *clen)
{
    *blen = 0;
    *clen = 0;
    const char *c = str;
    while (*c != 0)
    {
        size_t cp = cplen(*c);
        (*blen)+=cp;
        (*clen)+=1;
        c += cp;
    }
}

inline static size_t utfcpy(char *dst, const char *src, size_t n)
{
    return 0;
}

inline static size_t utfcat(char *dst, const char *src, size_t n)
{
    return 0;
}

/// <summary>
/// Counts the number of codepoints in a null-terminated UTF-8 string.
/// If you wish to count the number of characters, rather than codepoints, use utfclen(1).
/// </summary>
/// <param name="str">The UTF-8 string.</param>
/// <returns>1 if lengths and codepoints are equivalent, 0 otherwise.</returns>
#define utflen(str) strlen(str)

#define utfcmp(str1, str2) strcmp(str1, str2) 

#define utfncmp(str1, str2, len) strncmp(str1, str2, len)


#if __cplusplus
}
#endif

#endif//__NU_UTF8_H__
