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

#ifndef DiIterator_h__
#define DiIterator_h__


namespace Demi 
{
    template <typename T, typename IteratorType, typename ValType>
    class DiIteratorWrapper
    {

    private:
        DiIteratorWrapper();

    protected:
        IteratorType mBegin;
        IteratorType mCurrent;
        IteratorType mEnd;

    public:

        typedef ValType ValueType;
        typedef ValType* PointerType;

        typedef IteratorType iterator;

        typedef IteratorType const_iterator;

        DiIteratorWrapper ( IteratorType start, IteratorType last )
            : mBegin( start ), mCurrent ( start ), mEnd ( last )
        {
        }

        bool HasMoreElements ( ) const
        {
            return mCurrent != mEnd;
        }

        void MoveNext (  )
        {
            ++mCurrent;
        }

        const IteratorType& begin() {return mBegin;}

        IteratorType& current(){return mCurrent;}

        const IteratorType& end() {return mEnd;}
    };


    template <typename T, typename IteratorType>
    class DiVectorIteratorWrapper : public DiIteratorWrapper<T, IteratorType, typename  T::value_type>
    {

    public:
        typedef typename DiIteratorWrapper<T, IteratorType, typename  T::value_type>::ValueType ValueType ; 
        typedef typename DiIteratorWrapper<T, IteratorType, typename  T::value_type>::PointerType PointerType ;


        DiVectorIteratorWrapper ( IteratorType start, IteratorType last )
            : DiIteratorWrapper<T, IteratorType, typename T::value_type>( start, last ) 
        {
        }

        ValueType PeekNext (  ) const
        {
            return *(this->mCurrent);
        }

        PointerType PeekNextPtr (  )  const
        {
            return &(*(this->mCurrent) );
        }

        ValueType GetNext (  ) 
        {
            return *(this->mCurrent++);
        }    

    };


    template <typename T>
    class  DiVectorIterator : public DiVectorIteratorWrapper<T,  typename T::iterator>
    {
    public:

        DiVectorIterator( typename T::iterator start, typename T::iterator last )
            : DiVectorIteratorWrapper<T,  typename T::iterator>(start , last )
        {
        }

        explicit DiVectorIterator( T& c )
            : DiVectorIteratorWrapper<T,  typename T::iterator> ( c.begin(), c.end() )
        {
        }

    };

    template <typename T>
    class DiConstVectorIterator : public DiVectorIteratorWrapper<T,  typename T::const_iterator>{
    public:
        
        DiConstVectorIterator( typename T::const_iterator start, typename T::const_iterator last )
            : DiVectorIteratorWrapper<T,  typename T::const_iterator> (start , last )
        {
        }

        explicit DiConstVectorIterator ( const T& c )
            : DiVectorIteratorWrapper<T,  typename T::const_iterator> (c.begin() , c.end() )
        {
        }
    };

    template <typename T, typename IteratorType>
    class DiMapIteratorWrapper  : public DiIteratorWrapper<T, IteratorType, typename T::mapped_type>
    {

    public:
        
        typedef typename DiIteratorWrapper<T, IteratorType, typename  T::mapped_type>::ValueType ValueType ; 
        
        typedef typename DiIteratorWrapper<T, IteratorType, typename  T::mapped_type>::PointerType PointerType ;    

        typedef typename T::value_type PairType ; 
        
        typedef typename T::key_type KeyType;

        DiMapIteratorWrapper ( IteratorType start, IteratorType last )
            : DiIteratorWrapper<T, IteratorType, typename T::mapped_type>( start, last ) 
        {
        }

        KeyType PeekNextKey(void) const
        {
            return this->mCurrent->first;
        }

        ValueType PeekNextValue (  ) const
        {
            return this->mCurrent->second;
        }

        const PointerType PeekNextValuePtr (  )  const
        {
            return &(this->mCurrent->second);
        }

        ValueType GetNext()
        {
            return ((this->mCurrent++)->second) ;
        }    


    };

    template <typename T>
    class DiMapIterator : public DiMapIteratorWrapper<T,  typename T::iterator>
    {
    public:

        DiMapIterator( typename T::iterator start, typename T::iterator last )
            : DiMapIteratorWrapper<T,  typename T::iterator>(start , last )
        {
        }

        explicit DiMapIterator( T& c )
            : DiMapIteratorWrapper<T,  typename T::iterator> ( c.begin(), c.end() )
        {
        }

    };

    template <typename T>
    class DiConstMapIterator : public DiMapIteratorWrapper<T,  typename T::const_iterator>
    {
    public:

        DiConstMapIterator( typename T::const_iterator start, typename T::const_iterator last )
            : DiMapIteratorWrapper<T,  typename T::const_iterator> (start , last )
        {
        }

        explicit DiConstMapIterator ( const T& c )
            : DiMapIteratorWrapper<T,  typename T::const_iterator> (c.begin() , c.end() )
        {
        }
    };
}

#endif
