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
#include "D3D9Shader.h"
#include "AssetManager.h"
#include "Texture.h"

namespace Demi
{
    STDMETHODIMP DiShaderIncluder::Open(THIS_ D3DXINCLUDE_TYPE includeType,
        LPCSTR fileName, LPCVOID,
        LPCVOID *data, uint32 *dataSize)
    {
        HRESULT result = D3DERR_NOTFOUND;

        char fullpath[1024];
        strcpy_s(fullpath, 1024, DiAssetManager::GetInstance().GetBasePath().c_str());
        if (includeType == D3DXINC_SYSTEM)
        {
            strcat_s(fullpath, 1024, "shaders/hlsl/include/");  // TODO
        }
        strcat_s(fullpath, 1024, fileName);

        FILE *file = 0;
        fopen_s(&file, fullpath, "r");
        if (file)
        {
            fseek(file, 0, SEEK_END);
            size_t fileLen = ftell(file);
            if (fileLen > 1)
            {
                fseek(file, 0, SEEK_SET);
                char *fileData = DI_NEW char[fileLen + 1];
                fileLen = fread(fileData, 1, fileLen, file);
                fileData[fileLen] = 0;
                *data = fileData;
                *dataSize = (uint32)fileLen;
            }
            fclose(file);
            result = D3D_OK;
        }
        DI_ASSERT(result == D3D_OK);
        return result;
    }

    STDMETHODIMP DiShaderIncluder::Close(THIS_ LPCVOID data)
    {
        delete[]((char*)data);
        return D3D_OK;
    }
}