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
#include "ConvexBody.h"
#include "Math/vector3.h"
#include "Math/ray.h"
#include "math/AxisAlignedBox.h"
#include "Frustum.h"

namespace Demi
{
	DiConvexBody::PolygonList DiConvexBody::msFreePolygons;

	void DiConvexBody::_initialisePool()
	{
		if (msFreePolygons.empty())
		{
			const size_t initialSize = 30;

			// Initialise polygon pool with 30 polys
			msFreePolygons.resize(initialSize);
			for (size_t i = 0; i < initialSize; ++i)
			{
				msFreePolygons[i] = DI_NEW DiPolygon();
			}
		}
	}
    
	void DiConvexBody::_destroyPool()
	{
		for (PolygonList::iterator i = msFreePolygons.begin(); 
			i != msFreePolygons.end(); ++i)
		{
            DI_DELETE *i;
		}
		msFreePolygons.clear();
	}

	DiPolygon* DiConvexBody::allocatePolygon()
	{
		if (msFreePolygons.empty())
		{
			// if we ran out of polys to use, create a new one
			// hopefully this one will return to the pool in due course
			return DI_NEW DiPolygon();
		}
		else
		{
			DiPolygon* ret = msFreePolygons.back();
			ret->Reset();

			msFreePolygons.pop_back();

			return ret;

		}
	}

	void DiConvexBody::freePolygon(DiPolygon* poly)
	{
		msFreePolygons.push_back(poly);
	}
    
	DiConvexBody::DiConvexBody()
	{
		// Reserve space for 8 polys, normally 6 faces plus a couple of clips
		mPolygons.reserve(8);

	}

	DiConvexBody::~DiConvexBody()
	{
		reset();
	}
    
	DiConvexBody::DiConvexBody( const DiConvexBody& cpy )
	{
		for ( size_t i = 0; i < cpy.getPolygonCount(); ++i )
		{
			DiPolygon *p = allocatePolygon();
			*p = cpy.getPolygon( i );
			mPolygons.push_back( p );
		}
	}

	void DiConvexBody::define(const DiFrustum& frustum)
	{
		// ordering of the points:
		// near (0-3), far (4-7); each (top-right, top-left, bottom-left, bottom-right)
		//	   5-----4
		//	  /|    /|
		//	 / |   / |
		//	1-----0  |
		//	|  6--|--7
		//	| /   | /
		//	|/    |/
		//	2-----3
		
		const DiVec3 *pts = frustum.GetWorldSpaceCorners();

		/// reset ConvexBody
		reset();

		/// update vertices: near, far, left, right, bottom, top; fill in ccw
		DiPolygon *poly;

		// near
		poly = allocatePolygon();
		poly->InsertVertex( pts[0] );
		poly->InsertVertex( pts[1] );
		poly->InsertVertex( pts[2] );
		poly->InsertVertex( pts[3] );
		mPolygons.push_back( poly );

		// far
		poly = allocatePolygon();
		poly->InsertVertex( pts[5] );
		poly->InsertVertex( pts[4] );
		poly->InsertVertex( pts[7] );
		poly->InsertVertex( pts[6] );
		mPolygons.push_back( poly );

		// left
		poly = allocatePolygon();
		poly->InsertVertex( pts[5] );
		poly->InsertVertex( pts[6] );
		poly->InsertVertex( pts[2] );
		poly->InsertVertex( pts[1] );
		mPolygons.push_back( poly ); 

		// right
		poly = allocatePolygon();
		poly->InsertVertex( pts[4] );
		poly->InsertVertex( pts[0] );
		poly->InsertVertex( pts[3] );
		poly->InsertVertex( pts[7] );
		mPolygons.push_back( poly ); 

		// bottom
		poly = allocatePolygon();
		poly->InsertVertex( pts[6] );
		poly->InsertVertex( pts[7] );
		poly->InsertVertex( pts[3] );
		poly->InsertVertex( pts[2] );
		mPolygons.push_back( poly ); 

		// top
		poly = allocatePolygon();
		poly->InsertVertex( pts[4] );
		poly->InsertVertex( pts[5] );
		poly->InsertVertex( pts[1] );
		poly->InsertVertex( pts[0] );
		mPolygons.push_back( poly ); 
	}

