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
#include "DrvD3D9Pch.h"
#include "D3D9ErrorCheck.h"

namespace Demi 
{
    void DiD3D9ErrorCheck::CheckError(HRESULT hr)
    {
        if (hr != S_OK)
        {
            DiString desc = "Direct3D API Error:\n";
            desc += GetResultString(hr);
            DI_ERROR(desc.c_str());
        }
    }

    const char* DiD3D9ErrorCheck::GetResultString(HRESULT pr)
    {
        const int cnMaxLen = 512;
        static char szResult[cnMaxLen];

        szResult[ 0 ] = NULL;

        if ( IsD3DResult( pr ) )
        {
            const char* pszString  = DXGetErrorString( static_cast<HRESULT>(pr) );
            const char* pszDesc    = DXGetErrorDescription( static_cast<HRESULT>(pr) );

            _snprintf( szResult, cnMaxLen, "%s, description : %s",
                pszString,
                pszDesc );

            return szResult;
        }
        return NULL;
    }
}