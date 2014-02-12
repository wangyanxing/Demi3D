


#ifndef DiD3D9ErrorCheck_h__
#define DiD3D9ErrorCheck_h__


namespace Demi 
{
    class DiD3D9ErrorCheck
    {
    public:

        static void            CheckError(HRESULT hr);

        static BOOL            IsD3DResult( HRESULT hr )
        {
            return ( HRESULT_FACILITY( hr ) == _FACD3D );
        }

        static const char*    GetResultString( HRESULT pr );
    };

#define DX9_CHKERR(hr) DiD3D9ErrorCheck::CheckError(hr)
}

#endif
