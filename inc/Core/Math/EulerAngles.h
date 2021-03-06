/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Math/Vector.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Representation of an orientation as a set of Euler angles that can be
 * transformed into and back in a rotation matrix and quaternions.
 */

class API_CORE EulerAngles
{
public:

	EulerAngles();
	EulerAngles( float x, float y, float z );

	// Copy constructor.
	EulerAngles(const EulerAngles&);

	// Addition combined.
	EulerAngles& operator += (const EulerAngles&);

	// Addition combined.
	EulerAngles& operator += (const Vector3&);

	// Adds the given angles to the existing angles.
	void rotate( float x, float y, float z );

	// Resets the rotations so they don't have any effect.
	void identity();

public:

	float x, y, z;
};

//-----------------------------------//

NAMESPACE_CORE_END