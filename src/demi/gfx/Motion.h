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

#ifndef DiMotion_h__
#define DiMotion_h__


#include "Asset.h"

namespace Demi
{
    class DI_GFX_API DiMotion : public DiAsset
    {
    public:

        DiMotion(const DiString& name);

        ~DiMotion();

        typedef DiMap<DiString,DiAnimation*> AnimationList;

        typedef DiMapIterator<AnimationList> AnimationListIterator;

        typedef DiConstMapIterator<AnimationList> ConstAnimationListIterator;

    public:

        DiAssetType                 GetAssetType() const {return TYPE;}

        bool                        LoadingComplete() const;

        bool                        Load(DiDataStreamPtr data);

        bool                        Load();

        DiSkeleton*                 GetSkeleton() {return mSkeleton;}
                                    
        DiSkeleton*                 GetSkeleton()const {return mSkeleton;}

        DiAttachSet*                GetAttachSet() {return mAttachSet;}

        DiAttachSet*                GetAttachSet()const {return mAttachSet;}

        bool                        HasSkeleton() const{return mSkeleton!=NULL;}

        bool                        HasAttachSet() const{return mAttachSet!=NULL;}

        bool                        HasAnimation(const DiString& name);

        bool                        RenameAnimation(const DiString& name,const DiString& strNewName);

        size_t                      GetNumAnimations()const {return mAnimationList.size();}

        DiAnimation*                GetAnimation(const DiString& name);

        DiAnimation*                GetAnimation(const DiString& name)const;

        void                        AssociateNodeAnimToSkeleton(DiSkeleton* skeleton);

        void                        AssociateNodeAnimToAttachSet(DiAttachSet* attachset);

        DiSkeleton*                 CreateSkeleton();

        DiAttachSet*                CreateAttachSet();

        DiAnimation*                CreateAnimation(const DiString& name, float length);

        AnimationListIterator       GetAnimations();

        ConstAnimationListIterator  GetAnimations() const;

        void                        ApplySkeletonAnimation( DiSkeletonInstance* skel, DiClipControllerSet* clipset );

    public:

        static DiAssetType          TYPE;

    protected:

        DiSkeleton*                 mSkeleton;

        DiAttachSet*                mAttachSet;

        AnimationList               mAnimationList;
    };
}

#endif
