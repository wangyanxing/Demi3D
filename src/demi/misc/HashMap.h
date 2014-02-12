


#ifndef DiHashMap_h__
#define DiHashMap_h__

// hash map type
#define DEMI_UNORDED    1
#define DEMI_HASH       2

// Note - not in the original STL, but exists in SGI STL and STLport
// For gcc 4.3 see http://gcc.gnu.org/gcc-4.3/changes.html
#if (DEMI_COMPILER == DEMI_COMPILER_GNUC) && !defined(STLPORT)
#   if DEMI_COMPILER_VER >= 430
#       include <tr1/unordered_map>
#       include <tr1/unordered_set>
#       define DEMI_HASH_LIB DEMI_UNORDED
#   else
#       include <ext/hash_map>
#       include <ext/hash_set>
#       define DEMI_HASH_LIB DEMI_HASH
#   endif
#elif (DEMI_COMPILER == DEMI_COMPILER_CLANG)
#   if defined(_LIBCPP_VERSION)
#       include <unordered_map>
#       include <unordered_set>
#       define DEMI_HASH_LIB DEMI_UNORDED
#   else
#       include <tr1/unordered_map>
#       include <tr1/unordered_set>
#       define DEMI_HASH_LIB DEMI_UNORDED
#   endif
#else
#   if (DEMI_COMPILER == DEMI_COMPILER_MSVC) && !defined(STLPORT) && DEMI_COMPILER_VER >= 1600 // VC++ 10.0
#    	include <unordered_map>
#    	include <unordered_set>
#       define DEMI_HASH_LIB DEMI_UNORDED
#	else
#   	include <hash_set>
#   	include <hash_map>
#       define DEMI_HASH_LIB DEMI_HASH
#	endif
#endif

#if DEMI_COMPILER == DEMI_COMPILER_GNUC && DEMI_COMPILER_VER >= 310 && !defined(STLPORT)
#   if DEMI_COMPILER_VER >= 430
#       define DiHashMap ::std::tr1::unordered_map
#		define DiHashSet ::std::tr1::unordered_set
#    else
#       define DiHashMap ::__gnu_cxx::hash_map
#       define DiHashSet ::__gnu_cxx::hash_set
#    endif
#elif DEMI_COMPILER == DEMI_COMPILER_CLANG
#    if defined(_LIBCPP_VERSION)
#       define DiHashMap ::std::unordered_map
#       define DiHashSet ::std::unordered_set
#    else
#       define DiHashMap ::std::tr1::unordered_map
#       define DiHashSet ::std::tr1::unordered_set
#    endif
#else
#   if DEMI_COMPILER == DEMI_COMPILER_MSVC
#       if DEMI_COMPILER_VER >= 1600 // VC++ 10.0
#			define DiHashMap ::std::tr1::unordered_map
#           define DiHashSet ::std::tr1::unordered_set
#		elif DEMI_COMPILER_VER > 1300 && !defined(_STLP_MSVC)
#           define DiHashMap ::stdext::hash_map
#           define DiHashSet ::stdext::hash_set
#       else
#           define DiHashMap ::std::hash_map
#           define DiHashSet ::std::hash_set
#       endif
#   else
#       define DiHashMap ::std::hash_map
#       define DiHashSet ::std::hash_set
#   endif
#endif

#endif // HashMap_h__
