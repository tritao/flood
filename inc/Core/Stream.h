/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Pointers.h"
#include "Core/String.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

/**
 * Use these for different kinds of access to the streams.
 */

enum struct StreamOpenMode
{
	Read,
	Write,
	Append
};

/**
 * Controls where the seeking will occur from.
 */

enum struct StreamSeekMode
{
	Absolute,
	Relative,
	RelativeEnd
};

const int StreamEOF = 0;

//-----------------------------------//

/**
 * A stream allows uniform access to data backed by different storage
 * mediums like memory, files, archives, or even in remove servers.
 */

struct Stream;

typedef bool  (*StreamOpenFunction)(Stream*);
typedef bool  (*StreamCloseFunction)(Stream*);
typedef int64 (*StreamReadFunction)(Stream*, void*, int64);
typedef int64 (*StreamWriteFunction)(Stream*, void*, int64);
typedef int64 (*StreamTellFunction)(Stream*);
typedef int64 (*StreamSeekFunction)(Stream*, int64, int8);
typedef int64 (*StreamSizeFunction)(Stream*);
typedef void  (*StreamResizeFunction)(Stream*, int64 size);

struct StreamFuncs
{
	StreamOpenFunction  open;
	StreamCloseFunction close;
	StreamReadFunction  read;
	StreamWriteFunction write;
	StreamTellFunction  tell;
	StreamSeekFunction  seek;
	StreamSizeFunction  size;
	StreamResizeFunction  resize;
};

/**
 * Streams can be created from URIs (Univeral Resource Identifiers).
 * Each stream protocol supported has a unique scheme that will be
 * used to identify it when opening the stream. Examples, http:,
 * or zip: would be valid schemes for an HTTP or ZIP stream.
 */

struct API_CORE Stream
{
	virtual ~Stream() {}

	String path;
	StreamOpenMode mode;
	StreamFuncs* fn;
};

API_CORE Stream*  StreamCreateFromURI(Allocator*, const Path&, StreamOpenMode);
#define pStreamCreateFromURI(alloc, ...) CreateScopedPtr(StreamCreateFromURI, alloc, __VA_ARGS__)

API_CORE void   StreamDestroy(Stream*);
API_CORE bool   StreamClose(Stream*);

API_CORE int64  StreamGetSize(Stream*);
API_CORE void   StreamResize(Stream*, int64 size);

API_CORE int64  StreamRead(Stream*, std::vector<uint8>& data);
API_CORE int64  StreamReadBuffer(Stream*, void* buffer, int64 size);
API_CORE int64  StreamReadString(Stream*, String&);
API_CORE int64  StreamReadLines(Stream*, std::vector<String>&);

API_CORE int64  StreamWrite(Stream*, uint8* buf, uint64 size);
API_CORE int64  StreamWriteString(Stream*, const String&);

API_CORE int64  StreamGetPosition(Stream*);
API_CORE int64  StreamSetPosition(Stream*, int64, StreamSeekMode);

typedef scoped_ptr<Stream, StreamDestroy> StreamPtr;

//-----------------------------------//

struct API_CORE FileStream : Stream
{
	FILE* fp;
};

API_CORE Stream*  StreamCreateFromFile(Allocator*, const Path&, StreamOpenMode);
#define pStreamCreateFromFile(alloc, ...) CreateScopedPtr(StreamCreateFromFile, alloc, __VA_ARGS__)

//-----------------------------------//

struct API_CORE MemoryStream : Stream
{
	std::vector<uint8> data;
	uint8* buffer;
	uint64 position;
	bool useRawBuffer;
};

API_CORE MemoryStream* StreamCreateFromMemory(Allocator*, uint64 size);
API_CORE void StreamMemoryInit(MemoryStream*);
API_CORE void StreamMemorySetRawBuffer(MemoryStream*, uint8* buffer);

//-----------------------------------//

API_CORE Stream* StreamCreateWeb(Allocator* alloc, const String& URL, StreamOpenMode mode);

//-----------------------------------//

NAMESPACE_EXTERN_END