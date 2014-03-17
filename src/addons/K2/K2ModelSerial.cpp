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
}