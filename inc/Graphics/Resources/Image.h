/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "Core/Math/Color.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Possible pixel formats for an image.
 */

API_GRAPHICS REFLECT_DECLARE_ENUM(PixelFormat)

enum struct PixelFormat 
{
	// Uncompressed formats
	R8G8B8A8 = 0,
	R8G8B8,
	B8G8R8,
	B8G8R8A8,

	// Compressed formats
	DXT1,
	DXT1a,
	DXT3,
	DXT5,
	DXT5nm,

	// Misc. formats
	Depth,
	Unknown
};

//-----------------------------------//

/**
 * Represents an image in the engine. An image usually holds uncompressed 
 * image data, stored in an pixel array in a given pixel format specified 
 * at creation time.
 */

API_GRAPHICS REFLECT_DECLARE_CLASS(Image)

class API_GRAPHICS Image : public Resource
{
	REFLECT_DECLARE_OBJECT(Image)

public:

	Image();
	Image(uint32 width, uint32 height, PixelFormat format);

	// Gets/sets the width of the image.
	ACESSOR(Width, uint32, width)
	
	// Gets/sets the height of the image.
	ACESSOR(Height, uint32, height)

	// Gets/sets the pixel format of the image.
	ACESSOR(PixelFormat, PixelFormat, format)

	// Gets/sets the buffer containing the image data.
	ACESSOR(Buffer, const std::vector<byte>&, buffer)

	// Returns if the image is in a compressed format.
	bool isCompressed() const;

	// Gets/sets the buffer containing the image data.
	std::vector<uint8>& getBuffer() { return buffer; }

	// Return the proper resource group for this resource.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Images)

	// Sets the image with the given color.
	void setColor( const Color& color );

	// Prints image information to the log.
	void log() const;

	// Creates the image with the given details.
	void create(uint32 width, uint32 height, PixelFormat format);

	// Image width.
	uint32 width;

	// Image height.
	uint32 height;

	// Pixel format.
	PixelFormat format;

	// Image data.
	std::vector<uint8> buffer;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Image );
TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Image );

API_GRAPHICS ImageHandle ImageCreate(Allocator*, uint32 width, uint32 height, PixelFormat);

struct Stream;

class API_GRAPHICS ImageWriter
{
public:

	// Saves the image contents to a stream.
	void save( Image* image, Stream* stream );

	// Converts the image to the output format.
	bool convert( Image* image );

protected:

	std::vector<uint8> output;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END