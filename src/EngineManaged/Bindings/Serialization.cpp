/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Serialization.h"
#include "Reflection.h"
#include "Memory.h"
#include "Stream.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::ValueContext::ValueContext(::ValueContext* native)
{
    NativePtr = native;
}

Flood::ValueContext::ValueContext(System::IntPtr native)
{
    auto __native = (::ValueContext*)native.ToPointer();
    NativePtr = __native;
}

Flood::ReflectionHandleContext::ReflectionHandleContext(::ReflectionHandleContext* native)
{
    NativePtr = native;
}

Flood::ReflectionHandleContext::ReflectionHandleContext(System::IntPtr native)
{
    auto __native = (::ReflectionHandleContext*)native.ToPointer();
    NativePtr = __native;
}

Flood::ReflectionContext::ReflectionContext(::ReflectionContext* native)
{
    NativePtr = native;
}

Flood::ReflectionContext::ReflectionContext(System::IntPtr native)
{
    auto __native = (::ReflectionContext*)native.ToPointer();
    NativePtr = __native;
}

Flood::ReflectionContext::ReflectionContext()
{
    NativePtr = new ::ReflectionContext();
}

Flood::Serializer::Serializer(::Serializer* native)
{
    NativePtr = native;
}

Flood::Serializer::Serializer(System::IntPtr native)
{
    auto __native = (::Serializer*)native.ToPointer();
    NativePtr = __native;
}

Flood::Serializer::Serializer()
{
    NativePtr = new ::Serializer();
}

void Flood::Serializer::Destroy()
{
    auto arg0 = (::Serializer*)NativePtr;
    ::SerializerDestroy(arg0);
}

void Flood::FloodSerialization::ReflectionWalkComposite(Flood::ReflectionContext^ _112)
{
    auto arg0 = (::ReflectionContext*)_112->NativePtr;
    ::ReflectionWalkComposite(arg0);
}

void Flood::FloodSerialization::ReflectionWalkCompositeField(Flood::ReflectionContext^ _113)
{
    auto arg0 = (::ReflectionContext*)_113->NativePtr;
    ::ReflectionWalkCompositeField(arg0);
}

