/* sp_int.h
 *
 * Copyright (C) 2006-2020 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

/*
DESCRIPTION
This library provides single precision (SP) integer math functions.

*/
#ifndef WOLF_CRYPT_SP_INT_H
#define WOLF_CRYPT_SP_INT_H

#ifndef WOLFSSL_LINUXKM
#include <limits.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Find smallest type for smallest bits. */
#if UCHAR_MAX == 255
    #define SP_UCHAR_BITS    8

    typedef unsigned char sp_uint8;
    typedef          char  sp_int8;
#elif UCHAR_MAX == 127
    #define SP_UCHAR_BITS    7

    typedef unsigned char sp_uint7;
    typedef          char  sp_int7;
#else
    #error "Size of unsigned short not detected"
#endif

#if USHRT_MAX == 65535
    #define SP_USHORT_BITS    16

    typedef unsigned short sp_uint16;
    typedef          short  sp_int16;
#elif USHRT_MAX == 255
    #define SP_USHORT_BITS    8

    #if USHRT_MAX > UCHAR_MAX
        typedef unsigned short sp_uint8;
        typedef          short  sp_int8;
    #endif
#else
    #error "Size of unsigned short not detected"
#endif

#if UINT_MAX == 4294967295UL
    #define SP_UINT_BITS    32

    typedef unsigned int sp_uint32;
    typedef          int  sp_int32;
#elif UINT_MAX == 65535
    #define SP_UINT_BITS    16

    #if UINT_MAX > USHRT_MAX
        typedef unsigned int sp_uint16;
        typedef          int  sp_int16;
    #endif
#elif UINT_MAX == 255
    #define SP_UINT_BITS    8

    #if UINT_MAX > USHRT_MAX
        typedef unsigned int sp_uint8;
        typedef          int  sp_int8;
    #endif
#else
    #error "Size of unsigned int not detected"
#endif

#if ULONG_MAX == 18446744073709551615UL
    #define SP_ULONG_BITS    64

    typedef unsigned long sp_uint64;
    typedef          long  sp_int64;
#elif ULONG_MAX == 4294967295UL
    #define SP_ULONG_BITS    32

    #if ULONG_MAX > UINT_MAX
        typedef unsigned long sp_uint32;
        typedef          long  sp_int32;
    #endif
#elif ULONG_MAX == 65535
    #define SP_ULONG_BITS    16

    #if ULONG_MAX > UINT_MAX
        typedef unsigned long sp_uint16;
        typedef          long  sp_int16;
    #endif
#else
    #error "Size of unsigned long not detected"
#endif

#if ULLONG_MAX == 18446744073709551615UL
    #define SP_ULLONG_BITS    64

    #if SP_ULLONG_BITS > SP_ULONG_BITS
        typedef unsigned long long sp_uint64;
        typedef          long long  sp_int64;
    #endif
#elif ULLONG_MAX == 4294967295UL
    #define SP_ULLONG_BITS    32

    #if SP_ULLONG_BITS > SP_ULONG_BITS
        typedef unsigned long long sp_uint32;
        typedef          long long  sp_int32;
    #endif
#elif ULLONG_MAX == 65535
    #define SP_ULLONG_BITS    16

    #if SP_ULLONG_BITS > SP_ULONG_BITS
        typedef unsigned long long sp_uint16;
        typedef          long long  sp_int16;
    #endif
#else
    #error "Size of unsigned long long not detected"
#endif


#ifdef WOLFSSL_SP_DIV_32
#define WOLFSSL_SP_DIV_WORD_HALF
#endif

/* Make sure WOLFSSL_SP_ASM build option defined when requested */
#if !defined(WOLFSSL_SP_ASM) && ( \
      defined(WOLFSSL_SP_X86_64_ASM) || defined(WOLFSSL_SP_ARM32_ASM) || \
      defined(WOLFSSL_SP_ARM64_ASM)  || defined(WOLFSSL_SP_ARM_THUMB_ASM) || \
      defined(WOLFSSL_SP_ARM_CORTEX_M_ASM))
    #define WOLFSSL_SP_ASM
#endif


/* Detemine the number of bits to use in each word. */
#ifdef SP_WORD_SIZE
#elif defined(WOLFSSL_DSP_BUILD)
    #define SP_WORD_SIZE 32
#elif defined(WOLFSSL_SP_X86_64_ASM) || defined(WOLFSSL_SP_X86_64)
    #if SP_ULONG_BITS == 64
        #define SP_WORD_SIZE 64
        #define HAVE_INTEL_AVX1
        #define HAVE_INTEL_AVX2
    #elif SP_ULONG_BITS == 32
        #define SP_WORD_SIZE 32
        #undef WOLFSSL_SP_ASM
    #elif SP_ULONG_BITS == 16
        #define SP_WORD_SIZE 16
        #undef WOLFSSL_SP_ASM
    #endif
