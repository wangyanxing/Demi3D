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

#ifndef DiK2Asset_h__
#define DiK2Asset_h__

#include "K2Prerequisites.h"
#include "Asset.h"

namespace Demi
{
    /** Frame events, play sounds etc.
    */
    struct K2FrameEvent
    {
        int frame;
        DiString cmd;
    };

    struct K2Anim
    {
        DiString name;
        DiString clip;
        bool     loop;
        float    fps;
        int      numframes;
        int      loopbackframe;

        DiVector<K2FrameEvent> events;
    };

    struct K2KeyFrames
    {
        DiStrHash<DiVector<Trans>> boneFrames;
    };

    /** Model and animation clips
     */
    class DEMI_K2_API DiK2ModelAsset : public DiAsset
    {
    public:

        /** Load the model from a path (relative), e.g. heroes/aluna
        */
        DiK2ModelAsset(const DiString& name);

        ~DiK2ModelAsset();
        
        friend class DiK2MdfSerial;

    public:

        const DiString&  GetAssetType() const { return TYPE; }

        bool             LoadingComplete() const;

        /** We don't use this function
         */
        bool             Load(DiDataStreamPtr data);

        bool             Load();

        K2Anim&          AddAnim()
        {
            mAnims.push_back(K2Anim());
            return mAnims.back();
        }

        DiMeshPtr        GetMesh() { return mModel; }

        void             SetMesh(DiMeshPtr mesh) { mModel = mesh; }

        uint32           GetAnimNums() const { return mAnims.size(); }

        K2Anim&          GetAnim(uint32 id) { return mAnims[id]; }

        DiK2BonesDataPtr GetBoneData() { return mBoneData; }

        void             AddKeyFrame(const DiString& animName, K2KeyFrames* kf);

        void             CreateClipInstance(DiK2Animation* anim);
        
        const DiString&  GetBaseFolder() const {return mBaseFolder;}

    public:

        static DiString  TYPE;

    private:

        DiMeshPtr        mModel;

        DiK2BonesDataPtr mBoneData;

        /// animation configurations
        DiVector<K2Anim> mAnims;

        DiStrHash<K2KeyFrames*> mKeyFrames;
        
        /// file (it's usually "high.model")
        DiString         mModelFile;
        
        /// file folder
        /// e.g. "heroes/aluna"
        DiString         mBaseFolder;

        /// Tree config file
        DiString         mTreeFile;
    };
}

#endif