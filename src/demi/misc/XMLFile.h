
/********************************************************************
    File:        AffineParts.h
    Creator:     demiwangya
    Description: This file is based on XMLFile.h from Urho3D project
*********************************************************************/


#pragma once
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
