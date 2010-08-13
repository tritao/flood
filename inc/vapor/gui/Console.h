/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Font.h"
#include "vapor/render/Material.h"
#include "vapor/scene/Geometry.h"
#include "vapor/gui/Overlay.h"

namespace vapor {

//-----------------------------------//

/**
 * Scripting console shown in-game to control the engine at runtime via.
 * text commands. It has a scrolling list of text and also a text input 
 * box where you input a command and press <Enter>. The engine (Lua) will
 * evaluate it and execute the action. As this will be based on the Lua
 * scripting engine, you can pretty much evaluate anything that is bound
 * to its environment.
 */

class VAPOR_API Console : public Overlay
{
	DECLARE_CLASS_()

public:

	Console();

	// Updates the console geometry if needed.
	virtual void update( double delta );

protected:

	// Builds the initial geometry of the console.
	void buildGeometry();
	
	// Used to track if the geometry needs updating.
	bool isDirty;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Console );

//-----------------------------------//

} // end namespace