#elif defined(WOLFSSL_SP_X86)
    #define SP_WORD_SIZE 32
#elif defined(WOLFSSL_SP_ARM64_ASM) || defined(WOLFSSL_SP_ARM64)
    #define SP_WORD_SIZE 64
#elif defined(WOLFSSL_SP_ARM32_ASM) || defined(WOLFSSL_SP_ARM32)
    #define SP_WORD_SIZE 32
#elif defined(WOLFSSL_SP_ARM_THUMB_ASM) || defined(WOLFSSL_SP_ARM_THUMB)
    #define SP_WORD_SIZE 32
#elif defined(WOLFSSL_SP_PPC)
    #define SP_WORD_SIZE 32
#elif defined(WOLFSSL_SP_PPC64)
    #define SP_WORD_SIZE 64
#elif defined(WOLFSSL_SP_MIPS)
    #define SP_WORD_SIZE 32
#elif defined(WOLFSSL_SP_MIPS64)
    #define SP_WORD_SIZE 64
#elif defined(WOLFSSL_SP_RISCV32)
    #define SP_WORD_SIZE 32
#elif defined(WOLFSSL_SP_RISCV64)
    #define SP_WORD_SIZE 64
#elif defined(WOLFSSL_SP_S390X)
    #define SP_WORD_SIZE 64
#endif

/* If no predefined or assembly required size then use maximum available
 * with compiler.
 */
#ifndef SP_WORD_SIZE
    #if defined(NO_64BIT) || !defined(HAVE___UINT128_T)
        #define SP_WORD_SIZE 32
    #else
        #define SP_WORD_SIZE 64
    #endif
#endif

/* Number of bytes in each word. */
#define SP_WORD_SIZEOF  (SP_WORD_SIZE / 8)

/* Define the types used. */
#ifdef HAVE___UINT128_T
    #ifdef __SIZEOF_INT128__
        typedef __uint128_t   sp_uint128;
        typedef  __int128_t    sp_int128;
    #else
        typedef unsigned long sp_uint128   __attribute__ ((mode(TI)));
        typedef          long  sp_int128   __attribute__ ((mode(TI)));
    #endif
    #ifndef WOLFSSL_UINT128_T_DEFINED
        #ifdef __SIZEOF_INT128__
          typedef __uint128_t      uint128_t;
          typedef  __int128_t       int128_t;
        #else
          typedef unsigned long    uint128_t __attribute__ ((mode(TI)));
          typedef          long     int128_t __attribute__ ((mode(TI)));
        #endif
        #define WOLFSSL_UINT128_T_DEFINED
    #endif
#endif

#if SP_WORD_SIZE == 8
    typedef   sp_uint8  sp_int_digit;
    typedef    sp_int8 sp_sint_digit;
    typedef  sp_uint16  sp_int_word;
    typedef   sp_int16  sp_int_sword;

    #define SP_MASK         0xffU
#elif SP_WORD_SIZE == 16
    typedef  sp_uint16  sp_int_digit;
    typedef   sp_int16 sp_sint_digit;
    typedef  sp_uint32  sp_int_word;
    typedef   sp_int32  sp_int_sword;

    #define SP_MASK         0xffffU
#elif SP_WORD_SIZE == 32
    typedef  sp_uint32  sp_int_digit;
    typedef   sp_int32 sp_sint_digit;
    typedef  sp_uint64  sp_int_word;
    typedef   sp_int64  sp_int_sword;

    #define SP_MASK         0xffffffffU
#elif SP_WORD_SIZE == 64
    typedef  sp_uint64  sp_int_digit;
    typedef   sp_int64 sp_sint_digit;
    typedef sp_uint128  sp_int_word;
    typedef  sp_int128  sp_int_sword;

    #define SP_MASK         0xffffffffffffffffUL
#else
    #error Word size not defined
#endif

/* Define an SP digit. */
#ifndef WOLFSSL_SP_ASM
    /* SP C code uses n/m bits and therefore needs a signed type. */
    #if SP_WORD_SIZE == 8
        typedef  sp_int8  sp_digit;
    #elif SP_WORD_SIZE == 16
        typedef  sp_int16 sp_digit;
    #elif SP_WORD_SIZE == 32
        typedef  sp_int32 sp_digit;
    #elif SP_WORD_SIZE == 64
        typedef  sp_int64 sp_digit;
    #endif
