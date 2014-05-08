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
#include "GfxPch.h"
#include "StaticBillboardSet.h"
#include "AssetManager.h"
#include "ShaderParam.h"
#include "CullNode.h"
#include "SceneManager.h"
#include "SubMesh.h"
#include <strstream>
#include <sstream>

DiStaticBillboardSet::FadedMaterialMap DiStaticBillboardSet::sFadedMaterial;
unsigned long DiStaticBillboardSet::GUID             = 0;

namespace Demi
{
    DiStaticBillboardSet::DiStaticBillboardSet(DiCullNode *rootNode) :
        mVisible (true),
        mFadeEnabled (false),
        mCullNode (NULL),
        mModel (NULL),
        mUFactor (1.f),
        mVFactor (1.f),
        mBBOrigin (BBO_CENTER),
        mFadeVisibleDist (0.0f),
        mFadeInvisibleDist (0.0f)
    {
        mSceneManager = rootNode->GetCreator();

        mCullNode = rootNode->CreateChild();
        mModelName = GetUniqueID("SBSentity");

        ++sSelfInstances;
    }

    DiStaticBillboardSet::~DiStaticBillboardSet()
    {
        Clear();

        if (mMaterial)
            SBMaterialRef::RemoveMaterialRef(mMaterial);

        if (mFadeMaterial)
            SBMaterialRef::RemoveMaterialRef(mFadeMaterial);

        if (--sSelfInstances == 0)
            sFadedMaterial.clear();

        mSceneManager->DestroyNode(mCullNode);
    }

    void DiStaticBillboardSet::Clear()
    {
        if (mModel)
        {
            mCullNode->DetachAllObjects();

            DiString meshName(mMesh->GetName());
            mMesh.reset();
        }

        if (!mBillboardBuffer.empty())
        {
            for (int i = mBillboardBuffer.size() - 1; i > 0; )
            {
                DI_DELETE mBillboardBuffer[--i];
            }
            mBillboardBuffer.clear();
        }
    }

