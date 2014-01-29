
#include "GfxPch.h"
#include "AttachNode.h"

namespace Demi
{
    DiAttachNode::DiAttachNode(DiString strName)
        : DiNode(strName)
    {
        mNodeType = NT_ATTACHNODE;
    }

    DiAttachNode::~DiAttachNode()
    {

    }

    DiAttachNode* DiAttachNode::CreateChild( DiString strName )
    {
        if(strName.empty())
        {
            DI_WARNING("Invalid name");
            return NULL;
        }

        DiAttachNode* ret = DI_NEW DiAttachNode(strName);
        this->AddChild(ret);
        return ret;
    }

    DiNode* DiAttachNode::CreateChild( const DiString& strName, const DiVec3& vecTranslate /*= DiVec3::ZERO*/, const DiQuat& vecRotate /*= DiQuat::IDENTITY*/ )
    {
        DiAttachNode * pkNewChild = CreateChild(strName);

        if(!pkNewChild)
        {
            return NULL;
        }

        pkNewChild->Translate(vecTranslate);
        pkNewChild->Rotate(vecRotate);

        return pkNewChild;
    }

    DiNode* DiAttachNode::CreateChild( const DiVec3& vecTranslate /*= DiVec3::ZERO*/, const DiQuat& vecRotate /*= DiQuat::IDENTITY */ )
    {
        return NULL;
    }

    void DiAttachNode::SetBindingPose()
    {
        SetInitialState();

        mBindDerivedInversePosition        = - GetDerivedPosition();
        mBindDerivedInverseScale            = DiVec3::UNIT_SCALE / GetDerivedScale();
        mBindDerivedInverseOrientation    = GetDerivedOrientation().Inverse();
    }

    void DiAttachNode::Reset()
    {
        ResetToInitialState();
    }

    void DiAttachNode::GetOffsetTransform( DiMat4& matRes )
    {
        DiVec3 locScale = GetDerivedScale() * mBindDerivedInverseScale;
        DiQuat locRotate = GetDerivedOrientation() * mBindDerivedInverseOrientation;

        DiVec3 locTranslate = GetDerivedPosition() +
            locRotate * (locScale * mBindDerivedInversePosition);

        matRes.makeTransform(locTranslate, locScale, locRotate);
    }

    void DiAttachNode::NeedUpdate( bool forceParentUpdate /*= false*/ )
    {
        DiNode::NeedUpdate(forceParentUpdate);

        // 如果有手动控制 这里处理逻辑
    }

}