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

#ifndef DiFileStream_h__
#define DiFileStream_h__

#include <iosfwd>
#include <fstream>

namespace Demi
{
    template<class T>
    struct DiFileStream : public T
    {
        DiFileStream(){}

        template<class T1>
        DiFileStream(T1 v)
        {
            setlocale(LC_ALL,"");
            T::open(v);
            setlocale(LC_CTYPE, 0);
        }

        template<class T1,class T2>
        DiFileStream(T1 v1, T2 v2)
        {
            setlocale(LC_ALL,"");
            T::open(v1,v2);
            setlocale(LC_CTYPE, 0);
        }

        template<class T1>
        void open(T1 v)
        {
            setlocale(LC_ALL,"");
            T::open(v);
            setlocale(LC_CTYPE, 0);
        }

        template<class T1,class T2>
        void open(T1 v1, T2 v2)
        {
            setlocale(LC_CTYPE,"");
            T::open(v1,v2);
            setlocale(LC_CTYPE, 0);
        }

    };
}

typedef Demi::DiFileStream<std::ifstream> DiIfstream;
typedef Demi::DiFileStream<std::ofstream> DiOfstream;

#endif
