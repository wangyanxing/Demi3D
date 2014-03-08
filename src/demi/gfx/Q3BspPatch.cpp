/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#include "GfxPch.h"
#include "Q3BspPatch.h"

namespace Demi
{
    DiQ3BspVertex::DiQ3BspVertex()
        : mPosition(DiVec3::ZERO)
        , mNormal(DiVec3::ZERO)
    {
        mTexcoord[0] = DiVec2::ZERO;
        mTexcoord[1] = DiVec2::ZERO;
    }

    DiQ3BspVertex::DiQ3BspVertex(float p[3], float texcoord[2][2], float n[3])
    {
        for (int i = 0; i < 3; i++)
            mPosition[i] = p[i];

        for (int i = 0; i < 3; i++)
            mNormal[i] = n[i];

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                this->mTexcoord[i][j] = texcoord[i][j];
    }

    DiQ3BspVertex DiQ3BspVertex::operator+(const DiQ3BspVertex& a)
    {
        DiQ3BspVertex res;

        res.mPosition = this->mPosition + a.mPosition;
        res.mNormal = this->mNormal + a.mNormal;

        for (int i = 0; i < 2; i++)
        {
            res.mTexcoord[i] = this->mTexcoord[i] + a.mTexcoord[i];
        }

        return res;
    }

    DiQ3BspVertex DiQ3BspVertex::operator*(float a)
    {
        DiQ3BspVertex res;

        res.mPosition = this->mPosition * a;
        res.mNormal = this->mNormal * a;

        for (int i = 0; i < 2; i++)
        {
            res.mTexcoord[i] = this->mTexcoord[i] * a;
        }

        return res;
    }

    DiQ3Bezier::DiQ3Bezier()
    {
        mVertex = nullptr;
        mIndex = nullptr;
        mTrianglesPerRow = nullptr;
        mRowIndex = nullptr;
    }

    DiQ3Bezier::~DiQ3Bezier()
    {
        SAFE_ARRAY_DELETE(mVertex);
        SAFE_ARRAY_DELETE(mIndex);
        SAFE_ARRAY_DELETE(mTrianglesPerRow);
        SAFE_ARRAY_DELETE(mRowIndex);
    }

    void DiQ3Bezier::Tessellate(int L)
    {
        const int L1 = L + 1;

        mVertex = new DiQ3BspVertex[L1*L1];
        mNumVertex = L1*L1;

        for (int i = 0; i <= L; ++i)
        {
            float a = (float)i / L;
            float b = 1.0f - a;

            mVertex[i] =
                mControls[0] * (b * b) +
                mControls[3] * (2 * b * a) +
                mControls[6] * (a * a);
        }

        for (int i = 1; i <= L; i++)
        {
            float a = (float)i / L;
            float b = 1.0f - a;

            DiQ3BspVertex temp[3];

            for (int j = 0; j < 3; j++)
            {
                int k = 3 * j;
                temp[j] =
                    mControls[k + 0] * (b * b) +
                    mControls[k + 1] * (2 * b * a) +
                    mControls[k + 2] * (a * a);
            }

            for (int j = 0; j <= L; ++j)
            {
                float a = (float)j / L;
                float b = 1.0f - a;

                mVertex[i * L1 + j] =
                    temp[0] * (b * b) +
                    temp[1] * (2 * b * a) +
                    temp[2] * (a * a);
            }
        }

        mIndex = new unsigned int[L * (L + 1) * 2];
        mNumIndex = L * (L + 1) * 2;

        for (int row = 0; row < L; ++row)
        {
            for (int col = 0; col <= L; ++col)
            {
                mIndex[(row * (L + 1) + col) * 2 + 1] = row       * L1 + col;
                mIndex[(row * (L + 1) + col) * 2] = (row + 1) * L1 + col;
            }
        }

        mTrianglesPerRow = new unsigned int[L];
        mRowIndex = new unsigned int[L];

        for (int row = 0; row < L; ++row)
        {
            mTrianglesPerRow[row] = 2 * L1;
            mRowIndex[row] = row * 2 * L1;
        }

        for (int i = 0; i < L1*L1; i++)
        {
            mVertex[i].mNormal.normalise();
        }
    }

}