
/********************************************************************
    File:       Singleton.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#pragma warning(disable:4661)

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