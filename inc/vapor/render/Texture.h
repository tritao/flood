/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Image.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Represents a texture on the rendering API. This will be basically an
 * the same content of a resource image stored on the graphics card.
 * TODO: compressed texture?
 */

class VAPOR_API Texture
{
public:

	Texture( ushort width, ushort height );
	Texture( resources::ImagePtr );
	~Texture();

	// Generates a new texture id.
	bool generate();

	// Uploads the image data to the graphics card.
	bool upload();

	// Binds the texture object.
	void bind( int unit = 0 );

	// Unbinds the texture object.
	void unbind( int unit = 0 );

	// Configures the texture settings.
	void configure();

	// Checks if the texture size is supported.
	bool check();

	// Gets the associated identifier.
	uint id() const;

	// Gets the associated image.
	resources::ImagePtr getImage() const;

	// Sets the associated image.
	void setImage( const resources::ImagePtr& image );

private:

	void init();

	GLint convertSourceFormat( resources::PixelFormat::Enum fmt );
	GLint convertInternalFormat( resources::PixelFormat::Enum fmt );

	// OpenGL texture object id
	GLuint _id;

	uint width, height;
	resources::ImagePtr img;

	bool uploaded;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Texture );

//-----------------------------------//

} } // end namespaces