
#include "MiscPch.h"
#include "polygen.h"

namespace Demi
{
    DiPolygon::DiPolygon()
        : mNormal( DiVec3::ZERO )
        , mIsNormalSet(false)
    {
        // reserve space for 6 vertices to reduce allocation cost
        mVertexList.reserve(6);
    }
    
    DiPolygon::~DiPolygon()
    {
    }
    
    DiPolygon::DiPolygon( const DiPolygon& cpy )
    {
        mVertexList = cpy.mVertexList;
        mNormal = cpy.mNormal;
        mIsNormalSet = cpy.mIsNormalSet;
    }
    
    void DiPolygon::InsertVertex(const DiVec3& vdata, size_t vertex )
    {
        // TODO: optional: check planarity
        DI_ASSERT_MESSAGE(vertex <= GetVertexCount(), "Insert position out of range" );

        VertexList::iterator it = mVertexList.begin();

        std::advance(it, vertex);
        mVertexList.insert(it, vdata);

    }
    
    void DiPolygon::InsertVertex(const DiVec3& vdata)
    {
        mVertexList.push_back(vdata);
    }
    
    const DiVec3& DiPolygon::GetVertex( size_t vertex ) const
    {
        DI_ASSERT_MESSAGE(vertex < GetVertexCount(), "Search position out of range");

        return mVertexList[vertex];
    }
    
    void DiPolygon::SetVertex(const DiVec3& vdata, size_t vertex )
    {
        // TODO: optional: check planarity
        DI_ASSERT_MESSAGE(vertex < GetVertexCount(), "Search position out of range" );

        // set new vertex
        mVertexList[ vertex ] = vdata;
    }
    
    void DiPolygon::RemoveDuplicates( void )
    {
        for ( size_t i = 0; i < GetVertexCount(); ++i )
        {
            const DiVec3& a = GetVertex( i );
            const DiVec3& b = GetVertex( (i + 1)%GetVertexCount() );

            if (a.positionEquals(b))
            {
                DeleteVertex(i);
                --i;
            }
        }
    }
    
    size_t DiPolygon::GetVertexCount( void ) const
    {
        return mVertexList.size();
    }
    
    const DiVec3& DiPolygon::GetNormal( void ) const
    {
        DI_ASSERT_MESSAGE( GetVertexCount() >= 3, "Insufficient vertex count!" );

        UpdateNormal();

        return mNormal;
    }
    
    void DiPolygon::UpdateNormal( void ) const
    {
        DI_ASSERT_MESSAGE( GetVertexCount() >= 3, "Insufficient vertex count!" );

        if (mIsNormalSet)
            return;

        // vertex order is ccw
        const DiVec3& a = GetVertex( 0 );
        const DiVec3& b = GetVertex( 1 );
        const DiVec3& c = GetVertex( 2 );

        // used method: Newell
        mNormal.x = 0.5f * ( (a.y - b.y) * (a.z + b.z) +
            (b.y - c.y) * (b.z + c.z) + 
            (c.y - a.y) * (c.z + a.z));

        mNormal.y = 0.5f * ( (a.z - b.z) * (a.x + b.x) +
            (b.z - c.z) * (b.x + c.x) + 
            (c.z - a.z) * (c.x + a.x));

        mNormal.z = 0.5f * ( (a.x - b.x) * (a.y + b.y) +
            (b.x - c.x) * (b.y + c.y) + 
            (c.x - a.x) * (c.y + a.y));

        mNormal.normalise();

        mIsNormalSet = true;

    }
    
    void DiPolygon::DeleteVertex( size_t vertex )
    {
        DI_ASSERT_MESSAGE( vertex < GetVertexCount(), "Search position out of range" );

        VertexList::iterator it = mVertexList.begin();
        std::advance(it, vertex);

        mVertexList.erase( it );
    }
    
    void DiPolygon::StoreEdges( DiPolygon::EdgeMap *edgeMap ) const
    {
        DI_ASSERT_MESSAGE( edgeMap != NULL, "EdgeMap ptr is NULL" );

        size_t vertexCount = GetVertexCount();

        for ( size_t i = 0; i < vertexCount; ++i )
        {
            edgeMap->insert( Edge( GetVertex( i ), GetVertex( ( i + 1 ) % vertexCount ) ) );
        }
    }
    
    void DiPolygon::Reset( void )
    {
        // could use swap() to free memory here, but assume most may be reused so avoid realloc
        mVertexList.clear();

        mIsNormalSet = false;
    }
    
    bool DiPolygon::operator == (const DiPolygon& rhs) const
    {
        if ( GetVertexCount() != rhs.GetVertexCount() )
            return false;

        // Compare vertices. They may differ in its starting position.
        // find start
        size_t start = 0;
        bool foundStart = false;
        for (size_t i = 0; i < GetVertexCount(); ++i )
        {    
            if (GetVertex(0).positionEquals(rhs.GetVertex(i)))
            {
                start = i;
                foundStart = true;
                break;
            }
        }

        if (!foundStart)
            return false;

        for (size_t i = 0; i < GetVertexCount(); ++i )
        {
            const DiVec3& vA = GetVertex( i );
            const DiVec3& vB = rhs.GetVertex( ( i + start) % GetVertexCount() );

            if (!vA.positionEquals(vB))
                return false;
        }

        return true;
    }

    bool DiPolygon::IsPointInside(const DiVec3& point) const
    {
        // sum the angles 
        float anglesum = 0;
        size_t n = GetVertexCount();
        for (size_t i = 0; i < n; i++) 
        {
            const DiVec3& p1 = GetVertex(i);
            const DiVec3& p2 = GetVertex((i + 1) % n);

            DiVec3 v1 = p1 - point;
            DiVec3 v2 = p2 - point;

            float len1 = v1.length();
            float len2 = v2.length();

            if (DiMath::RealEqual(len1 * len2, 0.0f, 1e-4f))
            {
                // We are on a vertex so consider this inside
                return true; 
            }
            else
            {
                float costheta = v1.dotProduct(v2) / (len1 * len2);
                anglesum += acos(costheta);
            }
        }

        return DiMath::RealEqual(anglesum, DiMath::TWO_PI, 1e-4f);

    }
}