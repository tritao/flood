/************************************************************************
*
*  Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/API.h"
#include "Core/References.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class RenderCapabilities;
class RenderDevice;
class RenderTarget;
class RenderBackend;

class BufferManager;
class ProgramManager;
class TextureManager;

struct Color;
class RenderBuffer;
class Settings;

struct API_GRAPHICS RenderContextSettings
{
	RenderContextSettings();

	uint16 bitsPerPixel;
	uint16 depthBits;
	uint16 stencilBits;
	uint16 antialiasLevel;
};

/**
 * Represents a rendering context.
 */

class API_GRAPHICS RenderContext : public ReferenceCounted
{
public:

	RenderContext();
	virtual ~RenderContext();

	// Makes the context current.
	virtual void makeCurrent(RenderTarget* target) = 0;

	// Sets the current render target.
	void setTarget(RenderTarget* target);

	// Initializes the context.
	void init();

	// Resets the device to the initial state.
	void resetState();

	// Gets the framebuffer color in the given pixel location.
	// Note that point (0,0) is located in the top left corner.
	Color getPixel(uint16 x, uint16 y) const;

	// Logs the GPU capabilities.
	void showCapabilities(RenderCapabilities*);

	// Creates a new render buffer (offscreen render target).
	RenderBuffer* createRenderBuffer( const Settings& );

	// Render capabilities information.
	RenderCapabilities* caps;

	// Manages all the buffers.
	BufferManager* bufferManager;

	// Manages all the textures.
	TextureManager* textureManager;

	// Manages all the shaders.
	ProgramManager* programManager;

	// Keeps if the context initialization has already been done.
	bool initDone;

	// Current render target.
	RenderTarget* currentTarget;

	// Render backend.
	RenderBackend* backend;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( RenderContext );

//-----------------------------------//

NAMESPACE_GRAPHICS_END