	void DiConvexBody::define(const DiAABB& aab)
	{
		// ordering of the AAB points:
		//		1-----2
		//	   /|    /|
		//	  / |   / |
		//   5-----4  |
		//   |  0--|--3
		//   | /   | /
		//   |/    |/
		//   6-----7
		
		const DiVec3& min = aab.GetMinimum();
		const DiVec3& max = aab.GetMaximum();

		DiVec3 currentVertex = min;

		DiPolygon *poly;

		// reset body
		reset();

		// far
		poly = allocatePolygon();
		poly->InsertVertex( currentVertex ); // 0 
		currentVertex.y = max.y;
		poly->InsertVertex( currentVertex ); // 1
		currentVertex.x = max.x;
		poly->InsertVertex( currentVertex ); // 2
		currentVertex.y = min.y;
		poly->InsertVertex( currentVertex ); // 3
		insertPolygon( poly );

		// right
		poly = allocatePolygon();
		poly->InsertVertex( currentVertex ); // 3
		currentVertex.y = max.y;
		poly->InsertVertex( currentVertex ); // 2
		currentVertex.z = max.z;
		poly->InsertVertex( currentVertex ); // 4
		currentVertex.y = min.y;
		poly->InsertVertex( currentVertex ); // 7
		insertPolygon( poly ); 

		// near
		poly = allocatePolygon();
		poly->InsertVertex( currentVertex ); // 7
		currentVertex.y = max.y;
		poly->InsertVertex( currentVertex ); // 4
		currentVertex.x = min.x;
		poly->InsertVertex( currentVertex ); // 5
		currentVertex.y = min.y;
		poly->InsertVertex( currentVertex ); // 6
		insertPolygon( poly );

		// left
		poly = allocatePolygon();
		poly->InsertVertex( currentVertex ); // 6
		currentVertex.y = max.y;
		poly->InsertVertex( currentVertex ); // 5
		currentVertex.z = min.z;
		poly->InsertVertex( currentVertex ); // 1
		currentVertex.y = min.y;
		poly->InsertVertex( currentVertex ); // 0
		insertPolygon( poly ); 

		// bottom
		poly = allocatePolygon();
		poly->InsertVertex( currentVertex ); // 0 
		currentVertex.x = max.x;
		poly->InsertVertex( currentVertex ); // 3
		currentVertex.z = max.z;
		poly->InsertVertex( currentVertex ); // 7 
		currentVertex.x = min.x;
		poly->InsertVertex( currentVertex ); // 6
		insertPolygon( poly );

		// top
		poly = allocatePolygon();
		currentVertex = max;
		poly->InsertVertex( currentVertex ); // 4
		currentVertex.z = min.z;
		poly->InsertVertex( currentVertex ); // 2
		currentVertex.x = min.x;
		poly->InsertVertex( currentVertex ); // 1
		currentVertex.z = max.z;
		poly->InsertVertex( currentVertex ); // 5
		insertPolygon( poly );
		
	}

	void DiConvexBody::clip(const DiAABB& aab)
	{
		// only process finite boxes
		if (!aab.IsFinite())
			return;
		// ordering of the AAB points:
		//		1-----2
		//	   /|    /|
		//	  / |   / |
		//   5-----4  |
		//   |  0--|--3
		//   | /   | /
		//   |/    |/
		//   6-----7

		const DiVec3& min = aab.GetMinimum();
		const DiVec3& max = aab.GetMaximum();

		// clip object for each plane of the AAB
		DiPlane p;


		// front
		p.redefine(DiVec3::UNIT_Z, max);
		clip(p);

		// back
		p.redefine(DiVec3::NEGATIVE_UNIT_Z, min);
		clip(p);
		
		// left
		p.redefine(DiVec3::NEGATIVE_UNIT_X, min);
		clip(p);
		
		// right
		p.redefine(DiVec3::UNIT_X, max);
		clip(p);
		
		// bottom
		p.redefine(DiVec3::NEGATIVE_UNIT_Y, min);
		clip(p);
		
		// top
		p.redefine(DiVec3::UNIT_Y, max);
		clip(p);

	}
	//-----------------------------------------------------------------------
	void DiConvexBody::clip(const DiFrustum& fr)
	{
		// clip the body with each plane
		for ( unsigned short i = 0; i < 6; ++i )
		{
			// clip, but keep positive space this time since frustum planes are 
			// the opposite to other cases (facing inwards rather than outwards)
			clip(fr.GetFrustumPlane(i), false);
		}
	}
	//-----------------------------------------------------------------------
	void DiConvexBody::clip(const DiConvexBody& body)
	{
		if ( this == &body )
			return;

		// for each polygon; clip 'this' with each plane of 'body'
		// front vertex representation is ccw

		DiPlane pl;

		for ( size_t iPoly = 0; iPoly < body.getPolygonCount(); ++iPoly )
		{
			const DiPolygon& p = body.getPolygon( iPoly );

			DI_ASSERT( p.GetVertexCount() >= 3 );

			// set up plane with first three vertices of the polygon (a polygon is always planar)
			pl.redefine( p.GetVertex( 0 ), p.GetVertex( 1 ), p.GetVertex( 2 ) );

			clip(pl);
		}
	}

