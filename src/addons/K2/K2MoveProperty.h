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

#ifndef DiK2MoveProperty_h__
#define DiK2MoveProperty_h__

#include "K2Prerequisites.h"
#include "K2Property.h"
#include "K2PathFinder.h"

namespace Demi
{
#define FUSE_PARAM          (0.3f)
#define MAX_POS_NODE_NUMBER (MAX_PATH_NODE_NUMBER + 1)
#define	MAX_SENDPOS	5
#define	MAX_SENDPOSREM (MAX_SENDPOS - 1)
        
#define SUPERSONIC_SPEED   10.0f
#define RUN_SPEED          1.5f

    /** Moving and path finding property
     */
    class DEMI_K2_API DiK2MoveProperty : public DiK2Property
    {
        DEFINE_PROPERTY_ID(PROPERTY_MOVE)

    public:

        DiK2MoveProperty();

        ~DiK2MoveProperty();

    public:

        typedef DiVector<DiK2Pos> WorldPosList;

        void            Update(float dt);

        void            Stop(bool bPassive = false);

        void            TurnTo(const DiK2Pos& pos);

        void            MoveTo(const DiK2Pos& source, const DiK2Pos& target,
                            float fRange = 0.0f);

    private:

        DiK2Pos         GetIntersectionLineRound(const DiK2Pos& linePos1, const DiK2Pos& linePos2, 
                            const DiK2Pos& roundCenter, float fRadius);

        void            ModalityChange(K2ModalityType mod);

    private:

        DiK2Pos         mPosNode[MAX_POS_NODE_NUMBER];

        int             mNumNode;

        int             mNumCurTarget;

        DiK2Pos         mTargetPosition;

        K2WalkMode      mWalkMode;

        K2ModalityType  mModality;

        float           mCurrMoveDistance;

        float           mDistance;

        int             mTargetDirection;

        float           mCurSpeed;

        int32           mTumRate;
    };
}

#endif
