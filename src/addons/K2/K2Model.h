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
#include "Model.h"

namespace Demi
{
    /** Load a k2 mdf model
     */
    class DEMI_K2_API DiK2Model : public DiModel
    {
    public:

        DiK2Model(const DiString& path);

        ~DiK2Model();

    public:

        DiK2Animation*  GetAnimation() { return mAnimation; }

        void            Update(DiCamera*);

    private:

        void            Load(const DiString& path);

        void            PostLoad();

    private:

        /// the actual animation sets
        DiK2Animation*    mAnimation;

        /// the skeleton
        DiK2Skeleton*     mSkeleton;
        
        /// full path to this asset folder
        DiString          mName;

        /// the real data
        DiK2ModelAssetPtr mAsset;

        /// Materials
        DiVector<DiMaterialPtr> mMaterials;
    };
}

#endif
