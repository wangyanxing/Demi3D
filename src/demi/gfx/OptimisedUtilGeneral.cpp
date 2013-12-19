
#include "OptimisedUtil.h"

namespace Demi
{
    class DiOptimisedUtilGeneral : public DiOptimisedUtil
    {
    public:

        void             ConcatenateAffineMatrices(
                            const DiMat4& baseMatrix,
                            const DiMat4* srcMatrices,
                            DiMat4* dstMatrices,
                            size_t numMatrices);
    };

    void DiOptimisedUtilGeneral::ConcatenateAffineMatrices( const DiMat4& baseMatrix, 
        const DiMat4* pSrcMat, DiMat4* pDstMat, size_t numMatrices )
    {
        const DiMat4& m = baseMatrix;

        for (size_t i = 0; i < numMatrices; ++i)
        {
            const DiMat4& s = *pSrcMat;
            DiMat4& d = *pDstMat;

            d[0][0] = m[0][0] * s[0][0] + m[0][1] * s[1][0] + m[0][2] * s[2][0];
            d[0][1] = m[0][0] * s[0][1] + m[0][1] * s[1][1] + m[0][2] * s[2][1];
            d[0][2] = m[0][0] * s[0][2] + m[0][1] * s[1][2] + m[0][2] * s[2][2];
            d[0][3] = m[0][0] * s[0][3] + m[0][1] * s[1][3] + m[0][2] * s[2][3] + m[0][3];

            d[1][0] = m[1][0] * s[0][0] + m[1][1] * s[1][0] + m[1][2] * s[2][0];
            d[1][1] = m[1][0] * s[0][1] + m[1][1] * s[1][1] + m[1][2] * s[2][1];
            d[1][2] = m[1][0] * s[0][2] + m[1][1] * s[1][2] + m[1][2] * s[2][2];
            d[1][3] = m[1][0] * s[0][3] + m[1][1] * s[1][3] + m[1][2] * s[2][3] + m[1][3];

            d[2][0] = m[2][0] * s[0][0] + m[2][1] * s[1][0] + m[2][2] * s[2][0];
            d[2][1] = m[2][0] * s[0][1] + m[2][1] * s[1][1] + m[2][2] * s[2][1];
            d[2][2] = m[2][0] * s[0][2] + m[2][1] * s[1][2] + m[2][2] * s[2][2];
            d[2][3] = m[2][0] * s[0][3] + m[2][1] * s[1][3] + m[2][2] * s[2][3] + m[2][3];

            d[3][0] = 0;
            d[3][1] = 0;
            d[3][2] = 0;
            d[3][3] = 1;

            ++pSrcMat;
            ++pDstMat;
        }
    }

    extern DiOptimisedUtil* GetOptimisedUtilGeneral(void)
    {
        static DiOptimisedUtilGeneral s_kOptimisedUtilGeneral;
        return &s_kOptimisedUtilGeneral;
    }
}