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
