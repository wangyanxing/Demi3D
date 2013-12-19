
/********************************************************************
    File:       List.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include <list>

template <typename T>
class DiList : public std::list<T, std::allocator<T> > {};
