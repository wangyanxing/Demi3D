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

        return true;
    }
}