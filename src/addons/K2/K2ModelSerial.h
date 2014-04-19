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

        DiMaterialPtr ParseMaterial(const DiString& matFile,
            const DiString& basePath, bool needSkinning);

        static  DiString GetK2MediaPath(const DiString& relativePath);

    private:
        
        void    ParseAnim(DiXMLElement data, DiK2ModelAsset* target);
        
        bool    LoadBones(DiK2BonesDataPtr target, int numBones);

        DiSubMesh*  LoadMeshes(DiK2ModelAsset* target, DiMeshPtr mesh);

        DiString    TryMaterialFile(const DiString& name, DiK2ModelAsset* target);

        bool    CheckFourcc(char* sig);
        
        bool    CheckFourcc(char* hed, char* sig);

        void    ReadVertices();

        void    ReadFaces(DiSubMesh* sub);

        void    ReadNormals();

        void    ReadUVs();

        void    ReadVertColors();

        void    ReadBoneLinks(DiSubMesh* sm);

        void    ReadSigns();

        void    ReadTangents();

        void    ReadSurfaces();

        bool    mIgnoreSubMesh;
    };
}

#endif