	void DiConvexBody::extend(const DiVec3& pt)
	{
		// Erase all polygons facing towards the point. For all edges that
		// are not removed twice (once in AB and once BA direction) build a
		// convex polygon (triangle) with the point.
		DiPolygon::EdgeMap edgeMap;

		for ( size_t i = 0; i < getPolygonCount(); ++i )
		{
			const DiVec3& normal = getNormal( i );
			// direction of the point in regard to the polygon
			// the polygon is planar so we can take an arbitrary vertex
			DiVec3 ptDir  = pt - getVertex( i, 0 );
			ptDir.normalise();

			// remove polygon if dot product is greater or equals null.
			if ( normal.dotProduct( ptDir ) >= 0 )
			{
				// store edges (copy them because if the polygon is deleted
				// its vertices are also deleted)
				storeEdgesOfPolygon( i, &edgeMap );

				// remove polygon
				deletePolygon( i );

				// decrement iterator because of deleted polygon
				--i; 
			}
		}

		// point is already a part of the hull (point lies inside)
		if ( edgeMap.empty() )
			return;

		// remove the edges that are twice in the list (once from each side: AB,BA)

		DiPolygon::EdgeMap::iterator it;
		// iterate from first to the element before the last one
		for (DiPolygon::EdgeMap::iterator itStart = edgeMap.begin(); 
			itStart != edgeMap.end(); )
		{
			// compare with iterator + 1 to end
			// don't need to skip last entry in itStart since omitted in inner loop
			it = itStart;
			++it;

			bool erased = false;
			// iterate from itStart+1 to the element before the last one
			for ( ; it != edgeMap.end(); ++it )
			{	
				if (itStart->first.positionEquals(it->second) &&
					 itStart->second.positionEquals(it->first))
				{
					edgeMap.erase(it);
					// increment itStart before deletion (iterator invalidation)
					DiPolygon::EdgeMap::iterator delistart = itStart++;
					edgeMap.erase(delistart);
					erased = true;

					break; // found and erased
				}
			}
			// increment itStart if we didn't do it when erasing
			if (!erased)
				++itStart;

		}

		// use the remaining edges to build triangles with the point
		// the vertices of the edges are in ccw order (edgePtA-edgePtB-point
		// to form a ccw polygon)
		while ( !edgeMap.empty() )
		{
			DiPolygon::EdgeMap::iterator mapIt = edgeMap.begin();

			// build polygon it.first, it.second, point
			DiPolygon *p = allocatePolygon();

			p->InsertVertex(mapIt->first);
			p->InsertVertex(mapIt->second);

			p->InsertVertex( pt );
			// attach polygon to body
			insertPolygon( p );

			// erase the vertices from the list
			// pointers are now held by the polygon
			edgeMap.erase( mapIt );
		}
	}
	
	void DiConvexBody::reset( void )
	{
		for (PolygonList::iterator it = mPolygons.begin(); 
			it != mPolygons.end(); ++it)
		{
			freePolygon(*it);
		}
		mPolygons.clear();
	}

	size_t DiConvexBody::getPolygonCount( void ) const
	{
		return mPolygons.size();
	}

	size_t DiConvexBody::getVertexCount( size_t poly ) const
	{
		DI_ASSERT(poly < getPolygonCount());
		
		return mPolygons[ poly ]->GetVertexCount();
	}

	bool DiConvexBody::hasClosedHull( void ) const
	{
		// if this map is returned empty, the body is closed
		DiPolygon::EdgeMap edgeMap = getSingleEdges();

		return edgeMap.empty();
	}

