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

        bool    ParseMdf(const DiString& file, DiK2Model* target);

        bool    ParseMdf(DiXMLElement data, DiK2Model* target);

        bool    LoadModel(const DiString& file, DiK2Model* target);

        DiMaterialPtr ParseMaterial(const DiString& basePath, const DiString& name);
        
    private:
        
        void    ParseAnim(DiXMLElement data, DiK2Model* target);
        
        bool    LoadBones(DiK2Animation* target, int numBones);

        DiSubMesh*  LoadMeshes(DiK2Model* target, DiMeshPtr mesh);

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
