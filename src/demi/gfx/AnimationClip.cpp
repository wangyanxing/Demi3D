
#include "GfxPch.h"
#include "AnimationClip.h"
#include "Animation.h"
#include "KeyFrame.h"
#include "Node.h"
#include "Algorithm.h"

namespace Demi
{
    namespace 
    {
        struct KeyFrameTimeLess
        {
            bool operator() (const DiKeyFrame* kf, const DiKeyFrame* kf2) const
            {
                return kf->GetTime() < kf2->GetTime();
            }
        };
    }
    
    //////////////////////////////////////////////////////////////////////////

    DiAnimationClip::DiAnimationClip( DiAnimation* parent, uint32 handle )
        :mParent(parent),mHandle(handle)
    {
    }

    DiAnimationClip::~DiAnimationClip()
    {
        RemoveAllKeyFrames();
    }

    DiKeyFrame* DiAnimationClip::GetKeyFrame( uint32 index ) const
    {
        DI_ASSERT( index < (uint32)mKeyFrames.size() );

        return mKeyFrames[index];
    }

    float DiAnimationClip::GetKeyFramesAtTime( const DiTimeIndex& timeIndex,
        DiKeyFrame** keyFrame1, DiKeyFrame** keyFrame2,
        unsigned short* firstKeyIndex /*= 0*/ ) const
    {
        float t1, t2;

        float timePos = timeIndex.GetTimePos();

        KeyFrameList::const_iterator i;
        if (timeIndex.HasKeyIndex())
        {
            DI_ASSERT(timeIndex.GetKeyIndex() < mKeyFrameIndexMap.size());
            i = mKeyFrames.begin() + mKeyFrameIndexMap[timeIndex.GetKeyIndex()];
#ifdef _DEBUG
            DiKeyFrame timeKey(0, timePos);
            if (i != Demi::LowerBound(mKeyFrames.begin(), mKeyFrames.end(), 
                &timeKey, KeyFrameTimeLess()))
            {
                DI_ERROR("查找关键帧失败");
            }
#endif
        }
        else
        {
            float totalAnimationLength = mParent->GetLength();
            DI_ASSERT(totalAnimationLength > 0.0f);

            if( timePos > totalAnimationLength && totalAnimationLength > 0.0f )
                timePos = fmod( timePos, totalAnimationLength );

            DiKeyFrame timeKey(0, timePos);
            i = Demi::LowerBound(mKeyFrames.begin(), mKeyFrames.end(),
                &timeKey, KeyFrameTimeLess());
        }

        if (i == mKeyFrames.end())
        {
            *keyFrame2 = mKeyFrames.front();
            t2 = mParent->GetLength() + (*keyFrame2)->GetTime();
            --i;
        }
        else
        {
            *keyFrame2 = *i;
            t2 = (*keyFrame2)->GetTime();

            if (i != mKeyFrames.begin() && timePos < (*i)->GetTime())
            {
                --i;
            }
        }

        if (firstKeyIndex)
        {
            *firstKeyIndex = static_cast<unsigned short>(
                Demi::Distance(mKeyFrames.begin(), i));
        }

        *keyFrame1 = *i;

        t1 = (*keyFrame1)->GetTime();

        if (t1 == t2)
        {
            return 0.0;
        }
        else
        {
            return (timePos - t1) / (t2 - t1);
        }
    }

    DiKeyFrame* DiAnimationClip::CreateKeyFrame( float timePos )
    {
        DiKeyFrame* kf = CreateKeyFrameImpl(timePos);

        KeyFrameList::iterator i =
            Demi::UpperBound(mKeyFrames.begin(), mKeyFrames.end(), 
            kf, KeyFrameTimeLess());
        mKeyFrames.insert(i, kf);

        KeyFrameDataChanged();
        mParent->KeyFrameListChanged();

        return kf;

    }

    void DiAnimationClip::RemoveKeyFrame( uint32 index )
    {
        assert( index < (uint32)mKeyFrames.size() );

        KeyFrameList::iterator i = mKeyFrames.begin();
        i += index;
        DI_DELETE *i;

        mKeyFrames.erase(i);

        KeyFrameDataChanged();
        mParent->KeyFrameListChanged();

    }

    void DiAnimationClip::RemoveAllKeyFrames( void )
    {
        KeyFrameList::iterator i = mKeyFrames.begin();

        for (; i != mKeyFrames.end(); ++i)
        {
            DI_DELETE *i;
        }

        KeyFrameDataChanged();
        mParent->KeyFrameListChanged();

        mKeyFrames.clear();
    }

    size_t DiAnimationClip::GetNumFrames() const
    {
        return mKeyFrames.size();
    }