	void DiConvexBody::mergePolygons( void )
	{
		// Merge all polygons that lay in the same plane as one big polygon.
		// A convex body does not have two separate regions (separated by polygons
		// with different normals) where the same normal occurs, so we can simply
		// search all similar normals of a polygon. Two different options are 
		// possible when the normals fit:
		// - the two polygons are neighbors
		// - the two polygons aren't neighbors (but a third, fourth,.. polygon lays
		//   in between)

		// Signals if the body holds polygons which aren't neighbors but have the same
		// normal. That means another step has to be processed.
		bool bDirty = false;

		for ( size_t iPolyA = 0; iPolyA < getPolygonCount(); ++iPolyA )
		{

			for ( size_t iPolyB = iPolyA+1; iPolyB < getPolygonCount(); ++iPolyB )
			{
				const DiVec3& n1 = getNormal( iPolyA );
				const DiVec3& n2 = getNormal( iPolyB );

				// if the normals point into the same direction
				if ( n1.directionEquals( n2, DiRadian( DiDegree( 0.00001 ) ) )  )
				{
					// indicates if a neighbor has been found and joined
					bool bFound = false;

					// search the two fitting vertices (if there are any) for the common edge
					const size_t numVerticesA = getVertexCount( iPolyA );
					for ( size_t iVertexA = 0; iVertexA < numVerticesA; ++iVertexA )
					{
						const size_t numVerticesB = getVertexCount( iPolyB );
						for ( size_t iVertexB = 0; iVertexB < numVerticesB; ++iVertexB )
						{
							const DiVec3& aCurrent	= getVertex( iPolyA, iVertexA );
							const DiVec3& aNext		= getVertex( iPolyA, (iVertexA + 1) % getVertexCount( iPolyA ) );
							const DiVec3& bCurrent	= getVertex( iPolyB, iVertexB );
							const DiVec3& bNext		= getVertex( iPolyB, (iVertexB + 1) % getVertexCount( iPolyB ) );

							// if the edge is the same the current vertex of A has to be equal to the next of B and the other
							// way round
							if ( aCurrent.positionEquals(bNext) &&
								 bCurrent.positionEquals(aNext))
							{
								// polygons are neighbors, assemble new one
								DiPolygon *pNew = allocatePolygon();

								// insert all vertices of A up to the join (including the common vertex, ignoring
								// whether the first vertex of A may be a shared vertex)
								for ( size_t i = 0; i <= iVertexA; ++i )
								{
									pNew->InsertVertex( getVertex( iPolyA, i%numVerticesA ) );
								}

								// insert all vertices of B _after_ the join to the end
								for ( size_t i = iVertexB + 2; i < numVerticesB; ++i )
								{
									pNew->InsertVertex( getVertex( iPolyB, i ) );
								}

								// insert all vertices of B from the beginning up to the join (including the common vertex
								// and excluding the first vertex if the first is part of the shared edge)
								for ( size_t i = 0; i <= iVertexB; ++i )
								{
									pNew->InsertVertex( getVertex( iPolyB, i%numVerticesB ) );
								}

								// insert all vertices of A _after_ the join to the end
								for ( size_t i = iVertexA + 2; i < numVerticesA; ++i )
								{
									pNew->InsertVertex( getVertex( iPolyA, i ) );
								}

								// in case there are double vertices (in special cases), remove them
								for ( size_t i = 0; i < pNew->GetVertexCount(); ++i )
								{
									const DiVec3& a = pNew->GetVertex( i );
									const DiVec3& b = pNew->GetVertex( (i + 1) % pNew->GetVertexCount() );

									// if the two vertices are the same...
									if (a.positionEquals(b))
									{
										// remove a
										pNew->DeleteVertex( i );

										// decrement counter
										--i;
									}
								}

								// delete the two old ones
								DI_ASSERT( iPolyA != iPolyB );
								
								// polyB is always higher than polyA, so delete polyB first
								deletePolygon( iPolyB );
								deletePolygon( iPolyA );

								// continue with next (current is deleted, so don't jump to the next after the next)
								--iPolyA;
								--iPolyB;

								// insert new polygon
								insertPolygon( pNew );

								bFound = true;
								break;
							}
						}
						
						if ( bFound )
						{
							break;
						}
					}

					if ( bFound == false )
					{
						// there are two polygons available with the same normal direction, but they
						// could not be merged into one single because of no shared edge
						bDirty = true;
						break;
					}
				}
			}
		}

		// recursion to merge the previous non-neighbors
		if ( bDirty )
		{
			mergePolygons();
		}
	}