#else
    /* SP ASM code uses full size and needs an unsigned type. */
    #if SP_WORD_SIZE == 8
        typedef sp_uint8  sp_digit;
    #elif SP_WORD_SIZE == 16
        typedef sp_uint16 sp_digit;
    #elif SP_WORD_SIZE == 32
        typedef sp_uint32 sp_digit;
    #elif SP_WORD_SIZE == 64
        typedef sp_uint64 sp_digit;
    #endif
#endif

/** Number of bits in a half a word.  */
#define SP_HALF_SIZE    (SP_WORD_SIZE / 2)
/** Maximum value that can be held in a half a word.  */
#define SP_HALF_MAX     (((sp_digit)1 << SP_HALF_SIZE) - 1)
/** Maximum value that can be held in a word.  */
#define SP_DIGIT_MAX    SP_MASK
/* Number of bits to shift to divide by word size. */
#if SP_WORD_SIZE == 8
    #define SP_WORD_SHIFT   3
#elif SP_WORD_SIZE == 16
    #define SP_WORD_SHIFT   4
#elif SP_WORD_SIZE == 32
    #define SP_WORD_SHIFT   5
#elif SP_WORD_SIZE == 64
    #define SP_WORD_SHIFT   6
#endif
/* Mask of word size. */
#define SP_WORD_MASK    (SP_WORD_SIZE - 1)


#if defined(WOLFSSL_HAVE_SP_ECC) && defined(WOLFSSL_SP_NONBLOCK)
/* Non-blocking ECC operation context. */
typedef struct sp_ecc_ctx {
    #ifdef WOLFSSL_SP_384
    byte data[48*80]; /* stack data */
    #else
    byte data[32*80]; /* stack data */
    #endif
} sp_ecc_ctx_t;
#endif

#if defined(WOLFSSL_SP_MATH) || defined(WOLFSSL_SP_MATH_ALL)
#include <wolfssl/wolfcrypt/random.h>

