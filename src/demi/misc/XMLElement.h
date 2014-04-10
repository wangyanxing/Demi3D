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

#ifndef DiXMLElement_h__
#define DiXMLElement_h__


namespace pugi
{
    struct xml_node_struct;
    class xpath_node;
    class xpath_node_set;
    class xpath_query;
    class xpath_variable_set;
}

namespace Demi
{
    /// Element in an XML file.
    class DI_MISC_API DiXMLElement
    {
    public:
        /// Construct null element.
        DiXMLElement();
        
        /// Construct with document and node pointers.
        DiXMLElement(shared_ptr<DiXMLFile> file, pugi::xml_node_struct* node);
        
        /// Copy-construct from another element.
        DiXMLElement(const DiXMLElement& rhs);
       
        /// Destruct.
        ~DiXMLElement();
        
        /// Assignment operator.
        DiXMLElement& operator = (const DiXMLElement& rhs);

        /// Create a child element.
        DiXMLElement CreateChild(const DiString& name);
        
        /// Create a child element.
        DiXMLElement CreateChild(const char* name);
        
        /// Remove a child element. Return true if successful.
        bool RemoveChild(const DiXMLElement& element);
        
        /// Remove a child element by name. Return true if successful.
        bool RemoveChild(const DiString& name);
        
        /// Remove a child element by name. Return true if successful.
        bool RemoveChild(const char* name);
        
        /// Remove child elements of certain name, or all child elements if name is empty. Return true if successful.
        bool RemoveChildren(const DiString& name = DiString::BLANK);
       
        /// Remove child elements of certain name, or all child elements if name is empty. Return true if successful.
        bool RemoveChildren(const char* name);

        /// Remove an attribute by name. Return true if successful.
        bool RemoveAttribute(const DiString& name = DiString::BLANK);

        /// Remove an attribute by name. Return true if successful.
        bool RemoveAttribute(const char* name);

        bool SetAttribute(const DiString& name, const DiString& value);

        bool SetAttribute(const char* name, const char* value);

        bool SetBool(const DiString& name, bool value);

        bool SetBoundingBox(const DiAABB& value);

        /// Set a color attribute.
        bool SetColor(const DiString& name, const DiColor& value);

        /// Set a float attribute.
        bool SetFloat(const DiString& name, float value);

        /// Set an integer attribute.
        bool SetInt(const DiString& name, int value);

        bool SetUInt(const DiString& name, uint32 value);

        /// Set a quaternion attribute.
        bool SetQuaternion(const DiString& name, const DiQuat& value);

        /// Set a string attribute.
        bool SetString(const DiString& name, const DiString& value);
        
        bool SetVector2(const DiString& name, const DiVec2& value);
        
        bool SetVector3(const DiString& name, const DiVec3& value);

        bool SetVector4(const DiString& name, const DiVec4& value);

        /// Return whether does not refer to an element or an XPath node.
        bool IsNull() const;

        /// Return whether refers to an element or an XPath node.
        bool NotNull() const;

        /// Return true if refers to an element or an XPath node.
        operator bool () const;

        /// Return element name (or attribute name if it is an attribute only XPath query result).
        DiString GetName() const;

        bool CheckName(const char* name) const;

        /// Return whether has a child element.
        bool HasChild(const DiString& name) const;

        /// Return whether has a child element.
        bool HasChild(const char* name) const;

        /// Return child element, or null if missing.
        DiXMLElement GetChild(const DiString& name = DiString::BLANK) const;

        /// Return child element, or null if missing.
        DiXMLElement GetChild(const char* name) const;

        /// Return next sibling element.
        DiXMLElement GetNext(const DiString& name = DiString::BLANK) const;

        /// Return next sibling element.
        DiXMLElement GetNext(const char* name) const;

        /// Return parent element.
        DiXMLElement GetParent() const;

        /// Return number of attributes.
        unsigned GetNumAttributes() const;

        /// Return whether has an attribute.
        bool HasAttribute(const DiString& name) const;

        /// Return whether has an attribute.
        bool HasAttribute(const char* name) const;

        DiString GetValue()const;

        bool SetValue(const DiString& val);

        DiString ChildValue(const DiString& name) const;

        /// Return attribute, or empty if missing.
        DiString GetAttribute(const DiString& name = DiString::BLANK) const;

        /// Return attribute, or empty if missing.
        DiString GetAttribute(const char* name) const;

        /// Return attribute in lowercase, or empty if missing.
        DiString GetAttributeLower(const DiString& name) const;

        /// Return attribute in lowercase, or empty if missing.
        DiString GetAttributeLower(const char* name) const;

        /// Return attribute in lowercase, or empty if missing.
        DiString GetAttributeUpper(const DiString& name) const;

        /// Return attribute in lowercase, or empty if missing.
        DiString GetAttributeUpper(const char* name) const;

        /// Return names of all attributes.
        DiVector<DiString> GetAttributeNames() const;

        /// Return bool attribute, or false if missing.
        bool GetBool(const DiString& name) const;

        /// Return bounding box attribute, or empty if missing.
        DiAABB GetBoundingBox() const;

        /// Return a color attribute, or default if missing.
        DiColor GetColor(const DiString& name) const;

        /// Return a float attribute, or zero if missing.
        float GetFloat(const DiString& name) const;

        /// Return an integer attribute, or zero if missing.
        int GetInt(const DiString& name) const;

        uint32 GetUint(const DiString& name) const;

        /// Return a quaternion attribute, or default if missing.
        DiQuat GetQuaternion(const DiString& name) const;
        
        /// Return a Vector2 attribute, or default if missing.
        DiVec2 GetVector2(const DiString& name) const;

        /// Return a DiVec3 attribute, or default if missing.
        DiVec3 GetVector3(const DiString& name) const;

        /// Return a Vector4 attribute, or default if missing.
        DiVec4 GetVector4(const DiString& name) const;

        /// Return XML file.
        shared_ptr<DiXMLFile> GetFile() const;

        /// Return pugixml xml_node_struct.
        pugi::xml_node_struct* GetNode() const { return mNode; }

       
        /// Empty XMLElement.
        static const DiXMLElement EMPTY;

    private:
        shared_ptr<DiXMLFile> mFile;

        pugi::xml_node_struct* mNode;
    };
}


#endif
