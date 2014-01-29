
/********************************************************************
    File:       MaterialSerial.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GfxPrerequisites.h"
#include "XMLElement.h"
#include <sstream>

namespace Demi 
{
    class DI_GFX_API DiMaterialSerializer : public DiBase
    {
    public:

        DiMaterialSerializer();

        virtual ~DiMaterialSerializer();

    public:

        bool                    ParseMaterial(DiDataStreamPtr data, DiMaterial* target);

        bool                    ParseMaterial(DiXMLElement data, DiMaterial* target);

        bool                    SaveMaterial(const DiString& file, DiMaterial* source);

        bool                    SaveMaterial(DiXMLElement node, DiMaterial* source);

    protected:

        void                    ParseXml(DiXMLElement rootNode, DiMaterial* target);

        void                    ParseMatOpNode(DiXMLElement opNode, DiMaterial* target);

        void                    ParseVarNode(DiXMLElement varNode, DiMaterial* target);

        void                    ParseVarSampler(DiXMLElement samplerNode, DiMaterial* target, bool cubemap = false);

        void                    SaveXml(DiXMLElement rootNode, DiMaterial* source);

        void                    SaveTextureXml(DiXMLElement rootNode,DiTexture* tex,const DiString& varname );

        void                    SaveTexture(std::stringstream& ss,DiTexture* tex,const DiString& varname);

        void                    ResetDefaultValue(DiMaterial* target);

    public:

        static DiString         FilterToString(DiFilterType ft);

        static DiString         AddrModeToString(DiAddMode ad);

        static DiString         CullmodeToString(DiCullMode cm);

        static DiString         BlendModeToString(DiBlendMode cm);

    public:

        static DiFilterType     StringToFilter(const DiString& str);

        static DiAddMode        StringToAddrMode(const DiString& str);

        static DiCullMode       StringToCullmode(const DiString& str);

        static DiBlendMode      StringToBlendMode(const DiString& str);

    public:

        void                    InitTokensMaps();

        static DiMap<DiString,DiFilterType> sFilterTypeMap;

        static DiMap<DiString,DiAddMode>    sAddrModeMap;

        static DiMap<DiString,DiCullMode>   sCullModeMap;

        static DiMap<DiString,DiBlendMode>  sBlendModeMap;

        static DiFilterType     DEFAULT_FILTER;
                                
        static DiAddMode        DEFAULT_ADDRESSING_MODE;
                                
        static DiCullMode       DEFAULT_CULL_MODE;
                                
        static DiBlendMode      DEFAULT_BLEND_MODE;
                                
        static bool             DEFAULT_USER_MATERIAL;

        static bool             DEFAULT_DEPTH_CHECK;

        static bool             DEFAULT_DEPTH_WRITE;
    };
}