#ifdef SP_INT_BITS
    /* Calculate number of digits to have in an sp_int based maximum size of
     * numbers in bits that will be used.
     * Double the size to hold multiplication result.
     * Add one to accommodate extra digit used by sp_mul(), sp_mulmod(), sp_sqr(), and sp_sqrmod().
     */
    #define SP_INT_DIGITS                                                      \
        ((((SP_INT_BITS + (SP_WORD_SIZE - 1)) * 2 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
#endif

#ifndef SP_INT_DIGITS
    /* Calculate number of digits to have in an sp_int based on features
     * compiled in.
     */
    #if !defined(WOLFSSL_HAVE_SP_RSA) && !defined(WOLFSSL_HAVE_SP_DH) && \
        !defined(WOLFSSL_HAVE_SP_ECC)
        #if !defined(NO_RSA) || !defined(NO_DH) || !defined(NO_DSA)
            #define SP_INT_DIGITS        (((6144 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #elif defined(HAVE_ECC)
            #define SP_INT_DIGITS   \
                    (((2 * ( 521 + SP_WORD_SIZE) + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #elif !defined(NO_PWDBASED) && defined(WOLFSSL_SHA512)
            #define SP_INT_DIGITS        ((( 512 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #else
            #define SP_INT_DIGITS        ((( 256 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #endif
    #elif !defined(WOLFSSL_HAVE_SP_RSA) && !defined(WOLFSSL_HAVE_SP_DH)
        #ifdef WOLFSSL_SP_MATH_ALL
            #define SP_INT_DIGITS   \
                    (((2 * ( 521 + SP_WORD_SIZE) + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #elif defined(WOLFSSL_SP_384)
            #define SP_INT_DIGITS        ((( 768 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #else
            #define SP_INT_DIGITS        ((( 512 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #endif
    #elif defined(WOLFSSL_SP_4096)
        #if defined(WOLFSSL_HAVE_SP_DH)
            #define SP_INT_DIGITS        (((8192 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #else
            #define SP_INT_DIGITS        (((4096 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #endif
    #elif !defined(WOLFSSL_SP_NO_3072)
        #if defined(WOLFSSL_HAVE_SP_DH)
            #define SP_INT_DIGITS        (((6144 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #else
            #define SP_INT_DIGITS        (((3072 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #endif
    #else
        #if defined(WOLFSSL_HAVE_SP_DH)
            #define SP_INT_DIGITS        (((4096 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #else
            #define SP_INT_DIGITS        (((2048 + SP_WORD_SIZE) / SP_WORD_SIZE) + 1)
        #endif
    #endif
#endif

#ifndef SP_INT_MAX_BITS
    /* Convert number digits to number of bits. */
    #define SP_INT_MAX_BITS     (SP_INT_DIGITS * SP_WORD_SIZE)
#endif

#if SP_WORD_SIZE < 32
    /* Maximum number of digits in a number to mul or sqr. */
    #define SP_MUL_SQR_DIGITS       (SP_INT_MAX_BITS / 2 / SP_WORD_SIZE)
    /* Maximum value of partial in mul/sqr. */
    #define SP_MUL_SQR_MAX_PARTIAL  \
                                 (SP_MUL_SQR_DIGITS * ((1 << SP_WORD_SIZE) - 1))
    /* Maximim value in an sp_int_word. */
    #define SP_INT_WORD_MAX         ((1 << (SP_WORD_SIZE * 2)) - 1)

    #if SP_MUL_SQR_MAX_PARTIAL > SP_INT_WORD_MAX
        /* The sum of the partials in the multiplicaiton/square can exceed the
         * size of a word. This will overflow the word and loose data.
         * Use an implementation that handles carry after every add and uses an
         * extra temporary word for overflowing high word.
         */
        #define SP_WORD_OVERFLOW
    #endif
#endif


/* For debugging only - format string for different digit sizes. */
#if SP_WORD_SIZE == 64
    #if SP_ULONG_BITS == 64
        #define SP_PRINT_FMT       "%016lx"
    #else
        #define SP_PRINT_FMT       "%016llx"
    #endif
#elif SP_WORD_SIZE == 32
    #if SP_UINT_BITS == 32
        #define SP_PRINT_FMT       "%08x"
    #else
        #define SP_PRINT_FMT       "%08lx"
    #endif
#elif SP_WORD_SIZE == 16
    #define SP_PRINT_FMT       "%04x"
#elif SP_WORD_SIZE == 8
    #define SP_PRINT_FMT       "%02x"
#endif

#ifndef NO_FILESYSTEM
/* Output is formatted to be used with script that checks calculations. */

/* Print out a number in big endian. */
#ifndef WOLFSSL_SP_INT_NEGATIVE
/* Print out a positive multi-precision number.
 *
 * @param  [in]  a  SP integer to print.
 * @param  [in]  s  String that describes the use of the number.
 */
#define sp_print(a, s)                                  \
    do {                                                \
        int ii;                                         \
        fprintf(stderr, "%s=0x0", s);                   \
        for (ii = (a)->used-1; ii >= 0; ii--) {         \
            fprintf(stderr, SP_PRINT_FMT, (a)->dp[ii]); \
        }                                               \
        fprintf(stderr, "\n");                          \
    }                                                   \
    while (0)
#else
/* Print out a multi-precision number.
 *
 * @param  [in]  a  SP integer to print.
 * @param  [in]  s  String that describes the use of the number.
 */
#define sp_print(a, s)                                  \
    do {                                                \
        int ii;                                         \
        fprintf(stderr, "%s=0x", s);                    \
        if ((a)->sign == MP_NEG) {                      \
            fprintf(stderr, "-");                       \
        }                                               \
        fprintf(stderr, "0");                           \
        for (ii = (a)->used-1; ii >= 0; ii--) {         \
            fprintf(stderr, SP_PRINT_FMT, (a)->dp[ii]); \
        }                                               \
        fprintf(stderr, "\n");                          \
    }                                                   \
    while (0)
#endif

/* Print out a single multi-precision digit.
 *
 * @param  [in]  a  SP integer digit to print.
 * @param  [in]  s  String that describes the use of the number.
 */
#define sp_print_digit(a, s)                            \
    do {                                                \
        fprintf(stderr, "%s=0x0", s);                   \
        fprintf(stderr, SP_PRINT_FMT, a);               \
        fprintf(stderr, "\n");                          \
    }                                                   \
    while (0)

/* Print out an integer.
 *
 * @param  [in]  a  Number to print.
 * @param  [in]  s  String that describes the use of the number.
 */
#define sp_print_int(a, s)                              \
    do {                                                \
        fprintf(stderr, "%s=0x0%x\n", s, a);            \
    }                                                   \
    while (0)

#else

    /* No filesystem, no output
     * TODO: Use logging API?
     */
    #define sp_print(a, s)
    #define sp_print_digit(a, s)
    #define sp_print_int(a, s)

#endif

/* Returns whether multi-precision number is odd
 *
 * Assumes a is not NULL.
 *
 * @param  [in]  a  SP integer to check.
 * @return 1 when odd.
 * @return 0 when even.
 */
#define sp_isodd(a)      (((a)->used != 0) && ((a)->dp[0] & 1))
/* Returns whether multi-precision number is even
 *
 * Assumes a is not NULL.
 *
 * @param  [in]  a  SP integer to check.
 * @return 1 when even.
 * @return 0 when odd.
 */
#define sp_iseven(a)     (((a)->used != 0) && (((a)->dp[0] & 1) == 0))
/* Returns whether multi-precision number has the value zero.
 *
 * Assumes a is not NULL.
 *
 * @param  [in]  a  SP integer to check.
 * @return 1 when zero.
 * @return 0 when not zero.
 */
#define sp_iszero(a)     ((a)->used == 0)

#ifndef WOLFSSL_SP_INT_NEGATIVE
/* Returns whether multi-precision number has the value one.
 *
 * Assumes a is not NULL.
 *
 * @param  [in]  a  SP integer to check.
 * @return 1 when one.
 * @return 0 when not one.
 */
#define sp_isone(a)      (((a)->used == 1) && ((a)->dp[0] == 1))
#else
/* Returns whether multi-precision number has the value of positive one.
 *
 * Assumes a is not NULL.
 *
 * @param  [in]  a  SP integer to check.
 * @return 1 when one.
 * @return 0 when not one.
 */
#define sp_isone(a)      \
    (((a)->used == 1) && ((a)->dp[0] == 1) && ((a)->sign == MP_ZPOS))
#endif

#ifndef WOLFSSL_SP_INT_NEGATIVE
/* Returns whether multi-precision number has the value 'd'.
 *
 * Assumes a is not NULL.
 *
 * @param  [in]  a  SP integer to check.
 * @param  [in]  d  SP integer digit.
 * @return 1 when one.
 * @return 0 when not one.
 */
#define sp_isword(a, d)  \
    ((((d) == 0) && sp_iszero(a)) || (((a)->used == 1) && ((a)->dp[0] == (d))))
#else
/* Returns whether multi-precision number has the value 'd'.
 *
 * Assumes a is not NULL.
 *
 * @param  [in]  a  SP integer to check.
 * @param  [in]  d  SP integer digit.
 * @return 1 when one.
 * @return 0 when not one.
 */
#define sp_isword(a, d)                                                     \
    ((((d) == 0) && sp_iszero(a)) ||                                        \
     (((a)->used == 1) && ((a)->dp[0] == (d)) && ((a)->sign == MP_ZPOS)))
#endif

#ifndef WOLFSSL_SP_INT_NEGATIVE
/* Calculate the absolute value of the multi-precision number.
 *
 * Negative support not compiled in so just copies.
 *
 * @param  [in]   a  SP integer to calculate absolute value of.
 * @param  [out]  r  SP integer to hold result.
 *
 * @return  MP_OKAY on success.
 * @return  MP_VAL when a or r is NULL.
 */
#define sp_abs(a, b)     sp_copy(a, b)
/* Returns whether multi-precision number is negative.
 *
 * Negative support not compiled in so always returns 0 (false).
 *
 * @param  [in]  a  SP integer to check.
 * @param  [in]  d  SP integer digit.
 * @return 0 indicating not negative always.
 */
#define sp_isneg(a)      (0)
#else
/* Returns whether multi-precision number is negative.
 *
 * Assumes a is not NULL.
 *
 * @param  [in]  a  SP integer to check.
 * @param  [in]  d  SP integer digit.
 * @return 1 when negative.
 * @return 0 when not negative.
 */
#define sp_isneg(a)      ((a)->sign == MP_NEG)
#endif

/* Updates the used count to exclude leading zeros.
 *
 * Assumes a is not NULL.
 *
 * @param  [in]  a  SP integer to update.
 */
#define sp_clamp(a)                                               \
    do {                                                          \
        int ii;                                                   \
        for (ii = a->used - 1; ii >= 0 && a->dp[ii] == 0; ii--) { \
        }                                                         \
        a->used = ii + 1;                                         \
    } while (0)

/* Check the compiled and linked math implementation are the same.
 * Use the number of bits in a digit as indication of how code was compiled.
 *
 * @return  1 when the number of bits are the same.
 * @return  0 when the number of bits are differnt.
 */
#define CheckFastMathSettings()   (SP_WORD_SIZE == CheckRunTimeFastMath())


/* The number of bytes to a sp_int with 'cnt' digits. */
#define MP_INT_SIZEOF(cnt) \
    (sizeof(sp_int) - (SP_INT_DIGITS - (cnt)) * sizeof(sp_int_digit))
/* The address of the next sp_int after one with 'cnt' digits. */
#define MP_INT_NEXT(t, cnt) \
        (sp_int*)(((byte*)(t)) + MP_INT_SIZEOF(cnt))

/**
 * A reuslt of NO.
 * e.g. Is prime? NO.
 */
#define MP_NO      0
/**
 * A reuslt of YES.
 * e.g. Is prime? YES.
 */
#define MP_YES     1

#ifdef WOLFSSL_SP_INT_NEGATIVE
/** Number is 0/positive. */
#define MP_ZPOS    0
/** Number is negative. */
#define MP_NEG     1
#endif

/** Radix is base 10 or decimal. */
#define MP_RADIX_DEC     10
/** Radix is base 16 or hexadecimal. */
#define MP_RADIX_HEX     16

/** Result of comparison is that the first number is greater than second. */
#define MP_GT    1
/** Result of comparison is they are equal. */
#define MP_EQ    0
/** Result of comparison is that the first number is less than second. */
#define MP_LT    -1

/** Error value on success. */
#define MP_OKAY   0
/** Error value when dynamic memory allocation fails. */
#define MP_MEM   -2
/** Error value when value passed is not able to be used. */
#define MP_VAL   -3
/** Error value when non-blocking operation is returning after partial
 * completion.
 */
#define FP_WOULDBLOCK -4

/* Number of bits in each word/digit. */
#define DIGIT_BIT  SP_WORD_SIZE
/* Mask of all used bits in word/digit. */
#define MP_MASK    SP_MASK


#ifdef HAVE_WOLF_BIGINT
    /* Raw big integer as a big-endian byte array.
     *
     * Useful for when using hardware - canonical format.
     */
    typedef struct WC_BIGINT {
        /* Dynamically allocated buffer that is big-endian byte array. */
        byte*   buf;
        /* Length of buffer in bytes. */
        word32  len;
        /* Hint for heap used to allocate buffer. */
        void*   heap;
    } WC_BIGINT;

    /* Ensure WC_BIGINT defined once. */
    #define WOLF_BIGINT_DEFINED
#endif


/**
 * SP integer.
 *
 * dp at end so user can allocate a smaller amount and set size.
 */
typedef struct sp_int {
    /** Number of words that contain data.  */
    int used;
    /** Maximum number of words in data.  */
    int size;
#ifdef WOLFSSL_SP_INT_NEGATIVE
    /** Indicates whether number is 0/positive or negative.  */
    int sign;
#endif
#ifdef HAVE_WOLF_BIGINT
    /** Unsigned binary (big endian) representation of number. */
    struct WC_BIGINT raw;
#endif
    /** Data of number.  */
    sp_int_digit dp[SP_INT_DIGITS];
} sp_int;

/* Mulit-precision integer type is SP integer type. */
typedef sp_int       mp_int;
/* Mulit-precision integer digit type is SP integer digit type.
 * Type is unsigned.
 */
typedef sp_int_digit mp_digit;


/* Include the maths operations that are not implementation specific. */
#include <wolfssl/wolfcrypt/wolfmath.h>

/*
 * Function prototypes.
 */

MP_API int sp_init(sp_int* a);
MP_API int sp_init_size(sp_int* a, int size);
MP_API int sp_init_multi(sp_int* n1, sp_int* n2, sp_int* n3, sp_int* n4,
                         sp_int* n5, sp_int* n6);
MP_API void sp_free(sp_int* a);
MP_API int sp_grow(sp_int* a, int l);

MP_API void sp_zero(sp_int* a);
MP_API void sp_clear(sp_int* a);
MP_API void sp_forcezero(sp_int* a);
MP_API int sp_init_copy (sp_int* r, sp_int* a);

MP_API int sp_copy(sp_int* a, sp_int* r);
MP_API int sp_exch(sp_int* a, sp_int* b);
MP_API int sp_cond_swap_ct(mp_int * a, mp_int * b, int c, int m);

#ifdef WOLFSSL_SP_INT_NEGATIVE
MP_API int sp_abs(sp_int* a, sp_int* b);
#endif
#ifdef WOLFSSL_SP_MATH_ALL
MP_API int sp_cmp_mag(sp_int* a, sp_int* b);
#endif
MP_API int sp_cmp(sp_int* a, sp_int* b);

MP_API int sp_is_bit_set(sp_int* a, unsigned int b);
MP_API int sp_count_bits(sp_int* a);
#if defined(HAVE_ECC) && defined(HAVE_COMP_KEY)
MP_API int sp_cnt_lsb(sp_int* a);
#endif
MP_API int sp_leading_bit(sp_int* a);
MP_API int sp_set_bit(sp_int* a, int i);
MP_API int sp_2expt(sp_int* a, int e);

MP_API int sp_set(sp_int* a, sp_int_digit d);
MP_API int sp_set_int(sp_int* a, unsigned long n);
MP_API int sp_cmp_d(sp_int* a, sp_int_digit d);
MP_API int sp_add_d(sp_int* a, sp_int_digit d, sp_int* r);
MP_API int sp_sub_d(sp_int* a, sp_int_digit d, sp_int* r);
MP_API int sp_mul_d(sp_int* a, sp_int_digit d, sp_int* r);
MP_API int sp_div_d(sp_int* a, sp_int_digit d, sp_int* r, sp_int_digit* rem);
#if defined(WOLFSSL_SP_MATH_ALL) || (defined(HAVE_ECC) && \
    defined(HAVE_COMP_KEY))
MP_API int sp_mod_d(sp_int* a, const sp_int_digit d, sp_int_digit* r);
#endif
#if defined(WOLFSSL_SP_MATH_ALL) && defined(HAVE_ECC)
MP_API int sp_div_2_mod_ct (sp_int* a, sp_int* b, sp_int* c);
MP_API int sp_div_2(sp_int* a, sp_int* r);
#endif

MP_API int sp_add(sp_int* a, sp_int* b, sp_int* r);
MP_API int sp_sub(sp_int* a, sp_int* b, sp_int* r);
#ifdef WOLFSSL_SP_MATH_ALL
MP_API int sp_addmod(sp_int* a, sp_int* b, sp_int* m, sp_int* r);
MP_API int sp_submod(sp_int* a, sp_int* b, sp_int* m, sp_int* r);
#endif
#if defined(WOLFSSL_SP_MATH_ALL) && defined(HAVE_ECC)
MP_API int sp_submod_ct (sp_int* a, sp_int* b, sp_int* c, sp_int* d);
MP_API int sp_addmod_ct (sp_int* a, sp_int* b, sp_int* c, sp_int* d);
#endif

MP_API int sp_lshd(sp_int* a, int s);
MP_API void sp_rshd(sp_int* a, int c);
MP_API void sp_rshb(sp_int* a, int n, sp_int* r);

#ifdef WOLFSSL_SP_MATH_ALL
MP_API int sp_div(sp_int* a, sp_int* d, sp_int* r, sp_int* rem);
#endif
MP_API int sp_mod(sp_int* a, sp_int* m, sp_int* r);

MP_API int sp_mul(sp_int* a, sp_int* b, sp_int* r);
MP_API int sp_mulmod(sp_int* a, sp_int* b, sp_int* m, sp_int* r);

MP_API int sp_invmod(sp_int* a, sp_int* m, sp_int* r);
#if defined(WOLFSSL_SP_MATH_ALL) && defined(HAVE_ECC)
MP_API int sp_invmod_mont_ct(sp_int* a, sp_int* m, sp_int* r, sp_int_digit mp);
#endif

MP_API int sp_exptmod_ex(sp_int* b, sp_int* e, int digits, sp_int* m,
                         sp_int* r);
MP_API int sp_exptmod(sp_int* b, sp_int* e, sp_int* m, sp_int* r);
#if defined(WOLFSSL_SP_MATH_ALL) || defined(WOLFSSL_HAVE_SP_DH)
MP_API int sp_exptmod_nct(sp_int* b, sp_int* e, sp_int* m, sp_int* r);
#endif

#ifdef WOLFSSL_SP_MATH_ALL
MP_API int sp_div_2d(sp_int* a, int e, sp_int* r, sp_int* rem);
MP_API int sp_mod_2d(sp_int* a, int e, sp_int* r);
MP_API int sp_mul_2d(sp_int* a, int e, sp_int* r);
#endif

MP_API int sp_sqr(sp_int* a, sp_int* r);
MP_API int sp_sqrmod(sp_int* a, sp_int* m, sp_int* r);

MP_API int sp_mont_red(sp_int* a, sp_int* m, sp_int_digit mp);
MP_API int sp_mont_setup(sp_int* m, sp_int_digit* rho);
MP_API int sp_mont_norm(sp_int* norm, sp_int* m);

MP_API int sp_unsigned_bin_size(sp_int* a);
MP_API int sp_read_unsigned_bin(sp_int* a, const byte* in, word32 inSz);
MP_API int sp_to_unsigned_bin(sp_int* a, byte* out);
MP_API int sp_to_unsigned_bin_len(sp_int* a, byte* out, int outSz);
#ifdef WOLFSSL_SP_MATH_ALL
MP_API int sp_to_unsigned_bin_at_pos(int o, sp_int* a, unsigned char* out);
#endif

MP_API int sp_read_radix(sp_int* a, const char* in, int radix);
MP_API int sp_tohex(sp_int* a, char* str);
MP_API int sp_todecimal(mp_int* a, char* str);
#ifdef WOLFSSL_SP_MATH_ALL
MP_API int sp_toradix(mp_int* a, char* str, int radix);
MP_API int sp_radix_size(mp_int* a, int radix, int* size);
#endif

MP_API int sp_rand_prime(sp_int* r, int len, WC_RNG* rng, void* heap);
MP_API int sp_prime_is_prime(mp_int* a, int t, int* result);
MP_API int sp_prime_is_prime_ex(mp_int* a, int t, int* result, WC_RNG* rng);
MP_API int sp_gcd(sp_int* a, sp_int* b, sp_int* r);
MP_API int sp_lcm(sp_int* a, sp_int* b, sp_int* r);

WOLFSSL_API word32 CheckRunTimeFastMath(void);


/* Map mp functions to SP math versions. */
/* Different name or signature. */
#define mp_mul_2(a, r)                      sp_mul_2d(a, 1, r)
#define mp_div_3(a, r, rem)                 sp_div_d(a, 3, r, rem)
#define mp_rshb(A,x)                        sp_rshb(A,x,A)
#define mp_is_bit_set(a,b)                  sp_is_bit_set(a,(unsigned int)b)
#define mp_montgomery_reduce                sp_mont_red
#define mp_montgomery_setup                 sp_mont_setup
#define mp_montgomery_calc_normalization    sp_mont_norm

/* Macros mappings. */
#define mp_isodd                            sp_isodd
#define mp_iseven                           sp_iseven
#define mp_iszero                           sp_iszero
#define mp_isone                            sp_isone
#define mp_isword                           sp_isword
#define mp_abs                              sp_abs
#define mp_isneg                            sp_isneg
#define mp_clamp                            sp_clamp

/* One to one mappings. */
#define mp_init                             sp_init
#define mp_init_size                        sp_init_size
#define mp_init_multi                       sp_init_multi
#define mp_free                             sp_free
#define mp_grow                             sp_grow
#define mp_zero                             sp_zero
#define mp_clear                            sp_clear
#define mp_forcezero                        sp_forcezero
#define mp_copy                             sp_copy
#define mp_init_copy                        sp_init_copy
#define mp_exch                             sp_exch
#define mp_cond_swap_ct                     sp_cond_swap_ct
#define mp_cmp_mag                          sp_cmp_mag
#define mp_cmp                              sp_cmp
#define mp_count_bits                       sp_count_bits
#define mp_cnt_lsb                          sp_cnt_lsb
#define mp_leading_bit                      sp_leading_bit
#define mp_set_bit                          sp_set_bit
#define mp_2expt                            sp_2expt
#define mp_set                              sp_set
#define mp_set_int                          sp_set_int
#define mp_cmp_d                            sp_cmp_d
#define mp_add_d                            sp_add_d
#define mp_sub_d                            sp_sub_d
#define mp_mul_d                            sp_mul_d
#define mp_div_d                            sp_div_d
#define mp_mod_d                            sp_mod_d
#define mp_div_2_mod_ct                     sp_div_2_mod_ct
#define mp_div_2                            sp_div_2
#define mp_add                              sp_add
#define mp_sub                              sp_sub
#define mp_addmod                           sp_addmod
#define mp_submod                           sp_submod
#define mp_addmod_ct                        sp_addmod_ct
#define mp_submod_ct                        sp_submod_ct
#define mp_lshd                             sp_lshd
#define mp_rshd                             sp_rshd
#define mp_div                              sp_div
#define mp_mod                              sp_mod
#define mp_mul                              sp_mul
#define mp_mulmod                           sp_mulmod
#define mp_invmod                           sp_invmod
#define mp_invmod_mont_ct                   sp_invmod_mont_ct
#define mp_exptmod_ex                       sp_exptmod_ex
#define mp_exptmod                          sp_exptmod
#define mp_exptmod_nct                      sp_exptmod_nct
#define mp_div_2d                           sp_div_2d
#define mp_mod_2d                           sp_mod_2d
#define mp_mul_2d                           sp_mul_2d
#define mp_sqr                              sp_sqr
#define mp_sqrmod                           sp_sqrmod

#define mp_unsigned_bin_size                sp_unsigned_bin_size
#define mp_read_unsigned_bin                sp_read_unsigned_bin
#define mp_to_unsigned_bin                  sp_to_unsigned_bin
#define mp_to_unsigned_bin_len              sp_to_unsigned_bin_len
#define mp_to_unsigned_bin_at_pos           sp_to_unsigned_bin_at_pos
#define mp_read_radix                       sp_read_radix
#define mp_tohex                            sp_tohex
#define mp_todecimal                        sp_todecimal
#define mp_toradix                          sp_toradix
#define mp_radix_size                       sp_radix_size

#define mp_rand_prime                       sp_rand_prime
#define mp_prime_is_prime                   sp_prime_is_prime
#define mp_prime_is_prime_ex                sp_prime_is_prime_ex
#define mp_gcd                              sp_gcd
#define mp_lcm                              sp_lcm

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WOLF_CRYPT_SP_H */
