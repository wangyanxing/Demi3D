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

#ifndef DiLineList_h__
#define DiLineList_h__


#include "TransformUnit.h"
#include "RenderUnit.h"

namespace Demi
{
    class DI_GFX_API DiLineList : public Demi::DiTransformUnit, public Demi::DiRenderUnit
    {
    public:
        DiLineList(void);

        DiLineList(const DiString& name);

        ~DiLineList(void);

    public:

        virtual void            GetWorldTransform(Demi::DiMat4* xform) const;

        void                    AddLine(const Demi::DiVec3 &p0,const Demi::DiVec3 &p1,const Demi::DiColor& color,bool nolock=false);

        void                    AlterLine(int id,const Demi::DiVec3 &p0,const Demi::DiVec3 &p1,const Demi::DiColor& color);

        void                    FlushSourceData();

        void                    FlushSourceData(int beginLine, int endLine);

        void                    CheckResizeLine(size_t maxVerts);

        void                    ClearLine(void);

        int                     GetLineNums();

        const Demi::DiAABB&     GetBoundingBox(void) const;

        void                    AddToBatchGroup(DiRenderBatchGroup* bg);

        DiString&               GetType();

    private:

        struct Vertex
        {
            Demi::DiVec3 pos;
            uint32       color;
        };

        void                    Init();

        void                    AddPoint(const Demi::DiVec3 &p, const Demi::DiColor &color);

    private:

        uint32                  mMaxVerts;

        void*                   mLockedPositions;

        uint32                  mPositionStride;

        void*                   mLockedColors;

        uint32                  mColorStride;

        DiVector<Vertex>        mVertices;
    };

}

#endif
