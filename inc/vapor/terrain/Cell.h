/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/render/Renderable.h"
#include "vapor/terrain/Terrain.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Cells are pieces of terrain geometry. They are further subdivided in 
 * cells, that are conceptually similar to tiles in 2D games, and allow
 * you to aligned like in an horizontal grid and are identified by
 * integer coordinates.
 */

class VAPOR_API Cell : public render::Renderable
{
public:

	Cell( const TerrainSettings& settings, 
		const std::vector<float>& heights, int x, int y );
	
	~Cell();

	// Gets the heights of this Cell of terrain.
	const std::vector<float>& getHeights();

	// Sets the heights of this Cell of terrain.
	void setHeights( const std::vector<float>& heights );

	// Generates the terrain grid geometry.
	void updateGeometry();

	// Calculate the normals of the geometry.
	// void calculateNormals();

protected:

	// Coordinates of this cell of terrain.
	int x, y;

	// Stores the heights of this cell of terrain.
	std::vector<float> heights;
	
	// Terrain settings.
	const TerrainSettings& settings;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Cell );

//-----------------------------------//

} } // end namespaces