﻿using System;
using Flood;
using Flood.GUI.Controls;
using Flood.GUI.Renderers;
using Flood.GUI.Skins;

namespace Editor.Client.Gui
{
    public abstract class GuiWindow : IDisposable
    {
        /// <summary>
        /// Native GUI window.
        /// </summary>
        public Window NativeWindow;

        /// <summary>
        /// Renderer of the GUI.
        /// </summary>
        public Renderer Renderer { get; private set; }

        /// <summary>
        /// Skin of the GUI.
        /// </summary>
        public Skin Skin  { get; private set; }

        public Canvas Canvas  { get; private set; }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposing) return;

            Canvas.Dispose();
            Skin.Dispose();
            Renderer.Dispose();
        }

        public void Init(Renderer renderer, string textureName)
        {
            Renderer = renderer;
            Skin = new TexturedSkin(renderer, textureName);
            Canvas = new Canvas(Skin);

            Init();
        }

        protected abstract void Init();

        public void Render()
        {
            Canvas.RenderCanvas();
        }
    }
}