    void DiAnimationClip::CollectKeyFrameTimes( DiVector<float>& keyFrameTimes )
    {
        for (KeyFrameList::const_iterator i = mKeyFrames.begin(); 
            i != mKeyFrames.end(); ++i)
        {
            /// 获取该帧的时间戳
            float timePos = (*i)->GetTime();

            DiVector<float>::iterator it =
                Demi::LowerBound(keyFrameTimes.begin(), keyFrameTimes.end(), timePos);
            if (it == keyFrameTimes.end() || *it != timePos)
            {
                keyFrameTimes.insert(it, timePos);
            }
        }
    }

    void DiAnimationClip::BuildKeyFrameIndexMap( const DiVector<float>& keyFrameTimes )
    {
        mKeyFrameIndexMap.resize(keyFrameTimes.size() + 1);

        size_t i = 0, j = 0;
        while (j <= keyFrameTimes.size())
        {
            mKeyFrameIndexMap[j] = static_cast<uint32>(i);
            while (i < mKeyFrames.size() && 
                mKeyFrames[i]->GetTime() <= keyFrameTimes[j])
            {
                ++i;
            }
            ++j;
        }
    }

    //////////////////////////////////////////////////////////////////////////

    DiNodeClip::DiNodeClip( DiAnimation* parent, unsigned int handle )
        :DiAnimationClip(parent,handle),
        mTargetNode(NULL),
        mSplines(NULL),mSplineBuildNeeded(false),
        mUseShortestRotationPath(true)
    {

    }

    DiNodeClip::DiNodeClip( DiAnimation* parent, unsigned int handle, DiNode* targetNode )
        :DiAnimationClip(parent,handle),
        mTargetNode(targetNode),
        mSplines(NULL),mSplineBuildNeeded(false),
        mUseShortestRotationPath(true)
    {

    }

    DiNodeClip::~DiNodeClip()
    {
        if (mSplines)
        {
            DI_DELETE mSplines;
        }
    }

    DiTransformKeyFrame* DiNodeClip::CreateNodeKeyFrame( float timePos )
    {
        return static_cast<DiTransformKeyFrame*>(CreateKeyFrame(timePos));
    }

    void DiNodeClip::SetTargetNode( DiNode* node )
    {
        mTargetNode = node;
    }

    void DiNodeClip::ApplyToNode( DiNode* node, const DiTimeIndex& timeIndex, float weight /*= 1.0*/, float scl /*= 1.0f*/ )
    {
        if (mKeyFrames.empty() || !weight || !node)
        {
            return;
        }

        DiTransformKeyFrame kf(0, timeIndex.GetTimePos());
        GetInterpolatedKeyFrame(timeIndex, &kf);

        DiVec3 translate = kf.GetTranslate() * weight * scl;
        node->Translate(translate);

        DiQuat rotate;
        DiAnimation::RotationInterMode rim =
            mParent->GetRotationInterMode();
        if (rim == DiAnimation::RIM_LINEAR)
        {
            rotate = DiQuat::Nlerp(weight,
                DiQuat::IDENTITY, kf.GetRotation(), mUseShortestRotationPath);
        }
        else
        {
            rotate = DiQuat::Slerp(weight, 
                DiQuat::IDENTITY, kf.GetRotation(), mUseShortestRotationPath);
        }
        node->Rotate(rotate);

        DiVec3 scale = kf.GetScale();

        if (scale != DiVec3::UNIT_SCALE)
        {
            if (scl != 1.0f)
            {
                scale = DiVec3::UNIT_SCALE + (scale - DiVec3::UNIT_SCALE) * scl;
            }
            else if (weight != 1.0f)
            {
                scale = DiVec3::UNIT_SCALE + (scale - DiVec3::UNIT_SCALE) * weight;
            }
        }
        node->Scale(scale);
    }

    DiTransformKeyFrame* DiNodeClip::GetNodeKeyFrame( unsigned short index ) const
    {
        return static_cast<DiTransformKeyFrame*>(GetKeyFrame(index));
    }

    void DiNodeClip::KeyFrameDataChanged( void ) const
    {
        mSplineBuildNeeded = true;
    }

