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

/// This file is adapted from Ogre 2.0 (unstable version)

#ifndef __Id_H__
#define __Id_H__

#include "MiscPrerequisites.h"

namespace Demi
{
    /// Big projects with lots, lots of units for very long periods of time (MMORPGs?)
    /// may want to define this to 64-bit
    typedef unsigned int IdType;

    /**
        Usage:
        DI_NEW SceneNode( Id::generateNewId< Node >() )
    */
    class DI_MISC_API Id
    {
    public:
        //This function assumes creation of new objects can't be made from multiple threads!!!
        template <typename T> static IdType generateNewId()
        {
            static IdType g_currentId = 0;
            return g_currentId++;
        }
    };

    class DI_MISC_API IdObject
    {
    private:
        friend struct IdCmp; //Avoid calling getId()
        IdType mId;

    protected:
        /**In the rare case our derived class wants to override our Id
            (normally we don't want that, that's why it's private).
        */
        void _setId( IdType newId )         { mId = newId; }

    public:
        /** We don't call generateNewId() here, to prevent objects in the stack (i.e. local variables)
            that don't need an Id from incrementing the count; which is very dangerous if the caller
            is creating local objects from multiple threads (which should stay safe!).
            Instead our creator should do that.
        */
        IdObject( IdType id ) : mId( id ) {}

        /// Get the unique id of this object
        IdType getId() const                { return mId; }

        bool operator()( const IdObject *left, const IdObject *right )
        {
            return left->mId < right->mId;
        }

        bool operator()( const IdObject &left, const IdObject &right )
        {
            return left.mId < right.mId;
        }
    };

    struct DI_MISC_API IdCmp
    {
        bool operator()( const IdObject *left, const IdType rightId )
        {
            return left->mId < rightId;
        }

        bool operator()( const IdType leftId, const IdObject *right )
        {
            return leftId < right->mId;
        }

        bool operator()( const IdObject *left, const IdObject *right )
        {
            return left->mId < right->mId;
        }

        bool operator()( const IdObject &left, const IdObject &right )
        {
            return left.mId < right.mId;
        }
    };
}

#endif
