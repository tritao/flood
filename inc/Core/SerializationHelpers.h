/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"

#include "Core/Serialization.h"
#include "Core/Reflection.h"
#include "Core/Stream.h"

#ifdef ENABLE_SERIALIZATION

typedef struct json_t json_t;

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct Field;

struct API_CORE SerializerBinary : public Serializer
{
	MemoryStream* ms;
};

//-----------------------------------//

struct API_CORE SerializerJSON : public Serializer
{
	// Root JSON value.
	json_t* rootValue;
	
	// Stack of JSON values.
	std::vector<json_t*> values;
	std::vector<json_t*> arrays;
};

ValueContext ConvertValueToPrimitive( PrimitiveTypeKind kind, json_t* value );

//-----------------------------------//

// Deserializes a field from a stream.
API_CORE void DeserializeFields( ReflectionContext* context );

template<typename T>
static void PointerSetObject( const Field* field, void* address, T* object )
{
	if( FieldIsRawPointer(field) )
	{
		T** raw = (T**) address;
		*raw = object;
	}
	else if( FieldIsRefPointer(field) )
	{
		T* ref = (T*) object;
		RefPtr<T>* ref_obj = (RefPtr<T>*) address;
		ref_obj->reset(ref);
	}
	else if( FieldIsHandle(field) )
	{
		assert(0 && "Not implemented");
	}
	else
	{
		assert(0 && "Not implemented");
		//memcpy(element, object, size);
	}
}

//-----------------------------------//

/**
 * Wire format is inspired on Google Protocol Buffers.
 * http://code.google.com/apis/protocolbuffers/docs/encoding.html
 */

#define StreamIndex(s) (s->buffer+s->position)

#ifdef BUILD_DEBUG
void StreamAdvanceIndex(MemoryStream* ms, uint64 n);
#else
#define StreamAdvanceIndex(s, n) (s->position += n)
#endif

void EncodeVariableIntegerBuffer(uint8* buf, uint64& advance, uint64 val);
void API_CORE EncodeVariableInteger(MemoryStream* ms, uint64 val);
bool DecodeVariableIntegerBuffer(uint8* buf, uint64& advance, uint64& val);
bool DecodeVariableInteger(MemoryStream* ms, uint64& val);
uint32 EncodeZigZag32(sint32 n);
sint32 DecodeZigZag32(uint32 n);
uint64 EncodeZigZag64(sint64 n);
sint64 DecodeZigZag64(uint64 n);
void EncodeFixed32(MemoryStream* ms, uint32 val);
uint32 DecodeFixed32(MemoryStream* ms);
void EncodeFloat(MemoryStream* ms, float val);
float DecodeFloat(MemoryStream* ms);
void EncodeString(MemoryStream* ms, const String& s);
bool DecodeString(MemoryStream* ms, String& s);

// Array helpers.
void* ReflectionArrayResize( ReflectionContext*, void* address, uint32 size );
uint16 ReflectionArrayGetElementSize(const Field* field);

//-----------------------------------//

NAMESPACE_CORE_END

#endif