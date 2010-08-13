/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Model.h"
#include "vapor/resources/Mesh.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Model, Geometry)
END_CLASS()

//-----------------------------------//

Model::Model( const MeshPtr& mesh )
	: mesh(mesh)
{
	assert( mesh != nullptr );
}

//-----------------------------------//

void Model::update( double delta )
{
	if( !mesh->isLoaded() )
		return;

	if( !mesh->isBuilt() )
	{
		std::vector<RenderablePtr> renderables;
		
		mesh->build( renderables );

		foreach( const RenderablePtr& rend, renderables )
			addRenderable( rend );
	}

	Geometry::update(delta);
}

//-----------------------------------//

} // end namespace