/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Core/Stream.h"
#include "Core/Serialization.h"

struct _ENetPacket;
typedef _ENetPacket ENetPacket;

FWD_DECL_INTRUSIVE(Session)
FWD_DECL_INTRUSIVE(Message)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

typedef uint16 MessageId;

namespace MessageFlags
{
	enum Enum : uint8
	{
		Binary		= 1 << 0,
		Encrypted	= 1 << 1,
		Compressed	= 1 << 2,
		Reliable	= 1 << 3,
	};
}

class Message : public ReferenceCounted
{
public:

	Message(MessageId id);
	~Message();

	// Writes an object to the message.
	void write(const Object* object);

	// Accesses the message flags.
	ACCESSOR(Flags, uint8, flags)

	// Accesses the message id.
	GETTER(Id, MessageId, id)

	// Gets the internal packet.
	GETTER(Packet, ENetPacket*, packet)

	// Creates a network packet.
	void createPacket();

	// Sets the internal packet.
	void setPacket(ENetPacket* packet);

	// Prepares the message for sending.
	void prepare();

	// Writes a POD type to the message.
	template<typename T> void writeRaw(const T& pod)
	{
		ms->data.resize(ms->data.size() + sizeof(T));
		memcpy(&ms->data[0] + ms->position, &pod, sizeof(T));
		ms->position += sizeof(T);
	}

	// Reads a POD type from the message.
	template<typename T> T* readRaw()
	{
		T* pod = (T*)(&ms->data[0] + ms->position);
		ms->position += sizeof(T);
		return pod;
	}

	// Message id.
	MessageId id;

	// Message flags.
	uint8 flags;

	// Message data.
	MemoryStream* ms;

	// Network packet;
	ENetPacket* packet;

	// Keeps if packet needs to be freed.
	bool freePacket;
};

// Creates a new message using the network allocator.
API_CORE MessagePtr MessageCreate(MessageId id);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Message)

//-----------------------------------//

NAMESPACE_CORE_END