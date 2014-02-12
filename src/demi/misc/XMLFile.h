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


#ifndef DiXMLFile_h__
#define DiXMLFile_h__

#include "DataStream.h"
#include "XMLElement.h"

namespace pugi
{
    class xml_document;
}

namespace Demi
{

    /// XML document resource.
    class DI_MISC_API DiXMLFile : public enable_shared_from_this<DiXMLFile>
    {
    public:

        DiXMLFile();

        virtual ~DiXMLFile();

        virtual bool Load(DiDataStreamPtr stream);

        virtual bool Load(const DiString& xml);

        virtual bool Save(DiDataStreamPtr stream) const;

        virtual bool Save(const DiString& filepath) const;
        
        DiXMLElement CreateRoot(const DiString& name);
        
        DiXMLElement GetRoot(const DiString& name = DiString::BLANK);

        pugi::xml_document* GetDocument() const { return mDocument; }
        
    private:

        /// Pugixml document.
        pugi::xml_document* mDocument;
    };

}


#endif