	const DiVec3& DiConvexBody::getNormal( size_t poly )
	{
		DI_ASSERT_MESSAGE( poly < getPolygonCount(), "Search position out of range" );
		
		return mPolygons[ poly ]->GetNormal();
	}

	DiAABB DiConvexBody::getAABB( void ) const
	{
		DiAABB aab;

		for ( size_t i = 0; i < getPolygonCount(); ++i )
		{
			for ( size_t j = 0; j < getVertexCount( i ); ++j )
			{
				aab.Merge( getVertex( i, j ) );
			}
		}

		return aab;
	}

	bool DiConvexBody::operator == ( const DiConvexBody& rhs ) const
	{
		if ( getPolygonCount() != rhs.getPolygonCount() )
			return false;

		// Compare the polygons. They may not be in correct order.
		// A correct convex body does not have identical polygons in its body.
		bool *bChecked = DI_NEW bool[getPolygonCount()];
		for ( size_t i=0; i<getPolygonCount(); ++i )
		{
			bChecked[ i ] = false;
		}

		for ( size_t i=0; i<getPolygonCount(); ++i )
		{
			bool bFound = false;

			for ( size_t j=0; j<getPolygonCount(); ++j )
			{
				const DiPolygon& pA = getPolygon( i );
				const DiPolygon& pB = rhs.getPolygon( j );

				if ( pA == pB )
				{
					bFound = true;
					bChecked[ i ] = true;
					break;
				}
			}

			if ( bFound == false )
			{
				DI_DELETE[] bChecked;
				bChecked = 0;
				return false;
			}
		}

		for ( size_t i=0; i<getPolygonCount(); ++i )
		{
			if ( bChecked[ i ] != true )
			{
				DI_DELETE[] bChecked;
				bChecked = 0;
				return false;
			}
		}

        DI_DELETE[] bChecked;
		bChecked = 0;
		return true;
	}

	void DiConvexBody::insertPolygon(DiPolygon* pdata, size_t poly )
	{
		DI_ASSERT_MESSAGE(poly <= getPolygonCount(), "Insert position out of range" );
		DI_ASSERT_MESSAGE( pdata != NULL, "DiConvexBody is NULL" );

		PolygonList::iterator it = mPolygons.begin();
		std::advance(it, poly);

		mPolygons.insert( it, pdata );

	}
	
	void DiConvexBody::insertPolygon(DiPolygon* pdata)
	{
		DI_ASSERT_MESSAGE( pdata != NULL, "DiConvexBody is NULL" );

		mPolygons.push_back( pdata );

	}

	void DiConvexBody::insertVertex(size_t poly, const DiVec3& vdata, size_t vertex )
	{
		DI_ASSERT_MESSAGE(poly < getPolygonCount(), "Search position (polygon) out of range" );
		
		mPolygons[poly]->InsertVertex(vdata, vertex);
	}
	//-----------------------------------------------------------------------
	void DiConvexBody::insertVertex(size_t poly, const DiVec3& vdata)
	{
		DI_ASSERT_MESSAGE(poly < getPolygonCount(), "Search position (polygon) out of range" );

		mPolygons[poly]->InsertVertex(vdata);
	}
	//-----------------------------------------------------------------------
	void DiConvexBody::deletePolygon(size_t poly)
	{
		DI_ASSERT_MESSAGE(poly < getPolygonCount(), "Search position out of range" );

		PolygonList::iterator it = mPolygons.begin();
		std::advance(it, poly);
		
		freePolygon(*it);
		mPolygons.erase(it);
	}

	DiPolygon* DiConvexBody::unlinkPolygon(size_t poly)
	{
		DI_ASSERT_MESSAGE( poly < getPolygonCount(), "Search position out of range" );

		PolygonList::iterator it = mPolygons.begin();
		std::advance(it, poly);

		// safe address
		DiPolygon *pRet = *it;
		
		// delete entry
		mPolygons.erase(it);	

		// return polygon pointer

		return pRet;
	}

	void DiConvexBody::moveDataFromBody(DiConvexBody& body)
	{
		body.mPolygons.swap(this->mPolygons);
	}

	void DiConvexBody::deleteVertex(size_t poly, size_t vertex)
	{
		DI_ASSERT_MESSAGE(poly < getPolygonCount(), "Search position out of range" );

		mPolygons[poly]->DeleteVertex(vertex);
	}

