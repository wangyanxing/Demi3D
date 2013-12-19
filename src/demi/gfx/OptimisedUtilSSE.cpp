
#include "OptimisedUtil.h"
#include "SIMDUtil.h"

namespace Demi
{
    class  DiOptimisedUtilSSE : public DiOptimisedUtil
    {
    public:
        DiOptimisedUtilSSE(void){}

        void             ConcatenateAffineMatrices(
                                const DiMat4& baseMatrix,
                                const DiMat4* srcMatrices,
                                DiMat4* dstMatrices,
                                size_t numMatrices);
    };

    void DiOptimisedUtilSSE::ConcatenateAffineMatrices( const DiMat4& baseMatrix,
        const DiMat4* pSrcMat, DiMat4* pDstMat, size_t numMatrices )
    {
        __MT_CHECK_STACK_ALIGNED_FOR_SSE();

        DI_ASSERT(_isAlignedForSSE(pSrcMat));
        DI_ASSERT(_isAlignedForSSE(pDstMat));

        __m128 m0 = _mm_loadu_ps(baseMatrix[0]);
        __m128 m1 = _mm_loadu_ps(baseMatrix[1]);
        __m128 m2 = _mm_loadu_ps(baseMatrix[2]);
        __m128 m3 = _mm_loadu_ps(baseMatrix[3]); //

        for (size_t i = 0; i < numMatrices; ++i)
        {
            __m128 s0 = __MM_LOAD_PS((*pSrcMat)[0]);
            __m128 s1 = __MM_LOAD_PS((*pSrcMat)[1]);
            __m128 s2 = __MM_LOAD_PS((*pSrcMat)[2]);

            ++pSrcMat;

            __m128 t0, t1, t2, t3;

            t0 = _mm_mul_ps(__MM_SELECT(m0, 0), s0);
            t1 = _mm_mul_ps(__MM_SELECT(m0, 1), s1);
            t2 = _mm_mul_ps(__MM_SELECT(m0, 2), s2);
            t3 = _mm_mul_ps(m0, m3);
            __MM_STORE_PS((*pDstMat)[0], __MM_ACCUM4_PS(t0,t1,t2,t3));

            t0 = _mm_mul_ps(__MM_SELECT(m1, 0), s0);
            t1 = _mm_mul_ps(__MM_SELECT(m1, 1), s1);
            t2 = _mm_mul_ps(__MM_SELECT(m1, 2), s2);
            t3 = _mm_mul_ps(m1, m3);
            __MM_STORE_PS((*pDstMat)[1], __MM_ACCUM4_PS(t0,t1,t2,t3));

            t0 = _mm_mul_ps(__MM_SELECT(m2, 0), s0);
            t1 = _mm_mul_ps(__MM_SELECT(m2, 1), s1);
            t2 = _mm_mul_ps(__MM_SELECT(m2, 2), s2);
            t3 = _mm_mul_ps(m2, m3);
            __MM_STORE_PS((*pDstMat)[2], __MM_ACCUM4_PS(t0,t1,t2,t3));

            __MM_STORE_PS((*pDstMat)[3], m3);

            ++pDstMat;
        }
    }

    //////////////////////////////////////////////////////////////////////////

    extern DiOptimisedUtil* GetOptimisedUtilSSE(void)
    {
        static DiOptimisedUtilSSE s_kOptimisedUtilSSE;
        return &s_kOptimisedUtilSSE;
    }
}