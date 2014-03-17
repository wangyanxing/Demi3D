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

    /** Load a k2 mdf model
     */
    class DEMI_K2_API DiK2Model
    {
    public:

        DiK2Model(const DiString& path);

        ~DiK2Model();

    public:

        K2Anim&         AddAnim();

        uint32          GetAnimNums() const { return mAnims.size(); }

        K2Anim&         GetAnim(uint32 id) { return mAnims[id]; }
        
        DiModelPtr      GetMesh() {return mMesh;}

        void            SetMesh(DiModelPtr model) { mMesh = model; }
        
        /** e.g. "heroes/aluna"
         */
        const DiString& GetName() const {return mName;}

        DiK2Animation*  GetAnimation() { return mAnimation; }

        DiCullNode*     CreateNode(DiSceneManager* sm);

        DiCullNode*     GetNode() { return mNode; }

    private:

        /** Load the model from a full path, e.g. heroes/aluna
        */
        void            Load(const DiString& path);

    private:

        /// animation configs
        DiVector<K2Anim>  mAnims;

        /// a Demi mesh
        DiModelPtr        mMesh;

        /// the actual animation sets
        DiK2Animation*    mAnimation;
        
        /// full path to this asset folder
        DiString          mName;

        /// Scene node
        DiCullNode*       mNode;
    };
}

#endif
