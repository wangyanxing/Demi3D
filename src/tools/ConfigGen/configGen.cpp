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

#include "Demi.h"
#include "XMLElement.h"
#include "XMLFile.h"
#include "PathLib.h"

#include <sstream>

#define TEST

struct ConfigStruct
{
    DiString name;
    DiString type;
    DiString defaultVal;
    bool list;
};

DiVector<ConfigStruct> gProps;
DiString gPrefix;
DiString gBinPath;
DiString gName;

DiString gBaseClass = R"(
struct ArConfigLoaderBase
{
    void Load(const DiXMLElement& node)
    {
        DiXMLElement child = node.GetChild();
        while (child)
        {
            DiString name = child.GetName();
            auto i = mPropOps.find(name);
            if(i != mPropOps.end())
            {
                i->second(child);
            }
            child = child.GetNext();
        }
    }
    
    DiMap<DiString, std::function<void(const DiXMLElement&)>> mPropOps;
};

)";

DiString GetTypeFuncs(const DiString& type)
{
    static DiMap<DiString,DiString> sTypes;
    if(sTypes.empty())
    {
        sTypes["DiString"] = "c_str()";
        sTypes["float"] = "AsFloat()";
        sTypes["int"] = "AsInt()";
        sTypes["uint32"] = "AsUint32()";
        sTypes["bool"] = "AsBool()";
        sTypes["DiVec2"] = "AsVector2()";
        sTypes["DiVec3"] = "AsVector3()";
        sTypes["DiVec4"] = "AsVector4()";
        sTypes["DiQuat"] = "AsQuaternion()";
    }
    
    auto it = sTypes.find(type);
    if(it != sTypes.end())
    {
        return it->second;
    }
    return DiString::BLANK;
}

bool CheckBuiltinTypes(const DiString& type)
{
    static DiSet<DiString> sBuiltins;
    if(sBuiltins.empty())
    {
        sBuiltins.insert("DiString");
        sBuiltins.insert("int");
        sBuiltins.insert("uint32");
        sBuiltins.insert("float");
        sBuiltins.insert("DiVec2");
        sBuiltins.insert("DiVec3");
        sBuiltins.insert("DiVec4");
        sBuiltins.insert("bool");
        sBuiltins.insert("DiQuat");
    }
    
    return sBuiltins.find(type) != sBuiltins.end();
}

void WriteFileHead(DiDataStreamPtr stream, bool headFile, bool loader)
{
    DiString head = R"(
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
    
/*** !!!! This file was generated automatically by ConfigGen !!!! ***/
)";
    
    stream->Write(head.c_str(), head.size());
    
    std::stringstream s;
    
    if(headFile)
    {
        DiString postfix = loader ? "Loader__h__" : "__h__";
        s << std::endl << "#ifndef " << gName.c_str() << postfix.c_str() << std::endl;
        s << "#define " << gName.c_str() << postfix.c_str() << std::endl << std::endl;
    }
    
    s.seekg(0, std::ios::end);
    stream->Write(s.str().c_str(), s.tellg());
}

void WriteFileEnd(DiDataStreamPtr stream, bool headFile)
{
    std::stringstream s;
    
    // namespace
    s << "} // namespace Demi" << std::endl;
    
    if(headFile)
    {
        s << "#endif" << std::endl;
    }
    
    s.seekg(0, std::ios::end);
    stream->Write(s.str().c_str(), s.tellg());
}

void GetProps(const DiXMLElement& node)
{
    gProps.clear();
    
    DiXMLElement child = node.GetChild();
    while (child)
    {
        gProps.push_back(ConfigStruct());
        gProps.back().name = child.GetName();
        gProps.back().type = child.GetAttribute("type");
        gProps.back().defaultVal = child.GetAttribute("default");
        gProps.back().list = false;
        if(child.HasAttribute("list"))
        {
            gProps.back().list = child.GetBool("list");
        }
        
        child = child.GetNext();
    }
}

void WriteStruct(DiDataStreamPtr stream, const DiXMLElement& node)
{
    DiString structName = gPrefix + node.GetName();
    std::stringstream s;
    
    // structure
    DiString desc = node.GetAttribute("desc");
    
    s << "/**" << std::endl;
    s << "  " << desc.c_str() << std::endl;
    s << " **/" << std::endl;
    s << "struct "<< structName.c_str() << std::endl << "{" << std::endl;

    GetProps(node);
    
    // constructor
    s << "    " << structName.c_str() << "()" << std::endl;
    s << "    {" << std::endl;
    for(auto i = gProps.begin(); i != gProps.end(); ++i)
    {
        // default values
        if(!i->defaultVal.empty())
        {
            s << "        " << i->name.c_str() << " = " << i->defaultVal.c_str() << ";"<<std::endl;
        }
    }
    
    s << "    }" << std::endl << std::endl;
   
    // properties
    for(auto i = gProps.begin(); i != gProps.end(); ++i)
    {
        DiString type = i->type;
        if(!CheckBuiltinTypes(type))
            type = gPrefix + type;
        
        if(i->list)
            s << "    DiVector<" << type.c_str() << "> " << i->name.c_str() << ";"<<std::endl;
        else
            s << "    " << type.c_str() << " " << i->name.c_str() << ";"<<std::endl;
    }
    
    s << "};" << std::endl << std::endl;
    
    s.seekg(0, std::ios::end);
    stream->Write(s.str().c_str(), s.tellg());
}

void WriteConfigFile(DiDataStreamPtr out
                     , const DiVector<DiString>& includeList
                     , const DiXMLElement& root)
{
    /// file head
    WriteFileHead(out, true, false);
    
    /// include files
    for(auto i = includeList.begin(); i != includeList.end(); ++i)
    {
        DiString str = "#include \"" + *i;
        str += "\"\n";
        out->Write(str.c_str(), str.size());
    }
    out->Write("#include \"str.h\"\n", strlen("#include \"str.h\"\n"));
    out->Write("\n", 1);
    
    // namespace
    DiString namespacetag = "namespace Demi{\n\n";
    out->Write(namespacetag.c_str(), namespacetag.size());
    
    /// structures
    DiXMLElement child = root.GetChild();
    while (child)
    {
        WriteStruct(out, child);
        child = child.GetNext();
    }
    
    /// file end
    WriteFileEnd(out, true);
}

void WriteConfigLoaderFile(DiDataStreamPtr stream
    , const DiVector<DiString>& includeList
    , const DiXMLElement& root)
{
    std::stringstream s;
    
    /// file head
    WriteFileHead(stream, true, true);
    
    /// include files
    for (auto i = includeList.begin(); i != includeList.end(); ++i)
    {
        DiString str = "#include \"" + *i;
        str += "\"\n";
        stream->Write(str.c_str(), str.size());
    }
    stream->Write("#include \"XMLElement.h\"\n", strlen("#include \"XMLElement.h\"\n"));

    stream->Write("\n", 1);
    // namespace
    DiString namespacetag = "namespace Demi{\n";
    stream->Write(namespacetag.c_str(), namespacetag.size());
    
    // base class
    stream->Write(gBaseClass.c_str(), gBaseClass.size());
    
    /// structures
    DiXMLElement child = root.GetChild();
    while (child)
    {
        GetProps(child);
        
        DiString structName = gPrefix + child.GetName();
        s << "struct " << structName.c_str() << "Loader : public ArConfigLoaderBase" << std::endl;
        s << "{" << std::endl;
        // constructor
        s << "    " << structName.c_str() << "Loader("<< structName.c_str() << "* obj)"<< std::endl;
        s << "    {"<< std::endl;
        s << "        m" << child.GetName().c_str() << " = obj;" << std::endl << std::endl;
        
        for(auto i = gProps.begin(); i != gProps.end(); ++i)
        {
            s << "        " << "mPropOps[\"" << i->name.c_str() << "\"] = [this](const DiXMLElement& node){" << std::endl;

            DiString func = GetTypeFuncs(i->type);

            if(i->list)
            {
                if (func.empty())
                {
                    s << "            " << std::endl;
                }
                else
                {
                    s << "            " << "m" << child.GetName().c_str() << "->" <<
                        i->name.c_str() << ".push_back(" << "node.GetValue()." << func.c_str() << ");" << std::endl;
                }
            }
            else
            {
                if (func.empty())
                {
                    s << "            " << gPrefix.c_str()<< i->type.c_str() << "Loader ld(&m" << 
                        child.GetName().c_str() << "->" << i->name.c_str() << ");" << std::endl;
                    s << "            ld.Load(node);" << std::endl;
                }
                else
                {
                    s << "            " << "m" << child.GetName().c_str() << "->" <<
                        i->name.c_str() << " = " << "node.GetValue()." << func.c_str() << ";" << std::endl;
                }
            }
            s << "        };" << std::endl;
        }
        
        s << "    }"<< std::endl<< std::endl;
        
        s << "    " << structName.c_str() << "* " << "m" << child.GetName().c_str() << ";" << std::endl;
        s << "};" << std::endl << std::endl;
        
        child = child.GetNext();
    }
    
    s << std::endl;
    s.seekg(0, std::ios::end);
    stream->Write(s.str().c_str(), s.tellg());
    
    /// file end
    WriteFileEnd(stream, true);
}

void LoadXML(DiDataStreamPtr stream)
{
    shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
    xmlfile->Load(stream->GetAsString());
    DiXMLElement root = xmlfile->GetRoot();
    
    if (!root.CheckName("template"))
    {
        printf("Invalid xml file!\n");
        return;
    }
    
    gName = root.GetAttribute("name");
    DiString outpath = gBinPath + root.GetAttribute("outpath") + gName + ".h";
    DiString outLoaderpath = gBinPath + root.GetAttribute("outpath") + gName + "Loader.h";
    
    DiString includes = root.GetAttribute("includes");
    DiVector<DiString> includeList = includes.Tokenize(";");

    gPrefix = root.GetAttribute("prefix");
    
    FILE* fpOut = fopen(outpath.c_str(),"w");
    if (!fpOut)
    {
        printf("Cannot write the output file: %s\n", outpath.c_str());
        return;
    }
    DiDataStreamPtr out(DI_NEW DiFileHandleDataStream(fpOut, DiDataStream::WRITE));
    WriteConfigFile(out, includeList, root);
    
    FILE* fpOutLoader = fopen(outLoaderpath.c_str(),"w");
    if (!fpOutLoader)
    {
        printf("Cannot write the output file: %s\n", outLoaderpath.c_str());
        return;
    }
    DiDataStreamPtr outLoader(DI_NEW DiFileHandleDataStream(fpOutLoader, DiDataStream::WRITE));
    WriteConfigLoaderFile(outLoader, includeList, root);
}

int main(int numargs, char** args)
{
    gBinPath = DiPathLib::GetApplicationPath();
    
#ifndef TEST
    if (numargs < 2)
    {
        printf("Usage: configGen file1.xml file2.xml ...\n");
        return 0;
    }
    
    for(int i = 1; i < numargs; ++i)
    {
        FILE* fp = fopen(args[i],"r");
        DiDataStreamPtr stream(DI_NEW DiFileHandleDataStream(fp, DiDataStream::READ));
        
        LoadXML(stream);
    }
#else
    DiString in = gBinPath + "../../src/tools/ConfigGen/configTemplate.xml";
    FILE* fp = fopen(in.c_str(),"r");
    DiDataStreamPtr stream(DI_NEW DiFileHandleDataStream(fp, DiDataStream::READ));
    LoadXML(stream);
#endif
    
    return 0;
}
