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

#ifndef Singleton_h__
#define Singleton_h__

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable:4661)
#endif

namespace Demi
{
    template <typename T>
    class DiSingleton
    {
    private:
        DiSingleton(const DiSingleton<T> &);

        DiSingleton& operator=(const DiSingleton<T> &);

    protected:

        static T* s_pSingleton;

    public:
        DiSingleton( void )
        {
            DI_ASSERT( !s_pSingleton );
            s_pSingleton = static_cast< T* >( this );
        }

        ~DiSingleton( void )
        {  
            DI_ASSERT( s_pSingleton ); 
            s_pSingleton = 0;  
        }

        static T& GetInstance()
        {
            return *s_pSingleton;
        }

        static T* GetInstancePtr()
        {
            return s_pSingleton;
        }    
    };

#define _DECLARE_SINGLETON(cls) static cls* GetInstancePtr(void);\
                                static cls& GetInstance(void);

#define _IMPLEMENT_SINGLETON(cls) template<> cls* Demi::DiSingleton<cls>::s_pSingleton = 0;\
    cls* cls::GetInstancePtr(void){return s_pSingleton;}\
    cls& cls::GetInstance(void){return *s_pSingleton;}

}
#endif // Singleton_h__