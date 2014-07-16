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

#ifndef __ConvexBody_H__
#define __ConvexBody_H__

#include "MiscPrerequisites.h"
#include "Math/Polygen.h"

namespace Demi
{
	/** Holds a solid representation of a convex body.
		@remarks
			Administers a convex body. All polygons of the body are convex and
			planar. Several operations may be applied, ranging from intersection
			to join where each result it itself a convex body.
	*/
	class DI_GFX_API DiConvexBody
	{
	public:
		typedef DiVector<DiPolygon*> PolygonList;

	protected:
		PolygonList mPolygons;

		// Static 'free list' of polygons to save reallocation, shared between all bodies
		static PolygonList msFreePolygons;

	public:
		DiConvexBody();
        
		~DiConvexBody();
        
		DiConvexBody( const DiConvexBody& cpy );

		/** Build a new polygon representation from a frustum.
		*/
		void define(const DiFrustum& frustum);

		/** Build a new polygon representation from an AAB.
		*/
		void define(const DiAABB& aab);

		/** Clips the body with a frustum. The resulting holes
			are filled with new polygons.
		*/
		void clip( const DiFrustum& frustum );

		/** Clips the body with an AAB. The resulting holes
			are filled with new polygons.
		*/
		void clip( const DiAABB& aab );

		/** Clips the body with another body.
		*/
		void clip(const DiConvexBody& body);

		/** Clips the object by the positive half space of a plane
		*/
		void clip(const DiPlane& pl, bool keepNegative = true);

		/** Extends the existing body to incorporate the passed in point as a
			convex hull.
		@remarks
			You must already have constructed a basic body using a 'construct' 
			method.
		*/
		void extend(const DiVec3& pt);

		/** Resets the object.
		*/
		void reset( void );

		/** Returns the current number of polygons.
		*/
		size_t getPolygonCount( void ) const;

		/** Returns the number of vertices for a polygon
		*/
		size_t getVertexCount( size_t poly ) const;

		/** Returns a polygon.
		*/
		const DiPolygon& getPolygon( size_t poly ) const;

		/** Returns a specific vertex of a polygon.
		*/
		const DiVec3& getVertex( size_t poly, size_t vertex ) const;

		/** Returns the normal of a specified polygon.
		*/
		const DiVec3& getNormal( size_t poly );

		/** Returns an AABB representation.
		*/
		DiAABB getAABB( void ) const;

		/** Checks if the body has a closed hull.
		*/
		bool hasClosedHull( void ) const;

		/** Merges all neighboring polygons into one single polygon if they are
			lay in the same plane.
		*/
		void mergePolygons( void );

		/** Determines if the current object is equal to the compared one.
		*/
		bool operator == ( const DiConvexBody& rhs ) const;

		/** Determines if the current object is not equal to the compared one.
		*/
		bool operator != ( const DiConvexBody& rhs ) const
		{ return !( *this == rhs ); }

		/// Initialise the internal polygon pool used to minimise allocations
		static void _initialisePool();
		/// Tear down the internal polygon pool used to minimise allocations
		static void _destroyPool();


	protected:
		/** Get a new polygon from the pool.
		*/
		static DiPolygon* allocatePolygon();
        
		/** Release a polygon back tot he pool. */
		static void freePolygon(DiPolygon* poly);
        
		/** Inserts a polygon at a particular point in the body.
		@note
			After this method is called, the DiConvexBody 'owns' this DiPolygon
			and will be responsible for deleting it.
		*/
		void insertPolygon(DiPolygon* pdata, size_t poly);
		/** Inserts a polygon at the end.
		@note
			After this method is called, the DiConvexBody 'owns' this DiPolygon
			and will be responsible for deleting it.
		*/
		void insertPolygon(DiPolygon* pdata);

		/** Inserts a vertex for a polygon at a particular point.
		@note
			No checks are done whether the assembled polygon is (still) planar, 
			the caller must ensure that this is the case.
		*/
		void insertVertex(size_t poly, const DiVec3& vdata, size_t vertex);
		/** Inserts a vertex for a polygon at the end.
		@note
			No checks are done whether the assembled polygon is (still) planar, 
			the caller must ensure that this is the case.
		*/
		void insertVertex(size_t poly, const DiVec3& vdata);
		/** Deletes a specific polygon.
		*/
		void deletePolygon(size_t poly);

		/** Removes a specific polygon from the body without deleting it.
		@note
			The retrieved polygon needs to be deleted later by the caller.
		*/
		DiPolygon* unlinkPolygon(size_t poly);

		/** Moves all polygons from the parameter body to this instance.
		@note Both the passed in object and this instance are modified
		*/
		void moveDataFromBody(DiConvexBody& body);

		/** Deletes a specific vertex of a specific polygon.
		*/
		void deleteVertex(size_t poly, size_t vertex);

		/** Replace a polygon at a particular index.
		@note Again, the passed in polygon is owned by this object after this
			call returns, and this object is resonsible for deleting it.
		*/
		void setPolygon(DiPolygon* pdata, size_t poly );

		/** Replace a specific vertex of a polygon.
		@note
			No checks are done whether the assembled polygon is (still) planar, 
			the caller must ensure that this is the case.
		*/
		void setVertex( size_t poly, const DiVec3& vdata, size_t vertex );

		/** Returns the single edges in an EdgeMap (= edges where one side is a vertex and the
			other is empty space (a hole in the body)).
		*/
		DiPolygon::EdgeMap getSingleEdges() const;

		/** Stores the edges of a specific polygon in a passed in structure.
		*/
		void storeEdgesOfPolygon(size_t poly, DiPolygon::EdgeMap *edgeMap) const;
			
		/** Allocates space for an specified amount of polygons with
			each of them having a specified number of vertices.
			@note
				Old data (if available) will be erased.
		*/
		void allocateSpace(size_t numPolygons, size_t numVertices);

		/** Searches for a pair (an edge) in the intersectionList with an entry
			that equals vec, and removes it from the passed in list.
		@param vec The vertex to search for in intersectionEdges
		@param intersectionEdges A list of edges, which is updated if a match is found
		@param vNext A reference to a vector which will be filled with the other
			vertex at the matching edge, if found.
		@return True if a match was found
		*/
		bool findAndEraseEdgePair(const DiVec3& vec, 
			DiPolygon::EdgeMap& intersectionEdges, DiVec3& vNext ) const;

	};
	/** @} */
	/** @} */

}

#endif 

