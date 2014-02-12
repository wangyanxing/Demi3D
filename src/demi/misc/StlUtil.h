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

#ifndef DiStlUtil_h__
#define DiStlUtil_h__


namespace Demi
{
    namespace stl
    {
        template <typename DiMap>
        inline typename DiMap::mapped_type FindInMap(const DiMap& mapKeyToValue, const typename DiMap::key_type& key, typename DiMap::mapped_type valueDefault)
        {
            typename DiMap::const_iterator it = mapKeyToValue.find (key);
            if (it == mapKeyToValue.end())
                return valueDefault;
            else
                return it->second;
        }
    }
}

#endif
