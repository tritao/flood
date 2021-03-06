/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "VertexBuffer.h"
#include "Buffer.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::VertexElementP::VertexElementP(::VertexElementP* native)
{
    Attribute = (Flood::VertexAttribute)native->attribute;
    Type = (Flood::VertexDataType)native->type;
    Components = native->components;
}

Flood::VertexElementP::VertexElementP(System::IntPtr native)
{
    auto __native = (::VertexElementP*)native.ToPointer();
    Attribute = (Flood::VertexAttribute)__native->attribute;
    Type = (Flood::VertexDataType)__native->type;
    Components = __native->components;
}

Flood::VertexElement::VertexElement(::VertexElement* native)
{
    Stride = native->stride;
    Offset = native->offset;
    Size = native->size;
}

Flood::VertexElement::VertexElement(System::IntPtr native)
{
    auto __native = (::VertexElement*)native.ToPointer();
    Stride = __native->stride;
    Offset = __native->offset;
    Size = __native->size;
}

Flood::VertexElement::VertexElement(Flood::VertexAttribute _0, Flood::VertexDataType _1, unsigned char components)
{
}

unsigned char Flood::VertexElement::GetSize()
{
    auto this0 = (::VertexElement*) 0;
    auto ret = this0->getSize();
    return ret;
}

Flood::VertexDeclaration::VertexDeclaration(::VertexDeclaration* native)
{
    NativePtr = native;
}

Flood::VertexDeclaration::VertexDeclaration(System::IntPtr native)
{
    auto __native = (::VertexDeclaration*)native.ToPointer();
    NativePtr = __native;
}

void Flood::VertexDeclaration::Add(Flood::VertexAttribute _0, int numComponents)
{
    auto arg0 = (::VertexAttribute)_0;
    ((::VertexDeclaration*)NativePtr)->add(arg0, numComponents);
}

void Flood::VertexDeclaration::Add(Flood::VertexElement _0)
{
    auto _marshal0 = ::VertexElement();
    _marshal0.stride = (int8)_0.Stride;
    _marshal0.offset = (uint32)_0.Offset;
    _marshal0.size = (uint32)_0.Size;

    auto arg0 = _marshal0;
    ((::VertexDeclaration*)NativePtr)->add(arg0);
}

void Flood::VertexDeclaration::Add(Flood::VertexElementP _0)
{
    auto _marshal0 = ::VertexElementP();
    _marshal0.attribute = (::VertexAttribute)_0.Attribute;
    _marshal0.type = (::VertexDataType)_0.Type;
    _marshal0.components = (uint8)_0.Components;

    auto arg0 = _marshal0;
    ((::VertexDeclaration*)NativePtr)->add(arg0);
}

void Flood::VertexDeclaration::Reset()
{
    ((::VertexDeclaration*)NativePtr)->reset();
}

Flood::VertexElement Flood::VertexDeclaration::Find(Flood::VertexAttribute _0)
{
    auto arg0 = (::VertexAttribute)_0;
    auto ret = ((::VertexDeclaration*)NativePtr)->find(arg0);
    return Flood::VertexElement((::VertexElement*)ret);
}

unsigned char Flood::VertexDeclaration::GetOffset(Flood::VertexAttribute _0)
{
    auto arg0 = (::VertexAttribute)_0;
    auto ret = ((::VertexDeclaration*)NativePtr)->getOffset(arg0);
    return ret;
}

unsigned char Flood::VertexDeclaration::GetVertexSize()
{
    auto ret = ((::VertexDeclaration*)NativePtr)->getVertexSize();
    return ret;
}

void Flood::VertexDeclaration::CalculateStrides()
{
    ((::VertexDeclaration*)NativePtr)->calculateStrides();
}

Flood::VertexBuffer::VertexBuffer(::VertexBuffer* native)
    : Flood::Buffer(native)
{
}

Flood::VertexBuffer::VertexBuffer(System::IntPtr native)
    : Flood::Buffer(native)
{
    auto __native = (::VertexBuffer*)native.ToPointer();
}

Flood::VertexBuffer::VertexBuffer()
    : Flood::Buffer(nullptr)
{
    NativePtr = new ::VertexBuffer();
}

bool Flood::VertexBuffer::IsBuilt()
{
    auto ret = ((::VertexBuffer*)NativePtr)->isBuilt();
    return ret;
}

void Flood::VertexBuffer::ForceRebuild()
{
    ((::VertexBuffer*)NativePtr)->forceRebuild();
}

bool Flood::VertexBuffer::Built::get()
{
    return ((::VertexBuffer*)NativePtr)->built;
}

void Flood::VertexBuffer::Built::set(bool value)
{
    ((::VertexBuffer*)NativePtr)->built = value;
}

