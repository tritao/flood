/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Memory.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class Allocator;
    ref class BumpAllocator;
    ref class HeapAllocator;
    ref class PoolAllocator;

    public delegate System::IntPtr MemoryAllocateFunction(Flood::Allocator^ _0, int size, int align);

    public delegate void MemoryFreeFunction(Flood::Allocator^ _1, System::IntPtr object);

    public delegate void MemoryResetFunction(Flood::Allocator^ _2);

    /// <summary>
    /// Interface for a custom memory allocator.
    /// </summary>
    public ref class Allocator
    {
    public:
        property ::Allocator* NativePtr;

        Allocator(::Allocator* native);
        Allocator(System::IntPtr native);
        property Flood::MemoryAllocateFunction^ Allocate
        {
            Flood::MemoryAllocateFunction^ get();
            void set(Flood::MemoryAllocateFunction^);
        }
        property Flood::MemoryFreeFunction^ Deallocate
        {
            Flood::MemoryFreeFunction^ get();
            void set(Flood::MemoryFreeFunction^);
        }
        property Flood::MemoryResetFunction^ Reset
        {
            Flood::MemoryResetFunction^ get();
            void set(Flood::MemoryResetFunction^);
        }
        property System::String^ Group
        {
            System::String^ get();
            void set(System::String^);
        }
        void Destroy();
        void ResetMemory();
        void SetGroup(System::String^ group);
        Flood::Allocator^ CreatePool(int size);
        Flood::Allocator^ CreateBump(int size);
        Flood::Allocator^ CreateHeap();
        Flood::Allocator^ CreateStack();
        static Flood::Allocator^ GetHeap();
        static Flood::Allocator^ GetStack();
        static Flood::Allocator^ GetObject(System::IntPtr _0);
        static void DumpInfo();
    };

    /// <summary>
    /// Manages memory allocation using a fixed-size object pool. When a new
    /// allocation is requested, we search for the first free object available and
    /// return it.
    /// </summary>
    public ref class PoolAllocator : Flood::Allocator
    {
    public:
        PoolAllocator(::PoolAllocator* native);
        PoolAllocator(System::IntPtr native);
        property System::IntPtr Current
        {
            System::IntPtr get();
            void set(System::IntPtr);
        }
    };

    /// <summary>
    /// Manages memory allocations using a fixed-size buffer that can provide
    /// allocations and deallocations in O(1). The allocated space in the buffer is
    /// not re-used until the whole buffer is reset.
    /// </summary>
    public ref class BumpAllocator : Flood::Allocator
    {
    public:
        BumpAllocator(::BumpAllocator* native);
        BumpAllocator(System::IntPtr native);
        property System::IntPtr Start
        {
            System::IntPtr get();
            void set(System::IntPtr);
        }
        property System::IntPtr Current
        {
            System::IntPtr get();
            void set(System::IntPtr);
        }
        property unsigned int Size
        {
            unsigned int get();
            void set(unsigned int);
        }
    };

    /// <summary>
    /// Manages memory allocation using Doug Lea's malloc implementation. This is a
    /// boundary-tag allocator that manages memory by keeping track of the
    /// used/free memory blocks.
    /// </summary>
    public ref class HeapAllocator : Flood::Allocator
    {
    public:
        HeapAllocator(::HeapAllocator* native);
        HeapAllocator(System::IntPtr native);
        property System::IntPtr Space
        {
            System::IntPtr get();
            void set(System::IntPtr);
        }
    };
}