	const DiPolygon& DiConvexBody::getPolygon(size_t poly) const
	{
		DI_ASSERT_MESSAGE(poly < getPolygonCount(), "Search position out of range");

		return *mPolygons[poly];
	}

	void DiConvexBody::setPolygon(DiPolygon* pdata, size_t poly)
	{
		DI_ASSERT_MESSAGE(poly < getPolygonCount(), "Search position out of range" );
		DI_ASSERT_MESSAGE(pdata != NULL, "DiConvexBody is NULL" );

		if (pdata != mPolygons[poly])
		{
			// delete old polygon
			freePolygon(mPolygons[ poly ]);

			// set new polygon
			mPolygons[poly] = pdata;
		}
	}

	const DiVec3& DiConvexBody::getVertex(size_t poly, size_t vertex) const
	{
		DI_ASSERT_MESSAGE( poly < getPolygonCount(), "Search position out of range" );
		
		return mPolygons[poly]->GetVertex(vertex);
	}

	void DiConvexBody::setVertex(size_t poly, const DiVec3& vdata, size_t vertex)
	{
		DI_ASSERT_MESSAGE(poly < getPolygonCount(), "Search position out of range");
		
		mPolygons[poly]->SetVertex(vdata, vertex);
	}

	void DiConvexBody::storeEdgesOfPolygon(size_t poly, DiPolygon::EdgeMap *edgeMap ) const
	{
		DI_ASSERT_MESSAGE(poly <= getPolygonCount(), "Search position out of range" );
		DI_ASSERT_MESSAGE( edgeMap != NULL, "TEdgeMap ptr is NULL" );

		mPolygons[poly]->StoreEdges(edgeMap);
	}

	DiPolygon::EdgeMap DiConvexBody::getSingleEdges() const
	{
		DiPolygon::EdgeMap edgeMap;

		// put all edges of all polygons into a list every edge has to be
		// walked in each direction once	
		for ( size_t i = 0; i < getPolygonCount(); ++i )
		{
			const DiPolygon& p = getPolygon( i );

			for ( size_t j = 0; j < p.GetVertexCount(); ++j )
			{
				const DiVec3& a = p.GetVertex( j );
				const DiVec3& b = p.GetVertex( ( j + 1 ) % p.GetVertexCount() );

				edgeMap.insert( DiPolygon::Edge( a, b ) );
			}
		}

		// search corresponding parts
		DiPolygon::EdgeMap::iterator it;
		DiPolygon::EdgeMap::iterator itStart;
		DiPolygon::EdgeMap::const_iterator itEnd;
		while( !edgeMap.empty() )
		{
			it = edgeMap.begin(); ++it;	// start one element after itStart
			itStart = edgeMap.begin();	// the element to be compared with the others
			itEnd = edgeMap.end();		// beyond the last element
			
			bool bFound = false;

			for ( ; it != itEnd; ++it )
			{
				if (itStart->first.positionEquals(it->second) &&
					 itStart->second.positionEquals(it->first))
				{
					// erase itStart and it
					edgeMap.erase( it );
					edgeMap.erase( itStart );

					bFound = true;

					break; // found
				}
			}

			if ( bFound == false )
			{
				break;	// not all edges could be matched
						// body is not closed
			}
		}

		return edgeMap;
	}

	void DiConvexBody::allocateSpace( size_t numPolygons, size_t numVertices )
	{
		reset();

		// allocate numPolygons polygons with each numVertices vertices
		for ( size_t iPoly = 0; iPoly < numPolygons; ++iPoly )
		{
			DiPolygon *poly = allocatePolygon();

			for ( size_t iVertex = 0; iVertex < numVertices; ++iVertex )
			{
				poly->InsertVertex( DiVec3::ZERO );
			}

			mPolygons.push_back( poly );
		}
	}