    void DiStaticBillboardSet::Build()
    {
        if (mModel)
        {
            mCullNode->DetachAllObjects();

            DI_ASSERT(mMesh);
            DiString meshName(mMesh->GetName());
            mMesh.reset();
            DiAssetManager::GetInstance().DestroyAsset(meshName);
        }

        if (mBillboardBuffer.empty())
        {
            return;
        }

        mMesh = DiAssetManager::GetInstance().CreateManualAsset<DiMesh>(GetUniqueID("SBSmesh"));
        DiSubMesh *pSubMesh = mMesh->CreateSubMesh();

        pSubMesh->SetPrimitiveType(PT_TRIANGLELIST);
        pSubMesh->SetVerticeNum(4 * mBillboardBuffer.size());

        pSubMesh->GetVertexElements().ClearElements();
        pSubMesh->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        pSubMesh->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);
        pSubMesh->GetVertexElements().AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
        pSubMesh->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);

        float* pReal = static_cast<float*>(pSubMesh->CreateSourceData(0,pSubMesh->GetVerticeNum(),
            pSubMesh->GetVertexElements().GetStreamElementsSize(0)));

        float minX = (float)DiMath::POS_INFINITY, minY = (float)DiMath::POS_INFINITY, minZ = (float)DiMath::POS_INFINITY;
        float maxX = (float)DiMath::NEG_INFINITY, maxY = (float)DiMath::NEG_INFINITY, maxZ = (float)DiMath::NEG_INFINITY;

        size_t billboardCount = mBillboardBuffer.size();
        for (size_t ibb = 0; ibb < billboardCount; ++ibb)
        {
            const StaticBillboard *bb = mBillboardBuffer[ibb];

            // position
            ////*pReal++ = bb->xPos;
            ////*pReal++ = bb->yPos;
            ////*pReal++ = bb->zPos;
            ////*pReal++ = bb->xScaleHalf;
            ////*pReal++ = bb->yScaleHalf;
            memcpy(pReal, &(bb->xPos), 5 * sizeof(float));
            pReal += 5;
            *pReal++ = 0.0f;

            // color
            *(reinterpret_cast< uint32* >(pReal++)) = bb->color;
            // uv
            *pReal++ = (bb->texcoordIndexU * mUFactor);
            *pReal++ = (bb->texcoordIndexV * mVFactor);


            // position
            //////*pReal++ = bb->xPos;
            //////*pReal++ = bb->yPos;
            //////*pReal++ = bb->zPos;
            //////*pReal++ = bb->xScaleHalf;
            //////*pReal++ = bb->yScaleHalf;
            memcpy(pReal, &(bb->xPos), 5 * sizeof(float));
            pReal += 5; // move to next float value
            *pReal++ = 1.0f;
            // color
            *(reinterpret_cast< uint32* >(pReal++)) = bb->color;
            // uv
            *pReal++ = ((bb->texcoordIndexU + 1) * mUFactor);
            *pReal++ = (bb->texcoordIndexV * mVFactor);


            // position
            //////*pReal++ = bb->xPos;
            //////*pReal++ = bb->yPos;
            //////*pReal++ = bb->zPos;
            //////*pReal++ = bb->xScaleHalf;
            //////*pReal++ = bb->yScaleHalf;
            memcpy(pReal, &(bb->xPos), 5 * sizeof(float));
            pReal += 5;
            *pReal++ = 2.0f;
            // color
            *(reinterpret_cast< uint32* > (pReal++)) = bb->color;
            // uv
            *pReal++ = (bb->texcoordIndexU * mUFactor);
            *pReal++ = ((bb->texcoordIndexV + 1) * mVFactor);


            // position
            //////*pReal++ = bb->xPos;
            //////*pReal++ = bb->yPos;
            //////*pReal++ = bb->zPos;
            //////*pReal++ = bb->xScaleHalf;
            //////*pReal++ = bb->yScaleHalf;
            memcpy(pReal, &(bb->xPos), 5 * sizeof(float));
            pReal += 5;
            *pReal++ = 3.0f;
            // color
            *(reinterpret_cast< uint32* >(pReal++)) = bb->color;
            // uv
            *pReal++ = ((bb->texcoordIndexU + 1) * mUFactor);
            *pReal++ = ((bb->texcoordIndexV + 1) * mVFactor);

            // ����AABB
            if (bb->xPos - bb->xScaleHalf < minX) minX = bb->xPos - bb->xScaleHalf;
            if (bb->xPos + bb->xScaleHalf > maxX) maxX = bb->xPos + bb->xScaleHalf;
            if (bb->yPos - bb->yScaleHalf < minY) minY = bb->yPos - bb->yScaleHalf;
            if (bb->yPos + bb->yScaleHalf > maxY) maxY = bb->yPos + bb->yScaleHalf;
            //if (bb->zPos - halfXScale < minZ) minZ = bb->zPos - halfXScale;
            //if (bb->zPos + halfXScale > maxZ) maxZ = bb->zPos + halfXScale;
            if (bb->zPos < minZ) minZ = bb->zPos - 0.5f;
            if (bb->zPos > maxZ) maxZ = bb->zPos + 0.5f;

            DI_DELETE bb;
        }
        mBillboardBuffer.clear();

        {
            uint16 *pIBuf = static_cast < uint16* > (pSubMesh->CreateIndexData(6 * billboardCount,false));
            for (uint16 i = 0; i < billboardCount; ++i)
            {
                uint16 offset = i * 4;

                *pIBuf++ = 0 + offset;
                *pIBuf++ = 2 + offset;
                *pIBuf++ = 1 + offset;

                *pIBuf++ = 1 + offset;
                *pIBuf++ = 2 + offset;
                *pIBuf++ = 3 + offset;
            }
            pSubMesh->SetPrimitiveCount(2 * billboardCount);
        }

        {
            DiAABB bounds(minX, minY, minZ, maxX, maxY, maxZ);
            mMesh->SetBounds(bounds);
        }

        mModel = make_shared<DiModel>(mModelName,mMesh);
        mModel->SetMaterial(mFadeEnabled ? mFadeMaterial->GetName(): mMaterial->GetName());

        mCullNode->AttachObject(mModel);
        mModel->SetVisible(mVisible);
    }

    void DiStaticBillboardSet::SetMaterial(const DiString &materialName)
    {
        bool needUpdateMat = !mMaterial || mMaterial->GetName() != materialName;
        if (!needUpdateMat)
        {
            return;
        }

        if (mFadeEnabled)
        {
            DI_ASSERT(mFadeMaterial);
            SBMaterialRef::RemoveMaterialRef(mFadeMaterial);
        }
        else if (mMaterial)
        {
            SBMaterialRef::RemoveMaterialRef(mMaterial);
        }

        mMaterial = DiAssetManager::GetInstance().GetAsset<DiMaterial>(materialName);

        if (mFadeEnabled)
        {
            mFadeMaterial = GetFadeMaterial(mMaterial, mFadeVisibleDist, mFadeInvisibleDist);
            SBMaterialRef::AddMaterialRef(mFadeMaterial, mBBOrigin);
        }
        else 
        {
            SBMaterialRef::AddMaterialRef(mMaterial, mBBOrigin);
        }

        if (mModel)
        {
            mModel->SetMaterial(mFadeEnabled ? mFadeMaterial->GetName(): mMaterial->GetName());
        }
    }

    void DiStaticBillboardSet::SetFade(bool enabled, float visibleDist, float invisibleDist)
    {
        if (enabled)
        {
            if (!mMaterial)
            {
                DI_ERROR("SetFade faild - null material");
            }

            if (mFadeEnabled)
            {
                DI_ASSERT(mFadeMaterial);
                SBMaterialRef::RemoveMaterialRef(mFadeMaterial);
            }
            else
            {
                DI_ASSERT(mMaterial);
                SBMaterialRef::RemoveMaterialRef(mMaterial);
            }

            mFadeMaterial = GetFadeMaterial(mMaterial, visibleDist, invisibleDist);
            SBMaterialRef::AddMaterialRef(mFadeMaterial, mBBOrigin);

            if (mModel)
            {
                mModel->SetMaterial(mFadeMaterial->GetName());
            }

            mFadeEnabled = true;
            mFadeVisibleDist = visibleDist;
            mFadeInvisibleDist = invisibleDist;
        }
        else
        {
            if (mFadeEnabled)
            {
                DI_ASSERT(mFadeMaterial);
                DI_ASSERT(mMaterial);
                SBMaterialRef::RemoveMaterialRef(mFadeMaterial);
                SBMaterialRef::AddMaterialRef(mMaterial, mBBOrigin);

                if (mModel)
                {
                    mModel->SetMaterial(mMaterial->GetName());
                }

                mFadeEnabled = false;
                mFadeVisibleDist = visibleDist;
                mFadeInvisibleDist = invisibleDist;
            }
        }
    }

    void DiStaticBillboardSet::SetTextureStacksAndSlices(uint16 stacks, uint16 slices)
    {
        DI_ASSERT(stacks != 0 && slices != 0);
        mUFactor = 1.0f / slices;
        mVFactor = 1.0f / stacks;
    }

    DiMaterialPtr DiStaticBillboardSet::GetFadeMaterial(const DiMaterialPtr &protoMaterial,
                                                    float visibleDist_, float invisibleDist_)
    {
        DI_ASSERT(protoMaterial);

        std::stringstream materialSignature;
        materialSignature << mModelName << "|";
        materialSignature << visibleDist_ << "|";
        materialSignature << invisibleDist_ << "|";
        //materialSignature << protoMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureUScroll() << "|";
        //materialSignature << protoMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureVScroll() << "|";

        FadedMaterialMap::iterator it = sFadedMaterial.find(materialSignature.str());
        if (it != sFadedMaterial.end())
        {
            return it->second;
        }
        else
        {
            DiMaterialPtr fadeMaterial = protoMaterial->Clone(GetUniqueID("ImpostorFade"));
            sFadedMaterial.insert(std::pair<DiString, DiMaterialPtr>(materialSignature.str(), fadeMaterial));
            return fadeMaterial;
        }
    }

    void DiStaticBillboardSet::UpdateAll(const DiVec3 &cameraDirection)
    {
        DiVec3 forward = cameraDirection;
        DiVec3 vRight = forward.crossProduct(DiVec3::UNIT_Y);
        DiVec3 vUp = forward.crossProduct(vRight);
        vRight.normalise();
        vUp.normalise();

        if (vUp.y < 0)
            vUp *= -1;

        DiVec3 vPoint0 = (-vRight + vUp);
        DiVec3 vPoint1 = ( vRight + vUp);
        DiVec3 vPoint2 = (-vRight - vUp);
        DiVec3 vPoint3 = ( vRight - vUp);

        DiVec4 preRotatedQuad_BBO_CENTER[4] =
        {
            DiVec4((float)vPoint0.x, (float)vPoint0.y, (float)vPoint0.z, 0.0f),
            DiVec4((float)vPoint1.x, (float)vPoint1.y, (float)vPoint1.z, 0.0f),
            DiVec4((float)vPoint2.x, (float)vPoint2.y, (float)vPoint2.z, 0.0f),
            DiVec4((float)vPoint3.x, (float)vPoint3.y, (float)vPoint3.z, 0.0f)
        };

        vPoint0 = (-vRight + vUp + vUp);
        vPoint1 = ( vRight + vUp + vUp);
        vPoint2 = (-vRight);
        vPoint3 = ( vRight);

        DiVec4 preRotatedQuad_BBO_BOTTOM_CENTER[4] =
        {
            DiVec4((float)vPoint0.x, (float)vPoint0.y, (float)vPoint0.z, 0.0f),
            DiVec4((float)vPoint1.x, (float)vPoint1.y, (float)vPoint1.z, 0.0f),
            DiVec4((float)vPoint2.x, (float)vPoint2.y, (float)vPoint2.z, 0.0f),
            DiVec4((float)vPoint3.x, (float)vPoint3.y, (float)vPoint3.z, 0.0f)
        };

        static const DiString uvScroll = "uvScroll", preRotatedQuad = "preRotatedQuad";

        SBMaterialRefList::iterator i1 = SBMaterialRef::GetList().begin(), iend = SBMaterialRef::GetList().end();
        while (i1 != iend)
        {
            DiMaterial *mat = i1->second->GetMaterial();

            DiVec4 *pQuad = i1->second->GetOrigin() == BBO_CENTER ?
                preRotatedQuad_BBO_CENTER : preRotatedQuad_BBO_BOTTOM_CENTER;

            DiShaderParameter* shap = mat->GetShaderParameter();
            shap->WriteFloat2(uvScroll,DiVec2::ZERO);
            shap->WriteFloat4Array(preRotatedQuad,pQuad,4);
            ++i1;
        }
    }

    void DiStaticBillboardSet::SetBillboardOrigin(BillboardOrigin origin)
    {
        DI_ASSERT((origin == BBO_CENTER || origin == BBO_BOTTOM_CENTER));
        mBBOrigin = origin;
    }

    DiString DiStaticBillboardSet::GetUniqueID( const DiString &prefix )
    {
        DiString str;
        str.Format("%s%d",prefix.c_str(),++GUID);
        return str;
    }

    void DiStaticBillboardSet::SetVisible( bool visible )
    {
        if (mVisible != visible)
        {
            mVisible = visible;
            mCullNode->SetVisible(visible);
        }
    }

    void DiStaticBillboardSet::CreateBillboard( const DiVec3 &position, float xScale /*= 1.0f*/, float yScale /*= 1.0f*/,
        const DiColor &color /*= DiColor::White*/, uint16 texcoordIndexU /*= 0*/, uint16 texcoordIndexV /*= 0*/ )
    {
        mBillboardBuffer.push_back(new StaticBillboard(position, xScale, yScale, 
            color, texcoordIndexU, texcoordIndexV));
    }

    unsigned int DiStaticBillboardSet::sSelfInstances = 0;
    SBMaterialRefList SBMaterialRef::sSelfList;

    void SBMaterialRef::AddMaterialRef(const DiMaterialPtr &matP, BillboardOrigin o)
    {
        DiMaterial *mat = matP.get();

        SBMaterialRef *matRef;
        SBMaterialRefList::iterator it;
        it = sSelfList.find(mat);

        if (it != sSelfList.end())
        {
            matRef = it->second;
            ++matRef->mRefCount;
        }
        else 
        {
            matRef = new SBMaterialRef(mat, o);
            sSelfList[mat] = matRef;
        }
    }

    void SBMaterialRef::RemoveMaterialRef(const DiMaterialPtr &matP)
    {
        DiMaterial *mat = matP.get();

        SBMaterialRef *matRef;
        SBMaterialRefList::iterator it;

        it = sSelfList.find(mat);
        if (it != sSelfList.end())
        {
            matRef = it->second;
            if (--matRef->mRefCount == 0)
            {
                delete matRef;
                sSelfList.erase(it);
            }
        }
    }

    SBMaterialRef::SBMaterialRef(DiMaterial *mat, BillboardOrigin o)
    {
        mMaterial = mat;
        mOrigin = o;
        mRefCount = 1;
    }
}
