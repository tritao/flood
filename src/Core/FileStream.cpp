/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Log.h"

#include <cstdio>

#ifdef PLATFORM_WINDOWS
	#include <io.h>
	#define F_OK 0
#else
	#include <unistd.h>
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static bool  FileOpen(Stream*);
static bool  FileClose(Stream*);
static int64 FileRead(Stream*, void*, int64);
static int64 FileWrite(Stream*, void*, int64);
static int64 FileTell(Stream*);
static int64 FileSeek(Stream*, int64, int8);
static int64 FileGetSize(Stream*);

static StreamFuncs gs_FileFuncs = 
{
	FileOpen,
	FileClose,
	FileRead,
	FileWrite,
	FileTell,
	FileSeek,
	FileGetSize,
	0/*FileResize*/
};

//-----------------------------------//

Stream* StreamCreateFromFile(Allocator* alloc,
							 const Path& path, StreamOpenMode mode)
{
	FileStream* fs = Allocate(alloc, FileStream);
	if( !fs ) return nullptr;
	
	fs->fp = nullptr;
	fs->path = path;
	fs->mode = mode;
	fs->fn = &gs_FileFuncs;

	if( !FileOpen(fs) )
	{
		//LogWarn("Error opening file: %s", PathGetFile(path).c_str());
		Deallocate(fs);
		return nullptr;
	}

	return fs;
}

//-----------------------------------//

static bool FileOpen(Stream* stream)
{
	FileStream* fs = (FileStream*) stream;
	const char* mode = nullptr;

	switch(fs->mode)
	{
	case StreamOpenMode::Read:   mode = "rb"; break;
	case StreamOpenMode::Write:  mode = "w+b"; break;
	case StreamOpenMode::Append: mode = "a+b"; break;
	}

#ifdef COMPILER_MSVC
	fopen_s(&fs->fp, fs->path.c_str(), mode);
#else
	fs->fp = fopen(fs->path.c_str(), mode);
#endif

	return fs->fp != nullptr;
}

//-----------------------------------//

static bool FileClose(Stream* stream)
{
	FileStream* fs = (FileStream*) stream;

	if(fs->fp == nullptr)
		return true;

	int ret = fclose(fs->fp);
	fs->fp = nullptr;

	return ret == 0;
}

//-----------------------------------//

static int64 FileRead(Stream* stream, void* buffer, int64 size)
{
	FileStream* fs = (FileStream*) stream;

	if( feof(fs->fp) )
		return StreamEOF;

	return fread(buffer, 1, size_t(size), fs->fp);
}

//-----------------------------------//

static int64 FileWrite(Stream* stream, void* buffer, int64 size)
{
	FileStream* fs = (FileStream*) stream;

	assert( fs->mode == StreamOpenMode::Write || fs->mode == StreamOpenMode::Append );
	assert( buffer && size >= 0 );

	return fwrite(buffer, size_t(size), 1, fs->fp);
}

//-----------------------------------//

static int64 FileTell(Stream* stream)
{
	FileStream* fs = (FileStream*) stream;

#ifdef COMPILER_MSVC
	return _ftelli64(fs->fp);
#else
	return ftell(fs->fp);
#endif
}

//-----------------------------------//

static int64 FileSeek(Stream* stream, int64 offset, int8 mode)
{
	FileStream* fs = (FileStream*) stream;

	int origin = 0;

	switch(mode)
	{
	case StreamSeekMode::Absolute:
		origin = SEEK_SET;
		break;
	case StreamSeekMode::Relative:
		origin = SEEK_CUR;
		break;
	case StreamSeekMode::RelativeEnd:
		origin = SEEK_END;
		break;
	}

#ifdef COMPILER_MSVC
	return _fseeki64(fs->fp, offset, origin);
#else
	return fseek(fs->fp, (long) offset, origin);
#endif
}

//-----------------------------------//

static int64 FileGetSize(Stream* stream)
{
	FileStream* fs = (FileStream*) stream;

#ifdef COMPILER_MSVC
	return _filelengthi64( _fileno(fs->fp) );
#else
	// Hold the current file position.
	int64 curr = FileTell(fs);
	
	// Seek to the end of the file and get position.
	FileSeek(fs, 0, StreamSeekMode::Absolute);
	
	int64 size = FileTell(fs);
	
	// Seek again to the previously current position.
	FileSeek(fs, curr, StreamSeekMode::Absolute);

	return size;
#endif
}

//-----------------------------------//

static void FileSetBuffering( Stream* stream, bool state )
{
	FileStream* fs = (FileStream*) stream;
	FILE* fp = fs->fp;

	int mode = _IOFBF;
	
	if( !state )
		mode = _IONBF;
	
	setvbuf(fp, nullptr, mode, 0);
}

//-----------------------------------//

NAMESPACE_CORE_END