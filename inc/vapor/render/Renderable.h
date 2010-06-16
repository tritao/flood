/************************************************************************
*
* vaporEngine by triton (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/VertexBuffer.h"
#include "vapor/render/IndexBuffer.h"
#include "vapor/render/Material.h"
#include "vapor/render/GL.h"

FWD_DECL_TYPEDEF_PTR(RenderDevice)
//FWD_DECL_TYPEDEF_INT(Material)
//FWD_DECL_TYPEDEF_INT(VertexBuffer)
//FWD_DECL_TYPEDEF_INT(IndexBuffer)

namespace vapor {

//-----------------------------------//

/**
 * Type of primitive of the renderable.
 */

namespace Primitive
{
    enum Enum
    {
        Points			= GL_POINTS,
        Lines			= GL_LINES,
        LineLoop		= GL_LINE_LOOP,
        LineStrip		= GL_LINE_STRIP,
        Triangles		= GL_TRIANGLES,
        TriangleStrip	= GL_TRIANGLE_STRIP,
        TriangleFan		= GL_TRIANGLE_FAN,
        Quads			= GL_QUADS,
        QuadStrip		= GL_QUAD_STRIP,
        Polygon			= GL_POLYGON
    };
}

//-----------------------------------//

/**
 * Type of primitive of the renderable.
 */

namespace PolygonMode
{
    enum Enum
    {
		Solid = GL_FILL,
		Wireframe = GL_LINE,
		Point = GL_POINT
    };
}

//-----------------------------------//

/**
 * Represents a renderable object, that is, the only rendering object
 * the rendering device knows and cares about. A renderable object has
 * at least one vertex buffer and one index buffer, and a primitive 
 * type associated with them.
 */

class VAPOR_API Renderable : public ReferenceCounted
{
public:

	Renderable();

    Renderable( Primitive::Enum primitive, const VertexBufferPtr& vb,
		const IndexBufferPtr& ib, const MaterialPtr& mat );
    
    // No index buffer, default material will be used if none passed
	Renderable(Primitive::Enum primitive, const VertexBufferPtr& vb = VertexBufferPtr(),
			const MaterialPtr& mat = MaterialPtr() );

	// Bind/unbind all the state needed to draw the renderable.
	void bind();
	void unbind();

    // Render this renderable. This will bind all the necessary state
	// like binding the buffers and the materials.
	void render(const RenderDevicePtr& device);
    
    // Gets/sets the vertex buffer associated with the renderable.
	ACESSOR(VertexBuffer, VertexBufferPtr, vb) 
   
    // Gets/sets the index buffer associated with the renderable.
	ACESSOR(IndexBuffer, IndexBufferPtr, ib)

    // Gets/sets the material associated with the renderable.
    ACESSOR(Material, MaterialPtr, mat)

    // Gets/sets the render mode of this renderable.
	ACESSOR(PolygonMode, PolygonMode::Enum, mode)

	// Gets/sets the primitive type of this renderable.
	ACESSOR(PrimitiveType, Primitive::Enum, type)

protected:

    // Primitive type of the vertex buffer contents.
    Primitive::Enum type;

	// Rendering mode of the vertex buffer contents.
	PolygonMode::Enum mode;

	// Vertex buffer with all the vertices.
    VertexBufferPtr vb;

	// Index buffer with all the indices.
    IndexBufferPtr ib;
    
	// Material of this renderable.
	MaterialPtr mat;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Renderable );

//-----------------------------------//

} // end namespace