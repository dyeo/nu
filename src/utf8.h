#ifndef __NU_UTF8_H__
#define __NU_UTF8_H__

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

typedef uint8_t utf8;

/// <summary>
/// Decodes the length of a single UTF-8 codepoint.
/// </summary>
/// <param name="c">The codepoint to decode.</param>
/// <returns>1 for a single-byte char or a continued codepoint (denoted by 10xxxxxx). Otherwise, the number of bytes for any other length codepoint.</returns>
inline size_t cplen(const utf8 c)
{
    return 1 + ((c & 0xC0) == 0xC0) + ((c & 0xE0) == 0xE0) + ((c & 0xF0) == 0xF0);
}

/// <summary>
/// Compares two UTF-8 codepoints of variable length.
/// </summary>
/// <param name="l">The first variable-length UTF-8 codepoint.</param>
/// <param name="r">The second variable-length UTF-8 codepoint.</param>
/// <returns>1 if lengths and codepoints are equivalent, 0 otherwise.</returns>
inline int cpcmp(const utf8 *l, const utf8 *r)
{
    uint8_t n = cplen(*l);
    return (l[0] == r[0])
    && (n < 2 || l[1] == r[1])
    && (n < 3 || l[2] == r[2])
    && (n < 4 || l[3] == r[3]);
}

/// <summary>
/// Returns a unique hash of a given string.
/// </summary>
/// <param name="str">The string to hash.</param>
/// <returns>A numerical hash of the given string that should be unique.</returns>
inline size_t utfhash(utf8 *str)
{
    if (str == NULL || *str == '\0')
    {
        return 0;
    }
    size_t h = 5381;
    utf8 c;
    while (c = *str++)
    {
        h = ((h << 5) + h) ^ c;
    }
    return h;
}

/// <summary>
/// Counts the number of literal characters in a UTF-8 string, not the number of individual codepoints.
/// </summary>
/// <param name="str">The UTF-8 string to count.</param>
/// <returns>The number of characters in the string. For example, "\xe4\xbd\xa0\xe5\xa5\xbd" (你好) would return 2, not 6.</returns>
inline size_t utfclen(const utf8 *str)
{
    size_t i = 0;
    utf8 *c = str;
    while (*c != 0)
    {
        i++;
        c += cplen(*c);
    }
    return i;
}

inline size_t utfcpy(utf8 *dst, const utf8 *src, size_t n)
{
    return 0;
}

inline size_t utfcat(utf8 *dst, const utf8 *src, size_t n)
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
