#include "GfxPch.h"
#include "VertexDeclaration.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiVertexDeclaration::DiVertexDeclaration(void)
    {
    }

    DiVertexDeclaration::~DiVertexDeclaration(void)
    {
    }

    void DiVertexDeclaration::AddElement(uint16 stream, DiVertexType type, 
        DiVertexUsage usage, uint8 usageid)
    {
        mVertexElements.AddElement(stream,type,usage,usageid);
    }

    void DiVertexDeclaration::AddElement(const DiVertexElements::Element& ele)
    {
        AddElement(ele.Stream, (DiVertexType)ele.Type, (DiVertexUsage)ele.Usage, ele.UsageIndex);
    }

    void DiVertexDeclaration::AddElements( const DiVertexElements& eles )
    {
        for (auto it = eles.mVertexElements.begin(); it != eles.mVertexElements.end(); ++it)
            AddElement(*it);
    }

    void DiVertexDeclaration::Clone( const DiVertexDeclaration* from )
    {
        Release();
        mVertexElements = from->mVertexElements;
        Create();
    }

    uint16 DiVertexElements::GetElementTypeCount(DiVertexType type)
    {
        switch (type)
        {
        case VERT_TYPE_FLOAT1:
        case VERT_TYPE_COLOR:
            return 1;
            break;
        case VERT_TYPE_FLOAT2:
        case VERT_TYPE_SHORT2:
            return 2;
            break;
        case VERT_TYPE_FLOAT3:
            return 3;
            break;
        case VERT_TYPE_FLOAT4:
        case VERT_TYPE_UBYTE4:
        case VERT_TYPE_SHORT4:
            return 4;
            break;
        default:
            DI_WARNING("Unsupported vertex element type: %d", type);
            return 0;
        }
    }

    uint16 DiVertexElements::GetElementTypeSize(DiVertexType type)
    {
        switch(type)
        {
        case VERT_TYPE_FLOAT1:
            return sizeof(float);
            break;
        case VERT_TYPE_FLOAT2:
            return sizeof(float)*2;
            break;
        case VERT_TYPE_FLOAT3:
            return sizeof(float)*3;
            break;
        case VERT_TYPE_FLOAT4:
            return sizeof(float)*4;
            break;
        case VERT_TYPE_COLOR:
            return sizeof(uint8)*4;
            break;
        case VERT_TYPE_UBYTE4:
            return sizeof(uint8)*4;
            break;
        case VERT_TYPE_SHORT2:
            return sizeof(uint16)*2;
            break;
        case VERT_TYPE_SHORT4:
            return sizeof(uint16)*4;
            break;
        default:
            DI_WARNING("Unsupported vertex element type: %d", type);
            return 0;
        }
    }

    DiVertexElements::DiVertexElements()
    {
    }

    DiVertexElements::~DiVertexElements()
    {
    }

    DiVertexElements::Element DiVertexElements::AddElement(uint16 stream, DiVertexType type,
        DiVertexUsage usage, uint8 usageid/*=0*/)
    {
        if (mStreamElementSize.find(stream)==mStreamElementSize.end())
        {
            mStreamElementSize[stream] = 0;
        }

        Element element;
        element.Stream     = stream;
        element.Offset     = (uint16)mStreamElementSize[stream];
        element.Type       = (uint8)type;
        element.Usage      = (uint8)usage;
        element.UsageIndex = usageid;
        mVertexElements.push_back(element);

        uint16 typeSize = GetElementTypeSize(DiVertexType(element.Type));
        //m_kStreamElementSize[stream] += typeSize;
        mStreamElementSize[stream] = mStreamElementSize[stream] + typeSize;

        return element;
    }

    void DiVertexElements::AddElement(const DiVertexElements::Element& ele)
    {
        AddElement(ele.Stream, (DiVertexType)ele.Type, (DiVertexUsage)ele.Usage, ele.UsageIndex);
    }

    uint16 DiVertexElements::GetStreamElementsSize(uint16 stream)
    {
        if (mStreamElementSize.find(stream)==mStreamElementSize.end())
        {
            return 0;
        }
        return mStreamElementSize[stream];
    }

    void DiVertexElements::ClearElements()
    {
        mVertexElements.clear();
    }

    bool DiVertexElements::Contains(DiVertexUsage usage)
    {
        for (auto it = mVertexElements.begin(); it != mVertexElements.end(); ++it)
        {
            if (it->Usage == usage)
                return true;
        }
        return false;
    }

    bool DiVertexElements::Contains(DiVertexType type)
    {
        for (auto it = mVertexElements.begin(); it != mVertexElements.end(); ++it)
        {
            if (it->Type == type)
                return true;
        }
        return false;
    }

    DiVertexElements::Element DiVertexElements::GetElement(size_t index)
    {
        DI_ASSERT(index >= 0 && index <mVertexElements.size());
        return mVertexElements[index];
    }

    uint16 DiVertexElements::GetTypeUsageAtStream(DiVertexType type,
        DiVertexUsage usage, uint8 usageid/*=0*/)
    {
        for (auto it = mVertexElements.begin(); it != mVertexElements.end(); ++it)
        {
            if (it->Type == type && it->Usage == usage && 
                it->Usage && it->UsageIndex == usageid)
            {
                return it->Stream;
            }
        }
        return 0;
    }

    uint16 DiVertexElements::GetUsageAtStream(DiVertexUsage u)
    {
        for (auto it = mVertexElements.begin(); it != mVertexElements.end(); ++it)
        {
            if (it->Usage == u)
            {
                return it->Stream;
            }
        }
        return 0;
    }

    DiVertexElements::Element DiVertexElements::FindElement(DiVertexType type,
        DiVertexUsage usage, uint8 usageid/*=0*/)
    {
        auto itEnd = mVertexElements.end();
        for (auto it = mVertexElements.begin(); it != itEnd; ++it)
        {
            if (it->Type == type && it->Usage == usage && it->UsageIndex == usageid)
            {
                return *it;
            }
        }
        DiVertexElements::Element element;
        element.Stream     = 0xFF;
        element.Offset     = 0;
        element.Type       = VERT_TYPE_UNUSED;
        element.Usage      = 0;
        element.UsageIndex = 0;
        return element;
    }

    bool DiVertexElements::ElementValid(DiVertexElements::Element ele)
    {
        if (ele.Stream > 16 || ele.Type == VERT_TYPE_UNUSED)
            return false;
        return true;
    }

    uint16 DiVertexElements::GetNextFreeTexcoord()
    {
        uint16 result = 0;
        for (auto it = mVertexElements.begin(); it != mVertexElements.end(); ++it)
        {
            if (it->Usage == VERT_USAGE_TEXCOORD && it->UsageIndex > result)
                result = it->UsageIndex;
        }
        return result+1;
    }

    void DiVertexElements::DeleteSource( uint16 streamSorce )
    {
        for (auto it = mVertexElements.begin(); it != mVertexElements.end(); )
        {
            if (it->Stream == streamSorce)
                it = mVertexElements.erase(it);
            else
                ++it;
        }
        mStreamElementSize[streamSorce] = 0;
    }

    DiVertexElements::ElementsList DiVertexElements::GetElementsAtStream(uint16 streamSorce)
    {
        ElementsList list;
        for (auto it = mVertexElements.begin(); it != mVertexElements.end(); ++it)
        {
            if (it->Stream == streamSorce)
                list.push_back(*it);
        }
        return list;
    }

    void DiVertexElements::GetElementsAtStream(uint16 streamSorce, ElementsList& list)
    {
        for (auto it = mVertexElements.begin(); it != mVertexElements.end(); ++it)
        {
            if (it->Stream == streamSorce)
                list.push_back(*it);
        }
    }

    void DiVertexDeclaration::CreateDefaultDeclarations()
    {
        // TODO
    }

    DiVertexDeclaration* DiVertexDeclaration::GetDefault(DiDefaultVertexFormat vf)
    {
        //D3DDECL_END

        return nullptr;
    }

    DiVector<DiVertexDeclaration*> DiVertexDeclaration::sDefaultDeclarations;
}