/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Input/InputManager.h>
#include "Window.h"
#include "RenderTarget.h"
#include "Vector.h"
#include "Color.h"

namespace Flood
{
    ref class InputManager;
    ref class Window;
    ref class Event0;
    ref class Delegate0;
    ref class ClosurePtr;
    ref class GenericClass;
    ref class DelegateMemento;
    ref class Event1;
    ref class RefPtr;
    value struct WindowSettings;
    enum struct WindowStyles;
    ref class RenderContext;
    ref class RenderCapabilities;
    ref class BufferManager;
    ref class TextureManager;
    ref class ProgramManager;
    ref class RenderTarget;
    ref class RenderBackend;
    value struct Color;
    value struct Vector3;
    ref class RenderBuffer;
    value struct Settings;
    ref class RenderContextSettings;
    value struct Vector2i;
    ref class InputDevice;
    ref class Keyboard;
    ref class Mouse;
    ref class InputEvent;
    ref class Joystick;

    /// <summary>
    /// Manages a set of input devices.
    /// </summary>
    public ref class InputManager
    {
    public:
        property ::InputManager* NativePtr;

        InputManager(::InputManager* native);
        InputManager(System::IntPtr native);
        InputManager();
        property Flood::Window^ Window;
        void CreateDefaultDevices();
        Flood::Window^ GetWindow();
        void SetWindow(Flood::Window^ window);
    };

    public ref class FloodInputManager
    {
    public:
        static void InputInitialize();
        static void InputDeinitialize();
        static Flood::InputManager^ GetInputManager();
    };
}
