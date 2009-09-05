/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SHADER_GLSL

#include "vapor/render/GLSL_Shader.h"

#include "vapor/render/GL.h"

using namespace vapor::resources;

namespace vapor {
	namespace render {

//-----------------------------------//

GLSL_Shader::GLSL_Shader( ShaderType::Enum e, const std::string& text )
	: Shader( e, text )
{
	shaderId = glCreateShader( getGLShaderType(e) );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "glsl", "Could not create a new shader object" );
		return;
	}
#endif
}

//-----------------------------------//

GLSL_Shader::~GLSL_Shader()
{
	glDeleteShader( shaderId );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "glsl", 
			"Could not delete shader object '%d'", shaderId );
		return;
	}
#endif
}

//-----------------------------------//

bool GLSL_Shader::compile()
{
	upload();

	glCompileShader( shaderId );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "glsl", "Could not compile shader object '%d'", shaderId );
	}
#endif

	getGLSLLog();

	GLint status;
	glGetShaderiv( shaderId, GL_COMPILE_STATUS, &status );

	if( status != GL_TRUE ) 
	{
		compiled = false;
		return false; 
	}

	compiled = true;
	return true;
}

//-----------------------------------//

bool GLSL_Shader::upload()
{
	if( text.empty() ) return false;

	const char* str = text.c_str();

	glShaderSource( shaderId, 1, &str, nullptr );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "glsl", "Could not upload shader text to object '%d'", shaderId );
		return false;
	}
#endif

	return true;
}

//-----------------------------------//

void GLSL_Shader::getGLSLLog()
{
	// get compilation log size
	GLint size;
	glGetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &size );

	if( size == 0)
	{
		log.clear();
		return;
	}

	// TODO: move directly to string...

	GLchar* info = new char[size];
	GLsizei length;

	glGetShaderInfoLog( shaderId, size, &length, info );

	log.assign( info );

	delete info;
}

//-----------------------------------//

GLenum GLSL_Shader::getGLShaderType( resources::ShaderType::Enum type )
{
	switch( type )
	{
	case ShaderType::Vertex:
		return GL_VERTEX_SHADER;
	case ShaderType::Fragment:
		return GL_FRAGMENT_SHADER;
	case ShaderType::Geometry:
		return GL_GEOMETRY_SHADER_EXT;
	default:
		return GL_VERTEX_SHADER;
	}	
}

//-----------------------------------//

} } // end namespaces

#endif