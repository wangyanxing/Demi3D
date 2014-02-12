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
#include "MiscPch.h"
#include "XMLFile.h"
#include "pugixml.hpp"

namespace Demi
{
    /// XML writer for pugixml.
    class XMLWriter : public pugi::xml_writer
    {
    public:
        /// Construct.
        XMLWriter(DiDataStreamPtr dest) :
            mDest(dest),
            mSuccess(true)
        {
        }
        
        /// Write bytes to output.
        void write(const void* data, size_t size)
        {
            if (mDest->Write(data, size) != size)
                mSuccess = false;
        }
        
        DiDataStreamPtr mDest;

        bool mSuccess;
    };

    DiXMLFile::DiXMLFile() :
        mDocument(new pugi::xml_document())
    {
    }

    DiXMLFile::~DiXMLFile()
    {
        delete mDocument;
        mDocument = 0;
    }

    bool DiXMLFile::Load(const DiString& xml)
    {
        if (!mDocument->load_buffer(xml.c_str(), xml.size()))
        {
            DI_ERROR("Could not parse XML data from buffer.");
            return false;
        }
        
        return true;
    }

    bool DiXMLFile::Load(DiDataStreamPtr source)
    {
        size_t dataSize = source->Size();
        if (!dataSize)
            return false;
        
        char* buffer = new char[dataSize];
        if (source->Read(buffer, dataSize) != dataSize)
        {
            delete[] buffer;
            return false;
        }
        
        if (!mDocument->load_buffer(buffer, dataSize))
        {
            DI_ERROR("Could not parse XML data from %s.", source->GetName().c_str());
            delete[] buffer;
            return false;
        }
        
        delete[] buffer;
        return true;
    }

    bool DiXMLFile::Save(DiDataStreamPtr dest) const
    {
        XMLWriter writer(dest);
        mDocument->save(writer);
        return writer.mSuccess;
    }

    bool DiXMLFile::Save(const DiString& filepath) const
    {
        FILE* fp = fopen(filepath.c_str(), "w+");
        DiDataStreamPtr ds = std::make_shared<DiFileHandleDataStream>(fp,DiDataStream::WRITE);
        bool ret = Save(ds);
        ds->Close();
        return ret;
    }

    DiXMLElement DiXMLFile::CreateRoot(const DiString& name)
    {
        mDocument->reset();
        pugi::xml_node root = mDocument->append_child(name.c_str());
        return DiXMLElement(shared_from_this(), root.internal_object());
    }

    DiXMLElement DiXMLFile::GetRoot(const DiString& name)
    {
        pugi::xml_node root = mDocument->first_child();
        if (root.empty())
            return DiXMLElement();
        
        if (!name.empty() && name != root.name())
            return DiXMLElement();
        else
            return DiXMLElement(shared_from_this(), root.internal_object());
    }

}
