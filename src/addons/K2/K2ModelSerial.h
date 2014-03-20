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

#ifndef DiK2ModelSerial_h__
#define DiK2ModelSerial_h__

#include "K2Prerequisites.h"
#include "Serializer.h"

namespace Demi
{
    class DEMI_K2_API DiK2MdfSerial : public DiSerializer
    {
    public:

        DiK2MdfSerial();

        ~DiK2MdfSerial();

    public:

        bool    ParseMdf(const DiString& file, DiK2ModelAsset* target);

        bool    ParseMdf(DiXMLElement data, DiK2ModelAsset* target);

        bool    LoadModel(const DiString& file, DiK2ModelAsset* target);
        
        bool    LoadClip(const DiString& file, K2Anim* anim, DiK2ModelAsset* target);

        DiMaterialPtr ParseMaterial(const DiString& matFile, const DiString& basePath, 
            const DiString& name, bool needSkinning);

        static  DiString GetK2MediaPath(const DiString& relativePath);

    private:
        
        void    ParseAnim(DiXMLElement data, DiK2ModelAsset* target);
        
        bool    LoadBones(DiK2BonesDataPtr target, int numBones);

        DiSubMesh*  LoadMeshes(DiK2ModelAsset* target, DiMeshPtr mesh);

        bool    CheckFourcc(char* sig);
        
        bool    CheckFourcc(char* hed, char* sig);

        void    read_verts();

        void    read_face(DiSubMesh* sub);

        void    read_nrml();

        void    read_texc();

        void    read_colr();

        void    read_lnk();

        void    read_sign();

        void    read_tang();

        void    read_surf();
    };
}

#endif
