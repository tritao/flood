/************************************************************************
*
* vapor3D Server � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"
#include "Messages.h"

NAMESPACE_BEGIN

//-----------------------------------//

class Message : public ReferenceCounted
{
public:

	Message();
	Message(const std::vector<byte>& data);
	~Message();

	// Inits the message with nothing.
	void init();

	// Inits the message with data.
	void initData(const std::vector<byte>& data);

	// Gets the message type.
	GETTER(MessageType, MessageType::Enum, messageType)

	// Sets the message type.
	SETTER(MessageType, MessageType::Enum, messageType)

	// Gets the internal message.
	void* getBuffer() { return nullptr; }

protected:
	
	// Message type.
	MessageType::Enum messageType;

	// Message data.
	std::vector<byte> data;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Message)

//-----------------------------------//

NAMESPACE_END