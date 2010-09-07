/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Utilities.h"
#include "vapor/math/Vector3.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Ray.h"

namespace vapor {

//-----------------------------------//

/**
 * Axis-aligned Bounding Box.
 */

class VAPOR_API BoundingBox
{
public:

	BoundingBox();
	BoundingBox( const Vector3& min, const Vector3& max );
	BoundingBox( const BoundingBox& box );

	// Expands the bounding box to contain the vertex.
	void add( const Vector3& vertex );

	// Expands the bounding box to contain the bounding box.
	void add( const BoundingBox& box );

	// Resets the bounding box.
	void reset();

	// Gets the center point of the bounding box.
	Vector3 getCenter() const;

	// Gets the chosen corner vertice.
	Vector3 getCorner( int index ) const;

	// Intersection with a ray.
	bool intersects( const Ray& ray, float& distance ) const;
	
	// Transforms the BoundingBox by a matrix.
	BoundingBox transform( const Matrix4x3& mat ) const;

public:

	Vector3 min;
	Vector3 max;
};

//-----------------------------------//

} // end namespace
