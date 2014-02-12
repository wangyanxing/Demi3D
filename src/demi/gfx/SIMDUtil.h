


#ifndef DiSIMDUtil_h__
#define DiSIMDUtil_h__


#include <xmmintrin.h>

#define __MM_RSQRT_PS(x)    _mm_rsqrt_ps(x)

#define __MM_TRANSPOSE4x4_PS(r0, r1, r2, r3)                                            \
    {                                                                                   \
        __m128 tmp3, tmp2, tmp1, tmp0;                                                  \
                                                                                        \
        /* r00 r01 r02 r03 */                                                           \
        /* r10 r11 r12 r13 */                                                           \
        /* r20 r21 r22 r23 */                                                           \
        /* r30 r31 r32 r33 */                                                           \
                                                                                        \
        tmp0 = _mm_unpacklo_ps(r0, r1);                       /* r00 r10 r01 r11 */     \
        tmp2 = _mm_unpackhi_ps(r0, r1);                       /* r02 r12 r03 r13 */     \
        tmp1 = _mm_unpacklo_ps(r2, r3);                       /* r20 r30 r21 r31 */     \
        tmp3 = _mm_unpackhi_ps(r2, r3);                       /* r22 r32 r23 r33 */     \
                                                                                        \
        r0 = _mm_movelh_ps(tmp0, tmp1);                         /* r00 r10 r20 r30 */   \
        r1 = _mm_movehl_ps(tmp1, tmp0);                         /* r01 r11 r21 r31 */   \
        r2 = _mm_movelh_ps(tmp2, tmp3);                         /* r02 r12 r22 r32 */   \
        r3 = _mm_movehl_ps(tmp3, tmp2);                         /* r03 r13 r23 r33 */   \
    }

// 4x3������ת��
#define __MM_TRANSPOSE4x3_PS(v0, v1, v2)                                                \
    {                                                                                   \
        __m128 tmp0, tmp1, tmp2;                                                        \
                                                                                        \
        /* r00 r01 r02 r10 */                                                           \
        /* r11 r12 r20 r21 */                                                           \
        /* r22 r30 r31 r32 */                                                           \
                                                                                        \
        tmp0 = _mm_shuffle_ps(v0, v2, _MM_SHUFFLE(3,0,3,0));  /* r00 r10 r22 r32 */     \
        tmp1 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(1,0,2,1));  /* r01 r02 r11 r12 */     \
        tmp2 = _mm_shuffle_ps(v1, v2, _MM_SHUFFLE(2,1,3,2));  /* r20 r21 r30 r31 */     \
                                                                                        \
        v0 = _mm_shuffle_ps(tmp0, tmp2, _MM_SHUFFLE(2,0,1,0));  /* r00 r10 r20 r30 */   \
        v1 = _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(3,1,2,0));  /* r01 r11 r21 r31 */   \
        v2 = _mm_shuffle_ps(tmp1, tmp0, _MM_SHUFFLE(3,2,3,1));  /* r02 r12 r22 r32 */   \
    }

// 3x4������ת��
#define __MM_TRANSPOSE3x4_PS(v0, v1, v2)                                                \
    {                                                                                   \
        __m128 tmp0, tmp1, tmp2;                                                        \
                                                                                        \
        /* r00 r10 r20 r30 */                                                           \
        /* r01 r11 r21 r31 */                                                           \
        /* r02 r12 r22 r32 */                                                           \
                                                                                        \
        tmp0 = _mm_shuffle_ps(v0, v2, _MM_SHUFFLE(2,0,3,1));  /* r10 r30 r02 r22 */     \
        tmp1 = _mm_shuffle_ps(v1, v2, _MM_SHUFFLE(3,1,3,1));  /* r11 r31 r12 r32 */     \
        tmp2 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(2,0,2,0));  /* r00 r20 r01 r21 */     \
                                                                                        \
        v0 = _mm_shuffle_ps(tmp2, tmp0, _MM_SHUFFLE(0,2,2,0));  /* r00 r01 r02 r10 */   \
        v1 = _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(3,1,2,0));  /* r11 r12 r20 r21 */   \
        v2 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(3,1,1,3));  /* r22 r30 r31 r32 */   \
    }

#define __MM_SELECT(v, fp)                                                          \
    _mm_shuffle_ps((v), (v), _MM_SHUFFLE((fp),(fp),(fp),(fp)))

#define __MM_ACCUM4_PS(a, b, c, d)                                                  \
    _mm_add_ps(_mm_add_ps(a, b), _mm_add_ps(c, d))

#define __MM_DOT4x4_PS(a0, a1, a2, a3, b0, b1, b2, b3)                              \
    __MM_ACCUM4_PS(_mm_mul_ps(a0, b0), _mm_mul_ps(a1, b1), _mm_mul_ps(a2, b2), _mm_mul_ps(a3, b3))

#define __MM_DOT4x3_PS(r0, r1, r2, r3, v0, v1, v2)                                  \
    __MM_ACCUM4_PS(_mm_mul_ps(r0, v0), _mm_mul_ps(r1, v1), _mm_mul_ps(r2, v2), r3)

// three value 
#define __MM_ACCUM3_PS(a, b, c)                                                     \
    _mm_add_ps(_mm_add_ps(a, b), c)

// dot product
#define __MM_DOT3x3_PS(r0, r1, r2, v0, v1, v2)                                      \
    __MM_ACCUM3_PS(_mm_mul_ps(r0, v0), _mm_mul_ps(r1, v1), _mm_mul_ps(r2, v2))

// a x b + c (vector)
#define __MM_MADD_PS(a, b, c)                                                       \
    _mm_add_ps(_mm_mul_ps(a, b), c)

// linear interpolation
#define __MM_LERP_PS(t, a, b)                                                       \
    __MM_MADD_PS(_mm_sub_ps(b, a), t, a)

// a x b + c (float number)
#define __MM_MADD_SS(a, b, c)                                                       \
    _mm_add_ss(_mm_mul_ss(a, b), c)

// linear interpolation
#define __MM_LERP_SS(t, a, b)                                                       \
    __MM_MADD_SS(_mm_sub_ss(b, a), t, a)

#define __MM_LOAD_PS(p)                                                             \
    (*(__m128*)(p))

#define __MM_STORE_PS(p, v)                                                         \
    (*(__m128*)(p) = (v))


namespace Demi
{
    template <bool aligned = false>
    struct SSEMemoryAccessor
    {
        static FORCEINLINE __m128 load(const float *p)
        {
            return _mm_loadu_ps(p);
        }
        static FORCEINLINE void store(float *p, const __m128& v)
        {
            _mm_storeu_ps(p, v);
        }
    };

    template <>
    struct SSEMemoryAccessor<true>
    {
        static FORCEINLINE const __m128& load(const float *p)
        {
            return __MM_LOAD_PS(p);
        }
        static FORCEINLINE void store(float *p, const __m128& v)
        {
            __MM_STORE_PS(p, v);
        }
    };

    static FORCEINLINE bool _isAlignedForSSE(const void *p)
    {
        return (((size_t)p) & 15) == 0;
    }

#ifdef _DEBUG
#    define __MT_CHECK_STACK_ALIGNED_FOR_SSE()      \
    {                                               \
        __m128 test;                                \
        DI_ASSERT(_isAlignedForSSE(&test));         \
    }
#else 
#    define __MT_CHECK_STACK_ALIGNED_FOR_SSE()
#endif 
}

#endif
