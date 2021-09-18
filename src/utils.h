#ifndef __NU_UTILS_H__
#define __NU_UTILS_H__

#if __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// --------------------------------------------------------------------------------------------------------------------------------

#if _WIN64 || _LP64 || __LP64__ || __amd64__ || __amd64 || __x86_64__ || __x86_64 || _M_AMD_64 || __aarch64__
#define NU_64_BIT
#define NU_BASE_TYPE_BITS 3
#define NU_BASE_REFS_BITS 61
typedef double num_t;
#else
#define NU_32_BIT
#define NU_BASE_TYPE_BITS 3
#define NU_BASE_REFS_BITS 29
typedef float num_t;
#endif

// --------------------------------------------------------------------------------------------------------------------------------

#define xstr(s) str(s)
#define str(s) #s

#define NU_NEW(T) (T*)malloc(sizeof(T))
#define NU_ANEW(T, C) (T*)malloc(sizeof(T)*(C))

// --------------------------------------------------------------------------------------------------------------------------------

#define NU_DEF_FPTR(name, ret, ...) typedef ret (*name)(__VA_ARGS__)

#define NU_FPTR(ret, ...) ret (*)(__VA_ARGS__)

// --------------------------------------------------------------------------------------------------------------------------------

#define NU_LOG(msg) { fprintf(stdout, "LOG: %s\n", msg); }
#define NU_ERR(msg) {  fprintf(stderr, "ERR: %s\n", msg); exit(1); }
#ifndef NDEBUG
#define NU_ASSERT(exp, msg) { if (!(exp)) { fprintf(stderr, "ERR: " __FILE__ ":" xstr(__LINE__) ": ASSERT: " xstr(#exp) " is false: %s\n", msg); exit(1); } }
#else
#define NU_ASSERT(exp, msg) ((void)0)
#endif

// --------------------------------------------------------------------------------------------------------------------------------

inline static uint8_t rol8(uint8_t x, uint8_t bits)
{
    return (x<<bits)|x>>(8-bits);
}

inline static uint16_t rol16(uint16_t x, uint16_t bits)
{
    return (x<<bits)|x>>(16-bits);
}

inline static uint32_t rol32(uint32_t x, uint32_t bits)
{
    return (x<<bits)|x>>(32-bits);
}

inline static uint64_t rol64(uint64_t x, uint64_t bits)
{
    return (x<<bits)|x>>(64-bits);
}

inline static size_t rolN(size_t x, size_t bits)
{
    return (x<<bits)|x>>(sizeof(size_t)-bits);
}

inline static uint8_t ror8(uint8_t x, uint8_t bits)
{
    return (x>>bits)|x<<(8-bits);
}

inline static uint16_t ror16(uint16_t x, uint16_t bits)
{
    return (x>>bits)|x<<(16-bits);
}

inline static uint32_t ror32(uint32_t x, uint32_t bits)
{
    return (x>>bits)|x<<(32-bits);
}

inline static uint64_t ror64(uint64_t x, uint64_t bits)
{
    return (x>>bits)|x<<(64-bits);
}

inline static size_t rorN(size_t x, size_t bits)
{
    return (x>>bits)|x<<(sizeof(size_t)-bits);
}

// --------------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// Returns a unique hash of a given string.
/// </summary>
/// <param name="str">The string to hash.</param>
inline static uint32_t hash32(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0;
    size_t h = 3323198485ul;
    for (;*str;++str)
    {
        h ^= *str;
        h *= 2147483647ul;
        h ^= rol32(h, 15);
        h *= 1540483477ul;
        h ^= ror32(h, 15);
    }
    return h;
}

/// <summary>
/// Returns a unique hash of a given string.
/// </summary>
/// <param name="str">The string to hash.</param>
inline static uint64_t hash64(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0;
    size_t h = 18446744073709551557ull;
    for (;*str;++str)
    {
        h ^= *str;
        h *= 6616326155283851693ull;
        h ^= rol64(h, 47);
        h *= 525201411107845655ull;
        h ^= ror64(h, 47);
    }
    return h;
}

#ifdef NU_64_BIT
#define hashN(str) hash64(str)
#else
#define hashN(str) hash32(str)
#endif

// --------------------------------------------------------------------------------------------------------------------------------

#if __cplusplus
}
#endif

#endif//__NU_UTILS_H__