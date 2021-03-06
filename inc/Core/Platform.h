/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Platform detection
//---------------------------------------------------------------------//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
	#define PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(MACOSX)
	#define PLATFORM_MACOSX
#elif defined(__LINUX) || defined(__LINUX__) || defined(LINUX)
	#define PLATFORM_LINUX
#elif defined(__native_client__)
	#define PLATFORM_NATIVE_CLIENT
#else
	#pragma warning Unknown platform
#endif

//---------------------------------------------------------------------//
// Compiler detection
//---------------------------------------------------------------------//

#ifdef __clang__
	#define COMPILER_CLANG
	#define COMPILER_SUPPORTS_CXX11
#elif defined(_MSC_VER)
	#if _MSC_VER == 1700
		#define COMPILER_MSVC_2012 _MSC_VER
		#define COMPILER_MSVC	COMPILER_MSVC_2012
	#elif _MSC_VER == 1600
		#define COMPILER_MSVC_2010 _MSC_VER
		#define COMPILER_MSVC	COMPILER_MSVC_2010
	#elif _MSC_VER == 1500
		#define COMPILER_MSVC_2008 _MSC_VER
		#define COMPILER_MSVC	COMPILER_MSVC_2008
	#else
		#define COMPILER_MSVC _MSC_VER
		#pragma warning "Unsupported Visual C++ compiler version"
	#endif
	#ifdef _NATIVE_NULLPTR_SUPPORTED
		#define COMPILER_SUPPORTS_CXX11
	#endif
#elif defined(__GNUG__)
	#define COMPILER_GCC
	#if (__GNUG__ >= 4) && (__GNUC_MINOR__ > 5)
		#define COMPILER_SUPPORTS_CXX11
	#endif
#else
	#pragma warning Unknown compiler
#endif

//---------------------------------------------------------------------//
// Build mode
//---------------------------------------------------------------------//

#if defined( DEBUG ) || defined( _DEBUG ) || defined( ___DEBUG )
#ifndef BUILD_DEBUG
	#define BUILD_DEBUG
#endif
#else
#ifndef BUILD_RELEASE
	#define BUILD_RELEASE
#endif
#endif

//---------------------------------------------------------------------//
// Compiler-specific functionality
//---------------------------------------------------------------------//

#define MULTI_LINE_MACRO_BEGIN do {

#if defined(COMPILER_MSVC)
	#if COMPILER_MSVC >= COMPILER_MSVC_2012
		#define _ALLOW_KEYWORD_MACROS
	#endif
	#define alignof _alignof
	#define thread_local __declspec(thread)
	#define ALIGN_BEGIN(size) __declspec(align(size))
	#define ALIGN_END(size)
	#define INLINE __forceinline
	#define NO_VTABLE __declspec(novtable)
	#define OVERRIDE override
	#pragma warning(disable: 4481) // override warning
	#define MULTI_LINE_MACRO_END \
		__pragma(warning(push)) \
		__pragma(warning(disable:4127)) \
		} while(0) \
		__pragma(warning(pop))
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
	#define alignof __alignof__
	#define offsetof(type, member)  __builtin_offsetof (type, member)
	#define ALIGN_BEGIN(size)
	#define ALIGN_END(size) __attribute__((aligned(size)))
	#define INLINE __attribute__((always_inline))
	#define NO_VTABLE
	#define OVERRIDE //__attribute__((override))
	#define MULTI_LINE_MACRO_END } while(0)
	#define thread_local __thread
#endif

#if defined(COMPILER_CLANG)
	// This is defined so MSVC headers work properly.
	#define _ALLOW_KEYWORD_MACROS
#endif

#if defined(PLATFORM_WINDOWS) && defined(COMPILER_MSVC)
	#define API_EXPORT __declspec(dllexport)
	#define API_IMPORT __declspec(dllimport)
	#define API_HIDDEN
#elif defined(PLATFORM_WINDOWS)
	#define API_EXPORT __attribute__ ((dllimport))
	#define API_IMPORT __attribute__ ((dllexport))
	#define API_HIDDEN
#else
	#define API_EXPORT __attribute__ ((visibility("default")))
	#define API_IMPORT __attribute__ ((visibility("default")))
	#define API_HIDDEN __attribute__ ((visibility("hidden")))
#endif

//---------------------------------------------------------------------//
// Basic types
//---------------------------------------------------------------------//

typedef char            int8;
typedef signed char     sint8;
typedef unsigned char   uint8;

typedef short           int16;
typedef signed short    sint16;
typedef unsigned short  uint16;

typedef long            int32;
typedef signed long     sint32;
typedef unsigned long   uint32;

// Transition types.
typedef uint8 byte;
typedef uint32 uint;

#if defined(COMPILER_MSVC)
	typedef __int64 int64;
	typedef signed __int64 sint64;
	typedef unsigned __int64 uint64;
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
	typedef long long int64;
	typedef signed long long sint64;
	typedef unsigned long long uint64;
#endif

#if !defined(SWIG) && !defined(COMPILER_CLANG)
static_assert(sizeof(int8)  == 1, "");
static_assert(sizeof(int16) == 2, "");
static_assert(sizeof(int32) == 4, "");
static_assert(sizeof(int64) == 8, "");
#endif

#ifndef SWIG
#if defined(COMPILER_MSVC)
	#define enum_class_begin(name) enum name {
	#define enum_class_end };
	#define enum_class(name) name
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
	#define enum_class_begin(name) enum class name {
	#define enum_class_end };
	#define enum_class(name) name
#endif
#else
	#define enum_class_begin(name) enum name {
	#define enum_class_end };
	#define enum_class(name) name
#endif

#if !defined(COMPILER_SUPPORTS_CXX11)
	#define nullptr NULL
#endif

//---------------------------------------------------------------------//
// Helper macros
//---------------------------------------------------------------------//

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define __FILE__LINE__ __FILE__ "(" TOSTRING(__LINE__) ") : "
#define TODO( x )  message( __FILE__LINE__" TODO :   " #x ) 

//#pragma TODO("Switch to safer sizeof macro")

//((sizeof(a) / sizeof(*(a)))
//static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define GetBitFlag(bf, i) ((bf & i) ? true : false)
#define SetBitFlag(bf, i, state) (bf = (state) ? (bf | i) : (bf & ~i))

//---------------------------------------------------------------------//
// Types and Data structures
//---------------------------------------------------------------------//

#include <cassert>

//---------------------------------------------------------------------//
// Forward-declaration Helpers
//---------------------------------------------------------------------//

#define FWD_DECL(T) NAMESPACE_CORE_BEGIN class T; NAMESPACE_CORE_END

//---------------------------------------------------------------------//
// Acessors
//---------------------------------------------------------------------//

#define GETTER(name, type, var) \
	type get##name() const { return var; }

#define SETTER(name, type, var) \
	void set##name(type v) { var = v; }

#define ACCESSOR(name, type, var) \
	GETTER(name, type, var) \
	SETTER(name, type, var)

#define DECLARE_UNCOPYABLE(Type) \
	private: \
		Type( const Type& ); \
		const Type& operator=( const Type& ); \
	public:
