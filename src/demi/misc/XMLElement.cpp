
#include "XMLFile.h"

#include "pugixml.hpp"

namespace Demi
{
    const DiXMLElement DiXMLElement::EMPTY;

    DiXMLElement::DiXMLElement() :
        mNode(0)
    {
    }

    DiXMLElement::DiXMLElement(shared_ptr<DiXMLFile> file, pugi::xml_node_struct* node) :
        mFile(file),
        mNode(node)
    {
    }

    DiXMLElement::DiXMLElement(const DiXMLElement& rhs) :
        mFile(rhs.mFile),
        mNode(rhs.mNode)
    {
    }

    DiXMLElement::~DiXMLElement()
    {
    }

    DiXMLElement& DiXMLElement::operator = (const DiXMLElement& rhs)
    {
        mFile = rhs.mFile;
        mNode = rhs.mNode;
        return *this;
    }

    DiXMLElement DiXMLElement::CreateChild(const DiString& name)
    {
        return CreateChild(name.c_str());
    }

    DiXMLElement DiXMLElement::CreateChild(const char* name)
    {
        if (!mFile || !mNode)
            return DiXMLElement();

        const pugi::xml_node& node =  pugi::xml_node(mNode);
        pugi::xml_node child = const_cast<pugi::xml_node&>(node).append_child(name);
        return DiXMLElement(mFile, child.internal_object());
    }

    bool DiXMLElement::RemoveChild(const DiXMLElement& element)
    {
        if (!element.mFile || !element.mNode || !mFile || !mNode)
            return false;

        const pugi::xml_node& node = pugi::xml_node(mNode);
        const pugi::xml_node& child = pugi::xml_node(element.mNode);
        return const_cast<pugi::xml_node&>(node).remove_child(child);
    }

    bool DiXMLElement::RemoveChild(const DiString& name)
    {
        return RemoveChild(name.c_str());
    }

    bool DiXMLElement::RemoveChild(const char* name)
    {
        if (!mFile || !mNode)
            return false;

        const pugi::xml_node& node = pugi::xml_node(mNode);
        return const_cast<pugi::xml_node&>(node).remove_child(name);
    }

    bool DiXMLElement::RemoveChildren(const DiString& name)
    {
        return RemoveChildren(name.c_str());
    }

    bool DiXMLElement::RemoveChildren(const char* name)
    {
        if (!mFile || !mNode)
            return false;

        const pugi::xml_node& node = pugi::xml_node(mNode);
        if (!DiString::StrLen(name))
        {
            for (;;)
            {
                pugi::xml_node child = node.last_child();
                if (child.empty())
                    break;
                const_cast<pugi::xml_node&>(node).remove_child(child);
            }
        }
        else
        {
            for (;;)
            {
                pugi::xml_node child = node.child(name);
                if (child.empty())
                    break;
                const_cast<pugi::xml_node&>(node).remove_child(child);
            }
        }

        return true;
    }

    bool DiXMLElement::RemoveAttribute(const DiString& name)
    {
        return RemoveAttribute(name.c_str());
    }

    bool DiXMLElement::RemoveAttribute(const char* name)
    {
        if (!mFile || !mNode)
            return false;

        const pugi::xml_node& node = pugi::xml_node(mNode);
        return const_cast<pugi::xml_node&>(node).remove_attribute(node.attribute(name));
    }

    bool DiXMLElement::SetAttribute(const DiString& name, const DiString& value)
    {
        return SetAttribute(name.c_str(), value.c_str());
    }

    bool DiXMLElement::SetAttribute(const char* name, const char* value)
    {
        if (!mFile || !mNode)
            return false;

        const pugi::xml_node& node = pugi::xml_node(mNode);
        pugi::xml_attribute attr = node.attribute(name);
        if (attr.empty())
            attr = const_cast<pugi::xml_node&>(node).append_attribute(name);
        return attr.set_value(value);
    }

    bool DiXMLElement::SetBool(const DiString& name, bool value)
    {
        return SetAttribute(name, DiString::FromBool(value));
    }

    bool DiXMLElement::SetBoundingBox(const DiAABB& value)
    {
        if (!SetVector3("min", value.GetMinimum()))
            return false;
        return SetVector3("max", value.GetMaximum());
    }

    bool DiXMLElement::SetColor(const DiString& name, const DiColor& value)
    {
        return SetAttribute(name, DiString::FromColourValue(value));
    }

    bool DiXMLElement::SetFloat(const DiString& name, float value)
    {
        return SetAttribute(name, DiString::FromFloat(value));
    }

    bool DiXMLElement::SetInt(const DiString& name, int value)
    {
        return SetAttribute(name, DiString::FromInt(value));
    }

    bool DiXMLElement::SetQuaternion(const DiString& name, const DiQuat& value)
    {
        return SetAttribute(name, DiString::FromQuaternion(value));
    }

    bool DiXMLElement::SetString(const DiString& name, const DiString& value)
    {
        return SetAttribute(name, value);
    }

    bool DiXMLElement::SetVector2(const DiString& name, const DiVec2& value)
    {
        return SetAttribute(name, DiString::FromVector2(value));
    }

    bool DiXMLElement::SetVector3(const DiString& name, const DiVec3& value)
    {
        return SetAttribute(name, DiString::FromVector3(value));
    }

    bool DiXMLElement::SetVector4(const DiString& name, const DiVec4& value)
    {
        return SetAttribute(name, DiString::FromVector4(value));
    }

    bool DiXMLElement::IsNull() const
    {
        return !NotNull();
    }

    bool DiXMLElement::NotNull() const
    {
        return mNode!=NULL;
    }

    DiXMLElement::operator bool () const
    {
        return NotNull();
    }

    DiString DiXMLElement::GetName() const
    {
        if ((!mFile || !mNode))
            return DiString();

        const pugi::xml_node& node = pugi::xml_node(mNode);
        return DiString(node.name());
    }

    bool DiXMLElement::CheckName(const char* name) const
    {
        if ((!mFile || !mNode))
            return false;
        const pugi::xml_node& node = pugi::xml_node(mNode);
        return DiString::StrCmp(node.name(),name) == 0;
    }

    bool DiXMLElement::HasChild(const DiString& name) const
    {
        return HasChild(name.c_str());
    }

    bool DiXMLElement::HasChild(const char* name) const
    {
        if (!mFile || !mNode)
            return false;

        const pugi::xml_node& node = pugi::xml_node(mNode);
        return !node.child(name).empty();
    }

    DiXMLElement DiXMLElement::GetChild(const DiString& name) const
    {
        return GetChild(name.c_str());
    }

    DiXMLElement DiXMLElement::GetChild(const char* name) const
    {
        if (!mFile || !mNode)
            return DiXMLElement();

        const pugi::xml_node& node = pugi::xml_node(mNode);
        if (!DiString::StrLen(name))
            return DiXMLElement(mFile, node.first_child().internal_object());
        else
            return DiXMLElement(mFile, node.child(name).internal_object());
    }

    DiXMLElement DiXMLElement::GetNext(const DiString& name) const
    {
        return GetNext(name.c_str());
    }

    DiXMLElement DiXMLElement::GetNext(const char* name) const
    {
        if (!mFile || !mNode)
            return DiXMLElement();

        const pugi::xml_node& node = pugi::xml_node(mNode);
        if (!DiString::StrLen(name))
            return DiXMLElement(mFile, node.next_sibling().internal_object());
        else
            return DiXMLElement(mFile, node.next_sibling(name).internal_object());
    }

    DiXMLElement DiXMLElement::GetParent() const
    {
        if (!mFile || !mNode)
            return DiXMLElement();

        const pugi::xml_node& node = pugi::xml_node(mNode);
        return DiXMLElement(mFile, node.parent().internal_object());
    }

    unsigned DiXMLElement::GetNumAttributes() const
    {
        if (!mFile || !mNode )
            return 0;

        const pugi::xml_node& node = pugi::xml_node(mNode);
        unsigned ret = 0;

        pugi::xml_attribute attr = node.first_attribute();
        while (!attr.empty())
        {
            ++ret;
            attr = attr.next_attribute();
        }

        return ret;
    }

    DiString DiXMLElement::GetValue() const
    {
        const pugi::xml_node& node = pugi::xml_node(mNode);
        return node.child_value();
    }

    bool DiXMLElement::SetValue(const DiString& val)
    {
        pugi::xml_node& node = pugi::xml_node(mNode);
        return node.text().set(val.c_str());
        //return node.set_value(val.c_str());
    }

    DiString DiXMLElement::ChildValue(const DiString& name) const
    {
        const pugi::xml_node& node = pugi::xml_node(mNode);
        return node.child_value(name.c_str());
    }

    bool DiXMLElement::HasAttribute(const DiString& name) const
    {
        return HasAttribute(name.c_str());
    }

    bool DiXMLElement::HasAttribute(const char* name) const
    {
        if (!mFile || !mNode)
            return false;

        const pugi::xml_node& node = pugi::xml_node(mNode);
        return !node.attribute(name).empty();
    }

    DiString DiXMLElement::GetAttribute(const DiString& name) const
    {
        return DiString(GetAttribute(name.c_str()));
    }

    DiString DiXMLElement::GetAttribute(const char* name) const
    {
        if (!mFile || !mNode)
            return DiString::BLANK;

        const pugi::xml_node& node = pugi::xml_node(mNode);
        return node.attribute(name).value();
    }

    DiString DiXMLElement::GetAttributeLower(const DiString& name) const
    {
        return GetAttribute(name).AsLower();
    }

    DiString DiXMLElement::GetAttributeLower(const char* name) const
    {
        return DiString(GetAttribute(name)).AsLower();
    }

    DiString DiXMLElement::GetAttributeUpper(const DiString& name) const
    {
        return GetAttribute(name).AsUpper();
    }

    DiString DiXMLElement::GetAttributeUpper(const char* name) const
    {
        return DiString(GetAttribute(name)).AsUpper();
    }

    DiVector<DiString> DiXMLElement::GetAttributeNames() const
    {
        if (!mFile || !mNode)
            return DiVector<DiString>();

        const pugi::xml_node& node = pugi::xml_node(mNode);
        DiVector<DiString> ret;

        pugi::xml_attribute attr = node.first_attribute();
        while (!attr.empty())
        {
            ret.push_back(DiString(attr.name()));
            attr = attr.next_attribute();
        }

        return ret;
    }

    bool DiXMLElement::GetBool(const DiString& name) const
    {
        return GetAttribute(name).AsBool();
    }

    DiAABB DiXMLElement::GetBoundingBox() const
    {
        DiAABB ret;

        ret.SetMinimum(GetVector3("min"));
        ret.SetMaximum(GetVector3("max"));
        return ret;
    }

    DiColor DiXMLElement::GetColor(const DiString& name) const
    {
        return GetAttribute(name).AsColourValue();
    }

    float DiXMLElement::GetFloat(const DiString& name) const
    {
        return GetAttribute(name).AsFloat();
    }

    int DiXMLElement::GetInt(const DiString& name) const
    {
        return GetAttribute(name).AsInt();
    }

    DiQuat DiXMLElement::GetQuaternion(const DiString& name) const
    {
        return GetAttribute(name).AsQuaternion();
    }

    DiVec2 DiXMLElement::GetVector2(const DiString& name) const
    {
        return GetAttribute(name).AsVector2();
    }

    DiVec3 DiXMLElement::GetVector3(const DiString& name) const
    {
        return GetAttribute(name).AsVector3();
    }

    DiVec4 DiXMLElement::GetVector4(const DiString& name) const
    {
        return GetAttribute(name).AsVector4();
    }

    shared_ptr<DiXMLFile> DiXMLElement::GetFile() const
    {
        return mFile;
    }

    bool DiXMLElement::SetUInt( const DiString& name, uint32 value )
    {
        return SetAttribute(name, DiString::FromUint(value));
    }

    uint32 DiXMLElement::GetUint( const DiString& name ) const
    {
        return GetAttribute(name).AsUint32();
    }
}