	void DiConvexBody::clip( const DiPlane& pl, bool keepNegative )
	{
		if ( getPolygonCount() == 0 )
			return;

		// current will be used as the reference body
		DiConvexBody current;
		current.moveDataFromBody(*this);
		
		DI_ASSERT_MESSAGE( this->getPolygonCount() == 0, "Body not empty!" );
		DI_ASSERT_MESSAGE( current.getPolygonCount() != 0, "Body empty!" );

		// holds all intersection edges for the different polygons
		DiPolygon::EdgeMap intersectionEdges;

		// clip all polygons by the intersection plane
		// add only valid or intersected polygons to *this
		for ( size_t iPoly = 0; iPoly < current.getPolygonCount(); ++iPoly )
		{

			// fetch vertex count and ignore polygons with less than three vertices
			// the polygon is not valid and won't be added
			const size_t vertexCount = current.getVertexCount( iPoly );
			if ( vertexCount < 3 )
				continue;

			// current polygon
			const DiPolygon& p = current.getPolygon( iPoly );

			// the polygon to assemble
			DiPolygon *pNew = allocatePolygon();

			// the intersection polygon (indeed it's an edge or it's empty)
			DiPolygon *pIntersect = allocatePolygon();
			
			// check if polygons lie inside or outside (or on the plane)
			// for each vertex check where it is situated in regard to the plane
			// three possibilities appear:
			DiPlane::Side clipSide = keepNegative ? DiPlane::POSITIVE_SIDE : DiPlane::NEGATIVE_SIDE;
			// - side is clipSide: vertex will be clipped
			// - side is !clipSide: vertex will be untouched
			// - side is NOSIDE:   vertex will be untouched
			DiPlane::Side *side = DI_NEW DiPlane::Side[vertexCount];
			for ( size_t iVertex = 0; iVertex < vertexCount; ++iVertex )
			{
				side[ iVertex ] = pl.getSide( p.GetVertex( iVertex ) );
			}

			// now we check the side combinations for the current and the next vertex
			// four different combinations exist:
			// - both points inside (or on the plane): keep the second (add it to the body)
			// - both points outside: discard both (don't add them to the body)
			// - first vertex is inside, second is outside: add the intersection point
			// - first vertex is outside, second is inside: add the intersection point, then the second
			for ( size_t iVertex = 0; iVertex < vertexCount; ++iVertex )
			{
				// determine the next vertex
				size_t iNextVertex = ( iVertex + 1 ) % vertexCount;

				const DiVec3& vCurrent = p.GetVertex( iVertex );
				const DiVec3& vNext    = p.GetVertex( iNextVertex );

				// case 1: both points inside (store next)
				if ( side[ iVertex ]     != clipSide &&		// NEGATIVE or NONE
					 side[ iNextVertex ] != clipSide )		// NEGATIVE or NONE
				{
					// keep the second
					pNew->InsertVertex( vNext );
				}

				// case 3: inside -> outside (store intersection)
				else if ( side[ iVertex ]		!= clipSide &&
						  side[ iNextVertex ]	== clipSide )
				{
					// Do an intersection with the plane. We use a ray with a start point and a direction.
					// The ray is forced to hit the plane with any option available (eigher current or next
					// is the starting point)

					// intersect from the outside vertex towards the inside one
					DiVec3 vDirection = vCurrent - vNext;
					vDirection.normalise();
					DiRay ray( vNext, vDirection );
					auto intersect = ray.intersects( pl );

					// store intersection
					if ( intersect.first )
					{
						// convert distance to vector
						DiVec3 vIntersect = ray.getPoint( intersect.second );	

						// store intersection
						pNew->InsertVertex( vIntersect );
						pIntersect->InsertVertex( vIntersect );
					}
				}

				// case 4: outside -> inside (store intersection, store next)
				else if ( side[ iVertex ]		== clipSide &&
					side[ iNextVertex ]			!= clipSide )
				{
					// Do an intersection with the plane. We use a ray with a start point and a direction.
					// The ray is forced to hit the plane with any option available (eigher current or next
					// is the starting point)

					// intersect from the outside vertex towards the inside one
					DiVec3 vDirection = vNext - vCurrent;
					vDirection.normalise();
					DiRay ray( vCurrent, vDirection );
					auto intersect = ray.intersects( pl );

					// store intersection
					if ( intersect.first )
					{
						// convert distance to vector
						DiVec3 vIntersect = ray.getPoint( intersect.second );

						// store intersection
						pNew->InsertVertex( vIntersect );
						pIntersect->InsertVertex( vIntersect );
					}

					pNew->InsertVertex( vNext );

				}
				// else:
				// case 2: both outside (do nothing)
					
			}

			// insert the polygon only, if at least three vertices are present
			if ( pNew->GetVertexCount() >= 3 )
			{
				// in case there are double vertices, remove them
				pNew->RemoveDuplicates();

				// in case there are still at least three vertices, insert the polygon
				if ( pNew->GetVertexCount() >= 3 )
				{
					this->insertPolygon( pNew );
				}
				else
				{
					// delete pNew because it's empty or invalid
					freePolygon(pNew);
					pNew = 0;
				}
			}
			else
			{
				// delete pNew because it's empty or invalid
				freePolygon(pNew);
				pNew = 0;
			}

			// insert intersection polygon only, if there are two vertices present
			if ( pIntersect->GetVertexCount() == 2 )
			{
				intersectionEdges.insert( DiPolygon::Edge( pIntersect->GetVertex( 0 ),
														  pIntersect->GetVertex( 1 ) ) );
			}

			// delete intersection polygon
			// vertices were copied (if there were any)
			freePolygon(pIntersect);
			pIntersect = 0;

			// delete side info
			DI_DELETE[] side;
			side = 0;
		}

		// if the polygon was partially clipped, close it
		// at least three edges are needed for a polygon
		if ( intersectionEdges.size() >= 3 )
		{
			DiPolygon *pClosing = allocatePolygon();

			// Analyze the intersection list and insert the intersection points in ccw order
			// Each point is twice in the list because of the fact that we have a convex body
			// with convex polygons. All we have to do is order the edges (an even-odd pair)
			// in a ccw order. The plane normal shows us the direction.
			DiPolygon::EdgeMap::iterator it = intersectionEdges.begin();

			// check the cross product of the first two edges
			DiVec3 vFirst  = it->first;
			DiVec3 vSecond = it->second;

			// remove inserted edge
			intersectionEdges.erase( it );

			DiVec3 vNext;

			// find mating edge
			if (findAndEraseEdgePair(vSecond, intersectionEdges, vNext))
			{
				// detect the orientation
				// the polygon must have the same normal direction as the plane and then n
				DiVec3 vCross = ( vFirst - vSecond ).crossProduct( vNext - vSecond );
				bool frontside = ( pl.normal ).directionEquals( vCross, DiDegree( 1 ) );

				// first inserted vertex
				DiVec3 firstVertex;
				// currently inserted vertex
				DiVec3 currentVertex;
				// direction equals -> front side (walk ccw)
				if ( frontside )
				{
					// start with next as first vertex, then second, then first and continue with first to walk ccw
					pClosing->InsertVertex( vNext );
					pClosing->InsertVertex( vSecond );
					pClosing->InsertVertex( vFirst );
					firstVertex		= vNext;
					currentVertex	= vFirst;
				}
				// direction does not equal -> back side (walk cw)
				else
				{
					// start with first as first vertex, then second, then next and continue with next to walk ccw
					pClosing->InsertVertex( vFirst );
					pClosing->InsertVertex( vSecond );
					pClosing->InsertVertex( vNext );
					firstVertex		= vFirst;
					currentVertex	= vNext;

				}

				// search mating edges that have a point in common
				// continue this operation as long as edges are present
				while ( !intersectionEdges.empty() )
				{

					if (findAndEraseEdgePair(currentVertex, intersectionEdges, vNext))
					{
						// insert only if it's not the last (which equals the first) vertex
						if ( !intersectionEdges.empty() )
						{
							currentVertex = vNext;
							pClosing->InsertVertex( vNext );
						}
					}
					else
					{
						// degenerated...
						break;
					}

				} // while intersectionEdges not empty

				// insert polygon (may be degenerated!)
				this->insertPolygon( pClosing );

			}
			// mating intersection edge NOT found!
			else
			{
				freePolygon(pClosing);
			}

		} // if intersectionEdges contains more than three elements
	}

	bool DiConvexBody::findAndEraseEdgePair(const DiVec3& vec, 
		DiPolygon::EdgeMap& intersectionEdges, DiVec3& vNext ) const
	{
		for (DiPolygon::EdgeMap::iterator it = intersectionEdges.begin(); 
			it != intersectionEdges.end(); ++it)
		{
			if (it->first.positionEquals(vec))
			{
				vNext = it->second;

				// erase found edge
				intersectionEdges.erase( it );

				return true; // found!
			}
			else if (it->second.positionEquals(vec))
			{
				vNext = it->first;

				// erase found edge
				intersectionEdges.erase( it );

				return true; // found!
			}
		}

		return false; // not found!
	}
}

