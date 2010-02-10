/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Texture.h"

using namespace vapor::resources;

namespace vapor { namespace render {

//-----------------------------------//

Texture::Texture( resources::ImagePtr img )
	: uploaded( false ), img( img ), id( 0 )
{
	generate();
	bind();
	upload();
	unbind();
}

//-----------------------------------//

Texture::~Texture()
{
	glDeleteTextures( 1, &id );

#ifdef VAPOR_DEBUG
	while( glGetError() != GL_NO_ERROR )
	{
		warn( "gl", "Could not delete texture object '%d'", id );
	}
#endif
}

//-----------------------------------//

bool Texture::generate()
{
	glGenTextures( 1, &id );

#ifdef VAPOR_DEBUG
	while( glGetError() != GL_NO_ERROR )
	{
		warn( "gl", "Could not generate a new texture object '%d'", id );
		return false;
	}
#endif

	return true;
}

//-----------------------------------//

bool Texture::check()
{
	GLint max_size;
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &max_size );

	if( (img->getWidth() > max_size)
		|| (img->getHeight() > max_size) )
	{
		warn( "gl", "Texture size is not supported for image '%s'", img->getURI().c_str() );		
		return false;
	}

	return true;
}

//-----------------------------------//

bool Texture::upload()
{
	// TODO: check for maximum texture size the implementation
	// can handle...
	// TODO: check for OpenGL errors

	if( !check() ) return false;

	// TODO: check the formats more thoroughly
	glTexImage2D( GL_TEXTURE_2D, 0, 
		convertInternalFormat( img->getPixelFormat() ),
		img->getWidth(), img->getHeight(), 0,
		convertSourceFormat( img->getPixelFormat() ),
		GL_UNSIGNED_BYTE, &img->getBuffer()[0] );

	configure();

	//gluBuild2DMipmaps( GL_TEXTURE_2D,
	//	convertInternalFormat( img->getPixelFormat() ),
	//	img->getWidth(), img->getHeight(),
	//	GL_RGBA, GL_UNSIGNED_BYTE, &img->getBuffer()[0] );

#ifdef VAPOR_DEBUG
	while( glGetError() != GL_NO_ERROR )
	{
		warn( "gl", "Could not upload texture object '%d'", id );
		uploaded = false;
		return false;
	}
#endif

	uploaded = true;
	return true;
}

//-----------------------------------//

void Texture::configure()
{
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// TODO: mipmaps and stuff
}

//-----------------------------------//

void Texture::bind( int unit )
{
	glActiveTexture( GL_TEXTURE0+unit );
	glBindTexture( GL_TEXTURE_2D, id );
}

//-----------------------------------//

void Texture::unbind( int unit )
{
	glActiveTexture( GL_TEXTURE0+unit );
	glBindTexture( GL_TEXTURE_2D, 0 );

	//glDisable( GL_TEXTURE_2D );
}

//-----------------------------------//

GLint Texture::convertInternalFormat( resources::PixelFormat::Enum fmt )
{
	switch( fmt )
	{
	case PixelFormat::R8G8B8A8:
		return GL_RGBA8;
	case PixelFormat::R8G8B8:
		return GL_RGB8;
	default:
		warn( "GL", "Implement support for more pixel formats" );
		return GL_RGB8;
	}
}

//-----------------------------------//

GLint Texture::convertSourceFormat( resources::PixelFormat::Enum fmt )
{
	switch( fmt )
	{
	case PixelFormat::R8G8B8A8:
		return GL_RGBA;
	case PixelFormat::R8G8B8:
		return GL_RGB;
	default:
		warn( "GL", "Implement support for more pixel formats" );
		return GL_RGB;
	}
}

//-----------------------------------//

resources::ImagePtr Texture::getImage() const
{
	return img;
}

//-----------------------------------//

void Texture::setImage( const resources::ImagePtr& image )
{
	img = image;
}

//-----------------------------------//

} } // end namespaces