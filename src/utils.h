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

#define nu_malloc(T) (T*)malloc(sizeof(T))
#define nu_calloc(T, C) (T*)calloc(C, sizeof(T))
#define nu_ralloc(T, P, C) (T*)realloc(P, sizeof(T)*(C))

#define nu_copy(T, D, S, C) (T*)memcpy(D, S, sizeof(T)*(C))

// --------------------------------------------------------------------------------------------------------------------------------

#define NU_DEF_FPTR(name, ret, ...) typedef ret (*name)( __VA_ARGS__ )

#define NU_FPTR(ret, ...) ret (*)( __VA_ARGS__ )

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

const static uint32_t _FNV_PRIME_32 = 16777619;
const static uint32_t _FNV_OFFSET_32 = 2166136261;

/// <summary>
/// Returns a unique hash of a given sequence of bytes. (FNV-1a)
/// </summary>
/// <param name="str">The string to hash.</param>
inline static uint32_t hash32(const uint8_t *b, uint32_t n)
{
    if (b == NULL || *b == '\0')
        return 0;
    uint32_t h = _FNV_OFFSET_32;
    uint32_t i;
    for (i = 0; i < n; ++i, ++b)
    {
        h = (h ^ *b) * _FNV_PRIME_32;
    }
    return h;
}

const static uint64_t _FNV_PRIME_64 = 1099511628211;
const static uint64_t _FNV_OFFSET_64 = 14695981039346656037;

/// <summary>
/// Returns a unique hash of a given sequence of bytes. (FNV-1a)
/// </summary>
/// <param name="str">The string to hash.</param>
inline static uint64_t hash64(const uint8_t *b, uint64_t n)
{
    if (b == NULL || *b == '\0')
        return 0;
    uint64_t h = _FNV_OFFSET_64;
    uint64_t i;
    for (i = 0; i < n; ++i, ++b)
    {
        h = (h ^ *b) * _FNV_PRIME_64;
    }
    return h;
}

#ifdef NU_64_BIT
#define hashN(b, n) hash64(b, n)
#define _FNV_PRIME_N _FNV_PRIME_64
#define _FNV_OFFSET_N _FNV_OFFSET_64
#else
#define hashN(b, n) hash32(b, n)
#define _FNV_PRIME_N _FNV_PRIME_32
#define _FNV_OFFSET_N _FNV_OFFSET_32
#endif

// --------------------------------------------------------------------------------------------------------------------------------

#if __cplusplus
}
#endif

#endif//__NU_UTILS_H__