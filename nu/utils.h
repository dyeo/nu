#ifndef __NU_UTILS_H__
#define __NU_UTILS_H__

#if __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------------------------------------------------------------------

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "stb_ds.h"

// --------------------------------------------------------------------------------------------------------------------------------

#if _WIN64 || _LP64 || __LP64__ || __amd64__ || __amd64 || __x86_64__ || __x86_64 || _M_AMD_64 || __aarch64__
#    define NU_64_BIT
#    define nusize_t unsigned long long
#    define NU_BASE_TYPE_BITS 3
#    define NU_BASE_REFS_BITS 61
#    define NU_REGISTERS 32
#    define NU_NULLREG 255
#    define NU_REFS_MAX 2305843009213693951
#    define num_t double
#    define i_num_t long
#    define NU_FLP_FMT "%lf"
#    define NU_INT_FMT "%lld"
#    define NU_SZE_FMT "%llu"
#else
#    define NU_32_BIT
#    define nusize_t unsigned long
#    define NU_BASE_TYPE_BITS 3
#    define NU_BASE_REFS_BITS 29
#    define NU_REGISTERS 16
#    define NU_NULLREG 255
#    define NU_REFS_MAX 536870911
#    define num_t float
#    define i_num_t int
#    define NU_FLP_FMT "%f"
#    define NU_INT_FMT "%ld"
#    define NU_SZE_FMT "%lu"
#endif

#define str_t char *

// --------------------------------------------------------------------------------------------------------------------------------

#define xstr(s) str(s)
#define str(s) #s

#define nu_malloc(TYPE) (TYPE *) malloc(sizeof(TYPE))
#define nu_calloc(COUNT, TYPE) (TYPE *) calloc(COUNT, sizeof(TYPE))
#define nu_realloc(PTR, COUNT, TYPE) (TYPE *) realloc(PTR, sizeof(TYPE) * (COUNT))
#define nu_copy(DST, SRC, COUNT, TYPE) (TYPE *) memcpy(DST, SRC, sizeof(TYPE) * (COUNT))
#define nu_move(DST, SRC, COUNT, TYPE) (TYPE *) memmove(DST, SRC, sizeof(TYPE) * (COUNT))

// --------------------------------------------------------------------------------------------------------------------------------

#define NU_FPTR_NEW(NAME, RESULT, ...) typedef RESULT (*NAME)(__VA_ARGS__)

#define NU_FPTR(RESULT, ...) RESULT (*)(__VA_ARGS__)

// --------------------------------------------------------------------------------------------------------------------------------

#define NU_LOG(MESSAGE)                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stdout, "LOG: %s\n", MESSAGE);                                                                         \
    } while (0)

#define NU_ERR(MESSAGE)                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stderr, "ERR: %s\n", MESSAGE);                                                                         \
    } while (0)

#ifndef NDEBUG
#    define NU_ASSERT(EXPRESSION, MESSAGE)                                                                             \
        do                                                                                                             \
        {                                                                                                              \
            if (!(EXPRESSION))                                                                                         \
            {                                                                                                          \
                fprintf(stderr,                                                                                        \
                        "ERR: " __FILE__ ":" xstr(__LINE__) ": ASSERT: " xstr(#EXPRESSION) " is false: %s\n",          \
                        MESSAGE);                                                                                      \
                exit(1);                                                                                               \
            }                                                                                                          \
        } while (0)
#else
#    define NU_ASSERT(EXPRESSION, MESSAGE) ((void) 0)
#endif

// --------------------------------------------------------------------------------------------------------------------------------

inline static uintmax_t nu_from_bytes(const uint8_t *bytes, uint8_t count)
{
    NU_ASSERT(count <= sizeof(uintmax_t), "int too large");
    uintmax_t res = 0;
    for (uint8_t i = 0; i < count; ++i)
    {
        res = (res << 8) | *(bytes + i);
    }
    return res;
}

inline static uintmax_t nu_pop_bytes(uint8_t **bytes, size_t *size, uint8_t count)
{
    NU_ASSERT(count > *size, "could not pop enough bytes");
    if (*size < count)
    {
        return 0;
    }

    uintmax_t res = nu_from_bytes(*bytes, count);

    *bytes += count;
    *size -= count;
    return res;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline static uint8_t rol8(uint8_t x, uint8_t bits)
{
    return (x << bits) | x >> (8 - bits);
}

inline static uint16_t rol16(uint16_t x, uint16_t bits)
{
    return (x << bits) | x >> (16 - bits);
}

inline static uint32_t rol32(uint32_t x, uint32_t bits)
{
    return (x << bits) | x >> (32 - bits);
}

inline static uint64_t rol64(uint64_t x, uint64_t bits)
{
    return (x << bits) | x >> (64 - bits);
}

inline static size_t rolN(size_t x, size_t bits)
{
    return (x << bits) | x >> (sizeof(size_t) - bits);
}

inline static uint8_t ror8(uint8_t x, uint8_t bits)
{
    return (x >> bits) | x << (8 - bits);
}

inline static uint16_t ror16(uint16_t x, uint16_t bits)
{
    return (x >> bits) | x << (16 - bits);
}

inline static uint32_t ror32(uint32_t x, uint32_t bits)
{
    return (x >> bits) | x << (32 - bits);
}

inline static uint64_t ror64(uint64_t x, uint64_t bits)
{
    return (x >> bits) | x << (64 - bits);
}

inline static size_t rorN(size_t x, size_t bits)
{
    return (x >> bits) | x << (sizeof(size_t) - bits);
}

// --------------------------------------------------------------------------------------------------------------------------------

static const uint32_t _FNV_PRIME_32  = 16777619;
static const uint32_t _FNV_OFFSET_32 = 2166136261;

/// @brief Returns a unique hash of a given sequence of bytes (FNV-1a).
/// @param b The sequence of bytes to hash.
/// @param n The number of bytes in the sequence.
/// @return The hash value for b.
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

static const uint64_t _FNV_PRIME_64  = 1099511628211;
static const uint64_t _FNV_OFFSET_64 = 14695981039346656037;

/// @brief Returns a unique hash of a given sequence of bytes (FNV-1a).
/// @param b The sequence of bytes to hash.
/// @param n The number of bytes in the sequence.
/// @return The hash value for b.
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
/// @brief Returns a unique hash of a given sequence of bytes (FNV-1a).
/// @param b The sequence of bytes to hash.
/// @param n The number of bytes in the sequence.
/// @return The hash value for b.
#    define hashN(b, n) hash64(b, n)
#    define _FNV_PRIME_N _FNV_PRIME_64
#    define _FNV_OFFSET_N _FNV_OFFSET_64
#else
/// @brief Returns a unique hash of a given sequence of bytes (FNV-1a).
/// @param b The sequence of bytes to hash.
/// @param n The number of bytes in the sequence.
/// @return The hash value for b.
#    define hashN(b, n) hash32(b, n)
#    define _FNV_PRIME_N _FNV_PRIME_32
#    define _FNV_OFFSET_N _FNV_OFFSET_32
#endif

// --------------------------------------------------------------------------------------------------------------------------------

inline static bool is_int(num_t num)
{
    return floor(num) == num;
}

inline static bool is_float(num_t num)
{
    return floor(num) != num;
}

// --------------------------------------------------------------------------------------------------------------------------------

#if __cplusplus
}
#endif

#endif //__NU_UTILS_H__