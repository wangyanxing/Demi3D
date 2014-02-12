


#ifndef DiSceneSerial_h__
#define DiSceneSerial_h__


#include "Scene.h"
#include "Serializer.h"
#include "XMLElement.h"

namespace Demi
{

    enum DiMeshSerialVersion
    {
        SCENE_SERIAL_VERSION_0 = 0,

        SCENE_SERIAL_VERSION_NUM
    };

    class DiSceneBinSerializerImpl : public DiSerializer
    {
    public:

        void                ExportSceneBin(const DiScenePtr pMesh);

        void                ImportSceneBin(DiDataStreamPtr& stream, DiScene* pDest);

    protected:

        virtual void        ExportHeightMap(DiTerrainDesc* desc);

        virtual void        ExportVertColor(DiTerrainDesc* desc);

        virtual void        ExportLayerData(DiTerrainDesc* desc);

        virtual void        ExportWaterData(DiTerrainDesc* desc);

        virtual void        ExportFoliageData(DiTerrainDesc* desc);

        virtual void        ExportCliffData(DiTerrainDesc* desc);

    protected:

        virtual void        ImportHeightMap(DiTerrainDesc* desc, DiDataStreamPtr stream);

        virtual void        ImportVertColor(DiTerrainDesc* desc, DiDataStreamPtr stream);

        virtual void        ImportLayerData(DiTerrainDesc* desc, DiDataStreamPtr stream);

        virtual void        ImportWaterData(DiTerrainDesc* desc, DiDataStreamPtr stream);

        virtual void        ImportFoliageData(DiTerrainDesc* desc, DiDataStreamPtr stream);

        virtual void        ImportCliffData(DiTerrainDesc* desc, DiDataStreamPtr stream);

    };

    //////////////////////////////////////////////////////////////////////////

    struct QuerySceneNodesInterface
    {
        virtual void        QueryNodes(DiVector<DiCullNode*>& nodes) = 0;
    };

    class DI_GFX_API DiSceneSerializer : public DiBase
    {
    public:

        DiSceneSerializer();

        virtual             ~DiSceneSerializer();

    public:

        void                ExportScene(const DiScenePtr pScene, const DiString& pathName, QuerySceneNodesInterface* sni);

        void                ExportScene(const DiString& sceneName, const DiString& pathName, QuerySceneNodesInterface* sni);

        void                ImportScene(DiDataStreamPtr& stream, DiScene* scene, DiCullNode* parentNode = NULL);

        void                SaveObject(DiXMLElement node, DiCullNode* cn);

        void                LoadObject(DiXMLElement node, DiCullNode* parentNode);

        DiTransformUnit*    CreateTransformUnit(const DiString& type, const DiString& name);

    protected:

        void                WriteScene(const DiScenePtr pScene, DiXMLElement sceneRootNode);

        void                ReadScene(DiScene* pScene, DiXMLElement sceneRootNode);

        void                WriteBinaryData(const DiScenePtr pScene, const DiString& fileName);

        void                ReadBinaryData(DiScene* pScene, DiDataStreamPtr ds);


    protected:

        DiSceneBinSerializerImpl*    GetImplemention(DiMeshSerialVersion ver);

    };
}

#endif
