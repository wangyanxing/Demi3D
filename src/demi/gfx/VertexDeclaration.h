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

#ifndef DiVertexDeclaration_h__
#define DiVertexDeclaration_h__


#include "VertexFormat.h"

namespace Demi 
{
    class DI_GFX_API DiVertexElements : public DiBase
    {
    public:

        DiVertexElements();

        virtual        ~DiVertexElements();

        struct Element
        {
            uint16   Stream;     // Stream index
            uint16   Offset;     // Offset in the stream in bytes
            uint8    Type;       // Data type
            uint8    Usage;      // Semantics
            uint8    UsageIndex; // Semantic index
        };

        typedef DiVector<Element> ElementsList;

    public:

        Element               AddElement(uint16 stream, DiVertexType type, DiVertexUsage usage, uint8 usageid = 0);

        void                  AddElement(const Element& ele);

        void                  ClearElements();

        uint16                GetStreamElementsSize(uint16 stream);

        uint32                GetNumVertexElements()const { return mVertexElements.size(); }

        Element               GetElement(size_t index);

        Element               FindElement(DiVertexType type, DiVertexUsage usage, uint8 usageid = 0);

        bool                  Contains(DiVertexUsage usage);

        bool                  Contains(DiVertexType type);

        uint16                GetStreams() const { return (uint16)mStreamElementSize.size(); }

        uint16                GetTypeUsageAtStream(DiVertexType type, DiVertexUsage usage, uint8 usageid = 0);

        uint16                GetUsageAtStream(DiVertexUsage type);

        uint16                GetNextFreeTexcoord();

        void                  DeleteSource(uint16 streamSorce);

        ElementsList          GetElementsAtStream(uint16 streamSorce);

        void                  GetElementsAtStream(uint16 streamSorce, ElementsList& elem);

        static uint16         GetElementTypeSize(DiVertexType type);

        static uint16         GetElementTypeCount(DiVertexType type);

        static bool           ElementValid(Element ele);

    public:

        ElementsList          mVertexElements;

        DiMap<uint16,uint16>  mStreamElementSize;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiVertexDeclaration : public DiBase
    {
    public:
        DiVertexDeclaration(void);

        virtual                     ~DiVertexDeclaration(void);

    public:

        void                        Clone(const DiVertexDeclaration* from);

        void                        AddElement(uint16 stream, DiVertexType type, DiVertexUsage usage, uint8 usageid = 0);

        void                        AddElement(const DiVertexElements::Element& ele);

        void                        AddElements(const DiVertexElements& eles);

        DiVertexElements&           GetElements() { return mVertexElements; }

        virtual void                Release() = 0;

        virtual void                Create() = 0;

        virtual void                Bind() = 0;

        static DiVertexDeclaration* GetDefault(DiDefaultVertexFormat vf);

    private:

        static void                 CreateDefaultDeclarations();

        static DiVector<DiVertexDeclaration*> sDefaultDeclarations;

    protected:

        DiVertexElements            mVertexElements;
    };
}



#endif
