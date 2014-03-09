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

#ifndef DiSceneCuller_h__
#define DiSceneCuller_h__

#include "GfxPrerequisites.h"
#include <functional>

namespace Demi
{
    /** Culling information class, member for every scene node
     */
    class DiCullUnit : public enable_shared_from_this<DiCullUnit>
    {
    public:
        
        DiCullUnit(DiCullNode* parentNode, DiSceneCuller* culler)
            :mParent(parentNode)
            ,mCuller(culler){}
        
        virtual         ~DiCullUnit(){}
        
    public:
        
        /** Culling update
         */
        virtual void    Update() = 0;
        
        virtual bool    IsIn(const DiAABB& aabb) {return true;}
        
        DiCullNode*     mParent;
        
        DiSceneCuller*  mCuller;
    };
    
    /** Scene culling manager, member for every scene manager
     */
    class DiSceneCuller
    {
    public:
        
        DiSceneCuller(DiSceneManager* sm)
            :mSceneManager(sm){}
        
        virtual                 ~DiSceneCuller(){}
        
    public:
        
        virtual DiCullUnitPtr   CreateUnit(DiCullNode* node) = 0;
        
        virtual void            Cull(DiCamera* camera) = 0;
        
        virtual void            UpdateUnit(DiCullUnitPtr unit) {}
        
        virtual void            RemoveUnit(DiCullUnitPtr unit) {}

        virtual void            LoadScene(const DiString& scene) = 0;
        
    protected:
        
        DiSceneManager*         mSceneManager;
    };
    
    /** Scene culler factory
     */
    class DI_GFX_API DiSceneCullerFactory
    {
    public:
        
        DiSceneCullerFactory(DiSceneManager* sm);
        
        ~DiSceneCullerFactory(){}
        
        DiSceneCuller*          CreateSceneCuller(const DiString& name);
        
        void                    Register(const DiString& name,
                                    const std::function<DiSceneCuller*()>& entry);
        
    private:
        
        DiMap<DiString,std::function<DiSceneCuller*()> > mEntries;
    };
}

#endif