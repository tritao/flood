/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/PCH.h>

#include <vapor/Framework.h>

#include <limits>

using namespace vapor::input;

namespace vapor {

//-----------------------------------//

Framework::Framework(const std::string app, const char** argv)
	:  Engine(app, argv, false), numFrames( 0 ),
	minFrameTime( std::numeric_limits< double >::max() ),
	maxFrameTime( 0.0 ), sumFrameTime( 0.0 ), lastFrameTime( 0.0 )
{
	info( "framework", "Engine framework getting into action" );
}

//-----------------------------------//

Framework::~Framework()
{
	
}

//-----------------------------------//

void Framework::run()
{
	// init the engine
	Framework::init();

	// call the render cycle
	render();
}

//-----------------------------------//

void Framework::init()
{
	// init the engine
	Engine::init();

	// register input callbacks
	registerCallbacks();

	// app-specific initialization
	onInit();

	// set up all the resources
	onSetupResources();

	// set up the scene
	onSetupScene();
}

//-----------------------------------//

/**
 * Check out this article for a detailed analysis of some possible 
 * game loop implementations: http://dewitters.koonsolo.com/gameloop.html
 */

void Framework::render()
{
	render::Device* renderDevice = getRenderDevice();

	while( renderDevice->getWindow()->pumpEvents() )
	{
		frameTimer.reset();

		// update time!
		onUpdate( lastFrameTime );

		// main rendering by app
		onRender();

		// update the active target
		renderDevice->updateTarget();

		lastFrameTime = frameTimer.getElapsedTime();

		updateFrameTimes();
	}
}

//-----------------------------------//

void Framework::registerCallbacks()
{
	Keyboard* kbd = getInputManager()->getKeyboard();
	Mouse* mouse = getInputManager()->getMouse();

	if( kbd ) kbd->onKeyPress.bind( &Framework::onKeyPressed, this );
	if( mouse ) mouse->onMouseButtonPress.bind( &Framework::onButtonPressed, this );
}

//-----------------------------------//

void Framework::updateFrameTimes()
{
	numFrames++;

	minFrameTime = std::min( minFrameTime, lastFrameTime );
	maxFrameTime = std::max( maxFrameTime, lastFrameTime );

	sumFrameTime += lastFrameTime;
	avgFrameTime = sumFrameTime / numFrames;
}

//-----------------------------------//

void Framework::onKeyPressed( const KeyEvent& )
{

}

//-----------------------------------//

void Framework::onButtonPressed( const MouseButtonEvent& )
{

}

//-----------------------------------//

} // end namespace