    void DiNodeClip::Optimise( void )
    {
        DiVec3 lasttrans = DiVec3::ZERO;
        DiVec3 lastscale = DiVec3::ZERO;
        DiQuat lastorientation;
        KeyFrameList::iterator i = mKeyFrames.begin();
        DiRadian quatTolerance(1e-3f);
        DiList<unsigned short> removeList;
        unsigned short k = 0;
        uint32 dupKfCount = 0;
        for (; i != mKeyFrames.end(); ++i, ++k)
        {
            DiTransformKeyFrame* kf = static_cast<DiTransformKeyFrame*>(*i);
            DiVec3 newtrans = kf->GetTranslate();
            DiVec3 newscale = kf->GetScale();
            DiQuat neworientation = kf->GetRotation();

            if (i != mKeyFrames.begin() &&
                newtrans.positionEquals(lasttrans) &&
                newscale.positionEquals(lastscale) &&
                neworientation.equals(lastorientation, quatTolerance))
            {
                ++dupKfCount;

                if (dupKfCount == 4)
                {
                    removeList.push_back(k-2);
                    --dupKfCount;
                }
            }
            else
            {
                dupKfCount = 0;
                lasttrans = newtrans;
                lastscale = newscale;
                lastorientation = neworientation;
            }
        }

        if (!removeList.empty())
        {
            DiList<unsigned short>::iterator r = removeList.end();
            --r;
            for (; r!= removeList.begin(); --r)
            {
                RemoveKeyFrame(*r);
            }
            RemoveKeyFrame(*removeList.begin());
        }
    }

    void DiNodeClip::Apply( const DiTimeIndex& timeIndex, float weight /*= 1.0*/, float scale /*= 1.0f*/ )
    {
        ApplyToNode(mTargetNode, timeIndex, weight, scale);
    }

    DiKeyFrame* DiNodeClip::CreateKeyFrameImpl( float time )
    {
        return DI_NEW DiTransformKeyFrame(this, time);
    }

    void DiNodeClip::BuildInterpolationSplines( void ) const
    {
        if (!mSplines)
        {
            mSplines = DI_NEW Splines;
        }

        Splines* splines = mSplines;

        splines->positionSpline.SetAutoCalculate(false);
        splines->rotationSpline.SetAutoCalculate(false);
        splines->scaleSpline.SetAutoCalculate(false);

        splines->positionSpline.Clear();
        splines->rotationSpline.Clear();
        splines->scaleSpline.Clear();

        auto iend = mKeyFrames.end();
        for (auto i = mKeyFrames.begin(); i != iend; ++i)
        {
            DiTransformKeyFrame* kf = static_cast<DiTransformKeyFrame*>(*i);
            splines->positionSpline.AddPoint(kf->GetTranslate());
            splines->rotationSpline.AddPoint(kf->GetRotation());
            splines->scaleSpline.AddPoint(kf->GetScale());
        }

        splines->positionSpline.RecalcTangents();
        splines->rotationSpline.RecalcTangents();
        splines->scaleSpline.RecalcTangents();


        mSplineBuildNeeded = false;
    }

    void DiNodeClip::GetInterpolatedKeyFrame( const DiTimeIndex& timeIndex, DiKeyFrame* kf ) const
    {
        DiTransformKeyFrame* kret = static_cast<DiTransformKeyFrame*>(kf);

        DiKeyFrame *kBase1, *kBase2;
        DiTransformKeyFrame *k1, *k2;
        unsigned short firstKeyIndex;

        float t = GetKeyFramesAtTime(timeIndex, &kBase1, &kBase2, &firstKeyIndex);
        k1 = static_cast<DiTransformKeyFrame*>(kBase1);
        k2 = static_cast<DiTransformKeyFrame*>(kBase2);

        if (DiMath::RealEqual(t, 0.0f))
        {
            kret->SetRotation(k1->GetRotation());
            kret->SetTranslate(k1->GetTranslate());
            kret->SetScale(k1->GetScale());
        }
        else
        {
            DiAnimation::InterMode im = mParent->GetInterMode();
            DiAnimation::RotationInterMode rim =
                mParent->GetRotationInterMode();
            DiVec3 base;
            switch(im)
            {
            case DiAnimation::IM_LINEAR:
                
                if (rim == DiAnimation::RIM_LINEAR)
                {
                    kret->SetRotation( DiQuat::Nlerp(t, k1->GetRotation(),
                        k2->GetRotation(), mUseShortestRotationPath) );
                }
                else
                {
                    kret->SetRotation( DiQuat::Slerp(t, k1->GetRotation(),
                        k2->GetRotation(), mUseShortestRotationPath) );
                }

                base = k1->GetTranslate();
                kret->SetTranslate( base + ((k2->GetTranslate() - base) * t) );

                base = k1->GetScale();
                kret->SetScale( base + ((k2->GetScale() - base) * t) );
                break;

            case DiAnimation::IM_SPLINE:
                if (mSplineBuildNeeded)
                {
                    BuildInterpolationSplines();
                }

                kret->SetRotation( mSplines->rotationSpline.Interpolate(firstKeyIndex, t,
                    mUseShortestRotationPath) );
                kret->SetTranslate( mSplines->positionSpline.Interpolate(firstKeyIndex, t) );
                kret->SetScale( mSplines->scaleSpline.Interpolate(firstKeyIndex, t) );

                break;
            }

        }
    }
}