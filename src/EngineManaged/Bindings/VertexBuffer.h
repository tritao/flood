/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/VertexBuffer.h>
#include "Buffer.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct VertexAttribute : unsigned char;
    enum struct VertexDataType : unsigned char;
    ref class Buffer;
    ref class RefPtr;
    ref class VertexBuffer;
    ref class VertexDeclaration;
    value struct VertexElement;
    value struct VertexElementP;

    /// <summary>
    /// Attribute of a vertex element.
    /// </summary>
    public enum struct VertexAttribute : unsigned char
    {
        Position = 0,
        Normal = 2,
        Color = 3,
        BoneIndex = 4,
        FogCoord = 5,
        TexCoord0 = 6,
        TexCoord1 = 7,
        TexCoord2 = 8,
        TexCoord3 = 9,
        TexCoord4 = 10,
        TexCoord5 = 11,
        TexCoord6 = 12,
        TexCoord7 = 13
    };

    public enum struct VertexDataType : unsigned char
    {
        Byte = 0,
        Float = 1,
        Integer = 2
    };

    /// <summary>
    /// Each element inside a vertex declaration.
    /// </summary>
    public value struct VertexElementP
    {
    public:
        VertexElementP(::VertexElementP* native);
        VertexElementP(System::IntPtr native);
        Flood::VertexAttribute Attribute;
        Flood::VertexDataType Type;
        unsigned char Components;
    };

    public value struct VertexElement
    {
    public:
        VertexElement(::VertexElement* native);
        VertexElement(System::IntPtr native);
        VertexElement(Flood::VertexAttribute _0, Flood::VertexDataType _1, unsigned char components);
        char Stride;
        unsigned int Offset;
        unsigned int Size;
        Flood::VertexAttribute Attribute;
        Flood::VertexDataType Type;
        unsigned char Components;
        unsigned char GetSize();
    };

    /// <summary>
    /// This describes structure of a geometry buffer.
    /// </summary>
    public ref class VertexDeclaration
    {
    public:
        property ::VertexDeclaration* NativePtr;

        VertexDeclaration(::VertexDeclaration* native);
        VertexDeclaration(System::IntPtr native);
        void Add(Flood::VertexAttribute _0, int numComponents);
        void Add(Flood::VertexElement _0);
        void Add(Flood::VertexElementP _0);
        void Reset();
        Flood::VertexElement Find(Flood::VertexAttribute _0);
        unsigned char GetOffset(Flood::VertexAttribute _0);
        unsigned char GetVertexSize();
        void CalculateStrides();
    };

    public ref class VertexBuffer : Flood::Buffer
    {
    public:
        VertexBuffer(::VertexBuffer* native);
        VertexBuffer(System::IntPtr native);
        VertexBuffer();
        property bool Built
        {
            bool get();
            void set(bool);
        }
        bool IsBuilt();
        void ForceRebuild();
    };
}
