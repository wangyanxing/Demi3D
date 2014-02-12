


#ifndef DiList_h__
#define DiList_h__


#include <list>

template <typename T>
class DiList : public std::list<T, std::allocator<T> > {};


#endif
