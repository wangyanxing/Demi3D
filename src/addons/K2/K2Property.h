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
    static ENUM_PROPERTY_ID_TYPE GetStaticPropertyID()\
    { return (ENUM_PROPERTY_ID_TYPE)PropertyID; }\
    ENUM_PROPERTY_ID_TYPE GetPropertyID()   \
    { return (ENUM_PROPERTY_ID_TYPE)PropertyID; }

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

        virtual ENUM_PROPERTY_ID_TYPE getPropertyID() { return PROPERTY_NONE; }

    public:

        template< typename T >
        const T* getEntity()  const
        {
            DI_ASSERT(mEntity);
            return dynamic_cast<T*>(mEntity);
        }

        template< typename T >
        T* getEntity()
        {
            DI_ASSERT(mEntity);
            return dynamic_cast<T*>(mEntity);
        }

    public:

        virtual void update(float dt) {};

    public:

        void activate()
        {
            mIsActive = ACTIVE_STATE;
            onActivate();
        }

        void deactivate()
        {
            mIsActive = UNACTIVE_STATE;
            onDeactivate();
        }

        bool isActive() const
        {
            if (mIsActive == ACTIVE_STATE)
                return true;
            else
                return false;
        }

    protected:
        virtual void onActivate() {}
        virtual void onDeactivate() {}
        virtual void onEntityReady() {}

        friend class DiK2Entity;
        friend class DiK2GameEntity;

        void setEntity(DiK2Entity*);

    protected:
        DiK2GameEntity*     mEntity;

    private:
        ENUM_ACTIVE_STATUS  mIsActive;
    };
}

#endif
