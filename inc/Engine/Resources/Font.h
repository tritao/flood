/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "Graphics/Resources/Image.h"
#include "Core/Math/Vector.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * A glyph is the information about a character/symbol in a font file.
 * This structure stores more specifically information related to its
 * position and size in the texture font file.
 */

struct Glyph
{
	uint16 x;
	uint16 y;

	uint16 width;
	uint16 height;
};

//-----------------------------------//

/**
 * A font is a sequence of characters that can be loaded as either a 
 * pre-rendered texture with the glyph information being provided as
 * a separate file, or as a TTF file that will be rendered to a
 * texture at runtime and the glyph information extracted from the 
 * tables provided by the font file. With a TTF file you are not
 * limited to a unique font size or a unique set of glyphs that were
 * pre-rendered, so you might be able to achieve better quality using
 * less resources, as the engine can create a unique texture with
 * glyphs from different fonts, and with different sizes.
 */

REFLECT_DECLARE_CLASS(Font)

class API_ENGINE Font : public Resource
{
	REFLECT_DECLARE_OBJECT(Font)

public:

	// Gets the image that backs this font.
	virtual const ImageHandle& getImage() const = 0;

	// Gets an array with the available glyphs information.
	virtual const std::vector<Glyph>& getGlyphs() const = 0;

	// Gets the size information of a glyph.
	virtual const Vector2 getGlyphSize() const = 0;

	// Gets/sets the font name.
	ACCESSOR(Name, const String&, name)

	// Gets/sets the font size.
	ACCESSOR(Size, int, size)

	// Gets the resource group of the font.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Fonts)

	// Font name.
	String name;
	
	// Font size.
	int size;

protected:

	Font();
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Font );

//-----------------------------------//

NAMESPACE_ENGINE_END
