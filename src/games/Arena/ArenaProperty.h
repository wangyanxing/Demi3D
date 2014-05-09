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

#ifndef ArenaProperty_h__
#define ArenaProperty_h__

#include "ArenaPrerequisites.h"
#include "ArenaPropertyID.h"

namespace Demi
{
    #define DEFINE_PROPERTY_ID( Setting )   \
    public:                                 \
    enum ENUM_PROPERTY_ID {                 \
        PropertyID = Setting                \
    };                                      \
    static ArPropertyIDType GetStaticPropertyID()\
    { return (ArPropertyIDType)PropertyID; }\
    ArPropertyIDType GetPropertyID()   \
    { return (ArPropertyIDType)PropertyID; }

    /** Property base class for game entities
     */
    class ArProperty : public DiBase
    {
    protected:

        enum ENUM_ACTIVE_STATUS
        {
            UNACTIVE_STATE,
            ACTIVE_STATE
        };

    public:

        ArProperty()
            : mIsActive(UNACTIVE_STATE)
            , mEntity(nullptr)
        {
        }

        virtual ~ArProperty() {}

        virtual ArPropertyIDType GetPropertyID() { return PROPERTY_NONE; }

    public:

        template< typename T >
        const T* GetEntity()  const
        {
            DI_ASSERT(mEntity);
            return dynamic_cast<T*>(mEntity);
        }

        template< typename T >
        T* GetEntity()
        {
            DI_ASSERT(mEntity);
            return dynamic_cast<T*>(mEntity);
        }

    public:

        virtual void Update(float dt) {};

    public:

        void Activate()
        {
            mIsActive = ACTIVE_STATE;
            OnActivate();
        }

        void Deactivate()
        {
            mIsActive = UNACTIVE_STATE;
            OnDeactivate();
        }

        bool IsActive() const
        {
            if (mIsActive == ACTIVE_STATE)
                return true;
            else
                return false;
        }

    protected:
        virtual void OnActivate() {}
        virtual void OnDeactivate() {}
        virtual void OnEntityReady() {}

        friend class ArEntity;
        friend class ArGameEntity;

        void SetEntity(ArEntity*);

    protected:
        ArGameEntity*     mEntity;

    private:
        ENUM_ACTIVE_STATUS  mIsActive;
    };
}

#endif
