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

#ifndef ArenaMoveProperty_h__
#define ArenaMoveProperty_h__

#include "ArenaPrerequisites.h"
#include "ArenaProperty.h"
#include "K2PathFinder.h"

namespace Demi
{
#define FUSE_PARAM          (0.3f)
#define MAX_POS_NODE_NUMBER (MAX_PATH_NODE_NUMBER + 1)
#define	MAX_SENDPOS	5
#define	MAX_SENDPOSREM (MAX_SENDPOS - 1)

    /** Moving and path finding property
     */
    class ArMoveProperty : public ArProperty
    {
        DEFINE_PROPERTY_ID(PROPERTY_MOVE)

    public:

        ArMoveProperty();

        ~ArMoveProperty();

    public:

        typedef DiVector<DiK2Pos> WorldPosList;

        void            Update(float dt);

        void            Stop(bool bPassive = false);

        void            TurnTo(const DiK2Pos& pos);

        void            MoveTo(const DiK2Pos& source, const DiK2Pos& target,
                            float fRange = 0.0f);

        float           GetSpeed() const { return mSpeed; }

        void            SetSpeed(float val) { mSpeed = val; }
        
        int             GetTurnSpeed() const { return mTurnSpeed; }
        
        void            SetTurnSpeed(int val) { mTurnSpeed = val; }

        ArWalkMode      GetWalkMode() { return mWalkMode; }

        void            StopAt(const DiK2Pos& pos);

    private:

        DiK2Pos         GetIntersectionLineRound(const DiK2Pos& linePos1, const DiK2Pos& linePos2, 
                            const DiK2Pos& roundCenter, float fRadius);

        void            ModalityChange(ArModalityType mod);

    private:

        DiK2Pos         mPosNode[MAX_POS_NODE_NUMBER];

        int             mNumNode;

        int             mNumCurTarget;

        DiK2Pos         mTargetPosition;

        ArWalkMode      mWalkMode;

        ArModalityType  mModality;

        float           mCurrMoveDistance;

        float           mDistance;

        int             mTargetDirection;

        float           mSpeed;
        
        int             mTurnSpeed;
    };
}

#endif
