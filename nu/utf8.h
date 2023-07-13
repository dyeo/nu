#ifndef __NU_UTF8_H__
#define __NU_UTF8_H__

#if __cplusplus
extern "C" {
#endif

#include "utils.h"
#include <stdint.h>
#include <string.h>

/// @brief Decodes the length of a single UTF-8 codepoint.
/// @param c The codepoint to decode.
/// @return 1 for a single-byte char or a continued codepoint (denoted by 10xxxxxx). Otherwise, the number of bytes for any other length codepoint.
inline static size_t cplen(const char c)
{
    return 1 + ((c & 0xC0) == 0xC0) + ((c & 0xE0) == 0xE0) + ((c & 0xF0) == 0xF0);
}

/// @brief Compares two UTF-8 codepoints of variable length.
/// @param l The first variable-length UTF-8 codepoint.</param>
/// @param r The second variable-length UTF-8 codepoint.</param>
/// @return 1 if lengths and codepoints are equivalent, 0 otherwise.
inline static int cpcmp(const char *l, const char *r)
{
    uint8_t n = cplen(*l);
    return (l[0] == r[0])
    && (n < 2 || l[1] == r[1])
    && (n < 3 || l[2] == r[2])
    && (n < 4 || l[3] == r[3]);
}

/// @brief Counts the number of literal characters in a UTF-8 string, not the number of individual codepoints (bytes).
/// @param str The UTF-8 string to count.
/// @return The number of characters in the string. For example, "\xe4\xbd\xa0\xe5\xa5\xbd" (你好) would return 2, not 6.
inline static size_t utfclen(const char *str)
{
    size_t i = 0;
    const char *c = str;
    while (*c != '\0')
    {
        i += 1;
        c += cplen(*c);
    }
    return i;
}

/// @brief Counts the number of bytes and characters respectively in a UTF-8 string.
/// @param str The UTF-8 string to count.
/// @param blen The length of the string in bytes.
/// @param clen The length of the string in characters.
inline static void utfdlen(const char *str, size_t *blen, size_t *clen)
{
    *blen = 0;
    *clen = 0;
    const char *c = str;
    while (*c != '\0')
    {
        size_t cp = cplen(*c);
        (*blen) += cp;
        (*clen) += 1;
        c += cp;
    }
}

/// @brief Duplicates a UTF-8 string, ensuring it's correctly null-terminated.
/// @param str The UTF-8 string to duplicate.
/// @return The new duplicated string. This string must be freed later.
inline static char *utfdup(const char *str)
{
    size_t blen, clen;
    utfdlen(str, &blen, &clen);
    char *dest = nu_calloc(char, ++blen);
    dest[blen-1] = NULL;
    strncpy(dest, str, blen-1);
    return dest;
}

static char *_util_cat(char *dest, char *end, const char *str)
{
    while (dest < end && *str)
    {
        *dest++ = *str++;
    }
    return dest;
}

inline static size_t utfjoin(char *out, size_t out_blen, const char *delim, const char **strings)
{
    char *ptr = out;
    char *strend = out + out_blen;
    while (ptr < strend && *strings)
    {
        ptr = _util_cat(ptr, strend, *strings);
        strings++;
        if (*strings) ptr = _util_cat(ptr, strend, delim);
    }
    return ptr - out;
}

inline static size_t utfcpy(char *s, const char *v, size_t n)
{
    return 0;
}

inline static size_t utfcat(char *dst, const char *src, size_t n)
{
    return 0;
}

/// @brief Counts the number of codepoints in a null-terminated UTF-8 string.
/// If you wish to count the number of characters, rather than codepoints, use utfclen(1).
/// @param str The UTF-8 string.
/// @return 1 if lengths and codepoints are equivalent, 0 otherwise.
#define utflen(str) strlen(str)

#define utfcmp(str1, str2) strcmp(str1, str2) 

#define utfncmp(str1, str2, len) strncmp(str1, str2, len)


#if __cplusplus
}
#endif

#endif//__NU_UTF8_H__
