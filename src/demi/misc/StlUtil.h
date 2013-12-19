
/********************************************************************
    File:       StlUtil.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

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