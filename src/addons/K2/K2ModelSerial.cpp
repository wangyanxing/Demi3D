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

#include "K2Pch.h"
#include "K2ModelSerial.h"
#include "XMLFile.h"
#include "K2Model.h"
#include "Mesh.h"
#include "SubMesh.h"

namespace Demi
{
    DiK2MdfSerial::DiK2MdfSerial()
    {
    }

    DiK2MdfSerial::~DiK2MdfSerial()
    {
    }

    bool DiK2MdfSerial::ParseMdf(DiDataStreamPtr data, DiK2Model* target)
    {
        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        xmlfile->Load(data->GetAsString());
        DiXMLElement root = xmlfile->GetRoot();
        return ParseMdf(root, target);
    }

    bool DiK2MdfSerial::ParseMdf(DiXMLElement rootNode, DiK2Model* target)
    {
        if (rootNode.GetName() != "model")
        {
            DI_WARNING("Invalid material script.");
            return false;
        }
        
        DiXMLElement child = rootNode.GetChild();
        while (child)
        {
            if (child.CheckName("anim"))
                ParseAnim(child, target);
        }

        return true;
    }

    void DiK2MdfSerial::ParseAnim(DiXMLElement data, DiK2Model* target)
    {
        DiString animName = data.GetAttribute("name");
        DiString clipName = data.GetAttribute("clip");

        K2Anim& anim = target->AddAnim();
        anim.name = animName;
        anim.clip = clipName;
        anim.loop = false;
        anim.fps = 25;
        anim.numframes = 0;
        anim.loopbackframe = 0;

        if (data.HasAttribute("loop"))
            anim.loop = data.GetBool("loop");
        if (data.HasAttribute("fps"))
            anim.fps = data.GetFloat("fps");
        if (data.HasAttribute("numframes"))
            anim.numframes = data.GetInt("numframes");
        if (data.HasAttribute("loopbackframe"))
            anim.loopbackframe = data.GetInt("loopbackframe");
    }
    
    bool DiK2MdfSerial::LoadModel(DiDataStreamPtr data, DiK2Model* target)
    {
        mStream = data;
        
        DI_SERIAL_LOG("Loading k2 model: %s", data->GetName());
        
        if (!CheckFourcc("SMDL"))
        {
            DI_WARNING("Invalid k2 model file: unknow file format");
            return false;
        }
        
        if (!CheckFourcc("head"))
        {
            DI_WARNING("invalid k2 model file: file does not start with head chunk");
            return false;
        }
        
        // head size
        ReadInt(mStream);
        
        int version = ReadInt(mStream);
        int num_meshes = ReadInt(mStream);
        int num_sprites = ReadInt(mStream);
        int num_surfs = ReadInt(mStream);
        int num_bones = ReadInt(mStream);
        
        DI_SERIAL_LOG("version : %d", version);
        DI_SERIAL_LOG("%d meshes", num_meshes);
        DI_SERIAL_LOG("%d sprites", num_sprites);
        DI_SERIAL_LOG("%d surfaces", num_surfs);
        DI_SERIAL_LOG("%d bones", num_bones);
        
        float minx, miny, minz;
        float maxx, maxy, maxz;
        
        minx = ReadFloat(mStream);
        miny = ReadFloat(mStream);
        minz = ReadFloat(mStream);
        
        maxx = ReadFloat(mStream);
        maxy = ReadFloat(mStream);
        maxz = ReadFloat(mStream);
        
        DiMeshPtr mesh = nullptr;

        DiAABB bounds(minx, miny, minz, maxx, maxy, maxz);
        mesh->SetBounds(bounds);
        
        return true;
    }
    
    bool DiK2MdfSerial::CheckFourcc(char* sig)
    {
        char hed[4];
        mStream->Read(hed, 4);
        if (sig[0] != hed[0] ||
            sig[1] != hed[1] ||
            sig[2] != hed[2] ||
            sig[3] != hed[3])
        {
            return false;
        }
        return true;
    }
    
    bool DiK2MdfSerial::CheckFourcc(char* hed, char* sig)
    {
        if (sig[0] != hed[0] ||
            sig[1] != hed[1] ||
            sig[2] != hed[2] ||
            sig[3] != hed[3])
        {
            return false;
        }
        return true;
    }
}