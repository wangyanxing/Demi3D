
/********************************************************************
    File:       DList.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    template <class T>
    class DiDListBase
    {
    public:
        DiDListBase()
        {
            mPrev = nullptr;
            mNext = nullptr;
        }

        T mPrev;
        T mNext;
    };

    template <class T> 
    class DiDList
    {
    public:

        T mHead;
        T mTail;

        DiDList()
        {
            mHead = nullptr;
            mTail = nullptr;
        }

        ~DiDList()
        {
            mHead = nullptr;
            mTail = nullptr;
        }

        void push_after(T after, T obj)
        {
            obj->mPrev = after;
            obj->mNext = after->mNext;
            if (!after->mNext)
                mTail = obj;
            else
                after->mNext->mPrev = obj;
            after->mNext = obj;
        }

        void push_before(T before, T obj)
        {
            obj->mPrev = before->mPrev;
            obj->mNext = before;
            if (!before->mPrev)
                mHead = obj;
            else
                before->mPrev->mNext = obj;
            before->mPrev = obj;
        }

        void push_front(T obj)
        {
            if (!mHead)
            {
                mHead = obj;
                mTail = obj;
                obj->mPrev = nullptr;
                obj->mNext = nullptr;
            }
            else
                push_before(mHead, obj);
        }

        void push_back(T obj)
        {
            if (!mTail)
                push_front(obj);
            else
                push_after(mTail, obj);
        }

        bool contains(T obj)
        {
            for (T node = mHead; node; node = node->mNext)
            {
                if (node == obj)
                    return true;
            }
            return false;
        }

        void erase(T obj)
        {
            if (!obj->mPrev)
                mHead = obj->mNext;
            else
                obj->mPrev->mNext = obj->mNext;

            if (!obj->mNext)
                mTail = obj->mPrev;
            else
                obj->mNext->mPrev = obj->mPrev;

            obj->mPrev = nullptr;
            obj->mNext = nullptr;
        }
    };
}