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

#ifndef DiK2Property_h__
#define DiK2Property_h__

#include "K2Prerequisites.h"
#include "K2PropertyID.h"

namespace Demi
{
    #define DEFINE_PROPERTY_ID( Setting )   \
    public:                                 \
    enum ENUM_PROPERTY_ID {                 \
        PropertyID = Setting                \
    };                                      \
    static K2PropertyIDType GetStaticPropertyID()\
    { return (K2PropertyIDType)PropertyID; }\
    K2PropertyIDType GetPropertyID()   \
    { return (K2PropertyIDType)PropertyID; }

    /** Property base class for game entities
     */
    class DEMI_K2_API DiK2Property
    {
    protected:

        enum ENUM_ACTIVE_STATUS
        {
            UNACTIVE_STATE,
            ACTIVE_STATE
        };

    public:

        DiK2Property()
            : mIsActive(UNACTIVE_STATE)
            , mEntity(NULL)
        {
        }

        virtual ~DiK2Property() {}

        virtual K2PropertyIDType GetPropertyID() { return PROPERTY_NONE; }

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

        friend class DiK2Entity;
        friend class DiK2GameEntity;

        void SetEntity(DiK2Entity*);

    protected:
        DiK2GameEntity*     mEntity;

    private:
        ENUM_ACTIVE_STATUS  mIsActive;
    };
}

#endif
