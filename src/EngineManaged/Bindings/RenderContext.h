/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/RenderContext.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class BufferManager;
    ref class ProgramManager;
    ref class RefPtr;
    ref class RenderBackend;
    ref class RenderBuffer;
    ref class RenderCapabilities;
    ref class RenderContext;
    ref class RenderDevice;
    ref class RenderTarget;
    ref class TextureManager;
    value struct Color;
    value struct RenderContextSettings;
    value struct Settings;

    public value struct RenderContextSettings
    {
    public:
        RenderContextSettings(::RenderContextSettings* native);
        RenderContextSettings(System::IntPtr native);
        unsigned short BitsPerPixel;
        unsigned short DepthBits;
        unsigned short StencilBits;
        unsigned short AntialiasLevel;
    };

    /// <summary>
    /// Represents a rendering context.
    /// </summary>
    public ref class RenderContext
    {
    public:
        property ::RenderContext* NativePtr;

        RenderContext(::RenderContext* native);
        RenderContext(System::IntPtr native);
        RenderContext();
        property bool InitDone
        {
            bool get();
            void set(bool);
        }
        property Flood::RenderTarget^ CurrentTarget
        {
            Flood::RenderTarget^ get();
            void set(Flood::RenderTarget^);
        }
        void MakeCurrent(Flood::RenderTarget^ target);
        void SetTarget(Flood::RenderTarget^ target);
        void Init();
        void ResetState();
        Flood::Color GetPixel(unsigned short x, unsigned short y);
    };
}
