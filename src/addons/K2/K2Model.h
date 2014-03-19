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

#ifndef DiK2Model_h__
#define DiK2Model_h__

#include "K2Prerequisites.h"

namespace Demi
{
    

    /** Load a k2 mdf model
     */
    class DEMI_K2_API DiK2Model : public DiBase
    {
    public:

        DiK2Model(const DiString& path);

        ~DiK2Model();

    public:

        DiModelPtr      GetMesh() { return mMesh; }

        /** e.g. "heroes/aluna"
         */
        const DiString& GetName() const {return mName;}

        DiK2Animation*  GetAnimation() { return mAnimation; }

        DiCullNode*     CreateNode(DiSceneManager* sm);

        DiCullNode*     GetNode() { return mNode; }

        void            Update(float deltaTime);

    private:

        void            Load(const DiString& path);

    private:

        /// a Demi mesh
        DiModelPtr        mMesh;

        /// the actual animation sets
        DiK2Animation*    mAnimation;

        /// the skeleton
        DiK2Skeleton*     mSkeleton;
        
        /// full path to this asset folder
        DiString          mName;

        /// Scene node
        DiCullNode*       mNode;

        /// the real data
        DiK2ModelAssetPtr mAsset;
    };
}

#endif
