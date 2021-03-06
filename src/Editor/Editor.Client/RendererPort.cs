﻿using System;
using System.Collections.Generic;
using System.Drawing;
using Flood;
using Color = Flood.Color;
using Image = Flood.Image;
using StringTextureCache = System.Collections.Generic.Dictionary<
    System.Tuple<System.String, Flood.GUI.Font>, Flood.GUI.Texture>;

namespace Editor.Client
{
    class ManagedGeometryBuffer
    {
        struct BatchInfo
        {
            public RenderBatch Batch;
            public List<int> Ranges;
        };
    
        GeometryBuffer gb;
        Dictionary<uint, BatchInfo> batches;
        Dictionary<uint, ResourceHandle<Material>> materials;

        float zcount;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="hId">imageHandle</param>
        /// <returns>Material Handler</returns>
        ResourceHandle<Material> GetCreateMaterial(ResourceHandle<Image> imageHandle)
        {
            var hId = imageHandle.Id;
            if (!materials.ContainsKey(hId))
            {
                var mat = new Material("GwenGui");
                mat.SetBackfaceCulling(false);
                mat.SetBlending(BlendSource.SourceAlpha, BlendDestination.InverseSourceAlpha);
            
                if (hId==0)
                {
                    mat.SetShader("VertexColor");
                } 
                else 
                {
                    mat.SetShader("TexColor");
                    mat.SetTexture(0, imageHandle);
                    mat.GetTextureUnit(0).SetWrapMode(TextureWrapMode.Clamp);
                }

                materials.Add(hId, ResourceHandle<Material>.Create(mat));
            }
            return materials[hId];
        }

        BatchInfo GetCreateBatchInfo(ResourceHandle<Image> imageHandle)
        {
            var hId = imageHandle.Id;
            if (!batches.ContainsKey(hId))
            {
                var batch = new RenderBatch();
                batch.SetGeometryBuffer(gb);
                batch.SetRenderLayer(RenderLayer.Overlays);
                batch.SetPrimitiveType(PrimitiveType.Quads);

                var mat = GetCreateMaterial(imageHandle);
                batch.SetMaterial(mat);

                var bInfo = new BatchInfo {Batch = batch};
                batches[hId] = bInfo;
            }

            return batches[hId];
        }

        struct Vertex
        {
            public Vector3 position;
            public Vector2 uv;
            public Color color;
        };

        public ManagedGeometryBuffer()
        {
            gb = new GeometryBuffer();

            gb.Declarations.Add(new VertexElement(VertexAttribute.Position, VertexDataType.Float, 3));
            gb.Declarations.Add(new VertexElement(VertexAttribute.TexCoord0, VertexDataType.Float, 2));
            gb.Declarations.Add(new VertexElement(VertexAttribute.Color, VertexDataType.Float, 4));
            gb.Declarations.CalculateStrides();

            zcount = -100;
        }

        ~ManagedGeometryBuffer()
        {
            //Deallocate(gb);
        }

        public void AddRectangle(System.Drawing.Rectangle rect, System.Drawing.Color color)
        {
            //AddRectangle(rect,Vector2.Zero,Vector2.Zero, 0, color);
        }

        public void AddRectangle(Rectangle rect, Vector2 uv1, Vector2 uv2,
            ResourceHandle<Image> imageHandle, System.Drawing.Color color)
        {
            BatchInfo batchInfo = GetCreateBatchInfo(imageHandle); 
            batchInfo.Ranges.Add((int)gb.GetNumVertices());

            int top = Math.Max(rect.Bottom,rect.Top);
            int bottom = Math.Max(rect.Bottom,rect.Top);
            int left = rect.Left;
            int right = rect.Right;

            Vertex v1,v2,v3,v4;

            v1.position = new Vector3(left, bottom,zcount);
            v2.position = new Vector3(right, bottom,zcount);
            v3.position = new Vector3(right, top,zcount);
            v4.position = new Vector3(left, top,zcount);

            //TODO optimize precision/usage
            zcount += 0.001f;
        
            var c = new Color(color.R/255.0f,color.G/255.0f,color.B/255.0f,color.A/255.0f);

            v1.color = c;
            v2.color = c;
            v3.color = c;
            v4.color = c;

            //v1.uv = uv1;
            //v2.uv = new Vector2(uv2.x, uv1.y);
            //v3.uv = uv2;
            //v4.uv = new Vector2(uv1.x, uv2.y);
        
            // Vertex buffer setup
            //gb.Add((uint8*)&v1,sizeof(Vertex));
            //gb.Add((uint8*)&v2,sizeof(Vertex));
            //gb.Add((uint8*)&v3,sizeof(Vertex));
            //gb.Add((uint8*)&v4,sizeof(Vertex));
        }

    
        //void Render(RenderBlock rb)
        //{
        //    foreach(BatchInfo batch in batches.Values)
        //    {
        //        if (batch.Ranges.Count > 0)
        //        {
        //            batch.Batch.Range.Start = (ushort) gb.GetNumIndices();

        //            for (var ir = 0; ir < batch.Ranges.Count; ++ir)
        //            {
        //                var vertexIndex = (ushort)batch.Ranges[ir];
        //                gb.AddIndex(vertexIndex++);
        //                gb.AddIndex(vertexIndex++);
        //                gb.AddIndex(vertexIndex++);
        //                gb.AddIndex(vertexIndex);
        //            }
        //            batch.Batch.Range.End = (ushort)gb.GetNumIndices();

        //            var state = new RenderState(batch.Batch);
        //            rb.Renderables.push_back(state);
        //        }
        //    }
        //}

        void Clear()
        {
            foreach(var batch in batches.Values)
            {
                batch.Ranges.Clear();
            }

            gb.Clear();
            //gb.ClearIndexes();

            zcount = -100;
        }
    };

#if LALA
    public class TextureUtil
    {
        public static void LoadTextureInternal(Flood.GUI.Texture t, System.Drawing.Bitmap bmp)
        {
            System.Drawing.Imaging.BitmapData data = bmp.LockBits(System.Drawing.Rectangle(0, 0, bmp.Width, bmp.Height), System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            int bytes = std.abs(data.Stride) * bmp.Height;
            LoadTextureInternal(t,(uint8*)data.Scan0.ToPointer(), bytes);
            bmp.UnlockBits(data);
        }

        public static void LoadTextureInternal(Flood.GUI.Texture t, uint8* data, int size) 
        {
            ImageHandle iHandle = ImageCreate(AllocatorGetHeap(),t.Width,t.Height, PixelFormat.B8G8R8A8);
        
            //array to vector
            std.vector<byte> buffer(size);
            std.copy(data, data+size, buffer.begin());

            iHandle.Resolve().setBuffer(buffer);
            t.RendererData = iHandle.getId();
            iHandle.addReference(); //hackzito
        }
    };

    public class TextRenderer
    {

        static System.Drawing.StringFormat m_StringFormat;
        static System.Drawing.Graphics m_Graphics; // only used for text measurement
        static StringTextureCache m_StringCache = new StringTextureCache();

        static TextRenderer(){
            m_StringFormat = new System.Drawing.StringFormat(System.Drawing.StringFormat.GenericTypographic);
            m_StringFormat.FormatFlags = m_StringFormat.FormatFlags | System.Drawing.StringFormatFlags.MeasureTrailingSpaces;
            m_Graphics = System.Drawing.Graphics.FromImage(new System.Drawing.Bitmap(1024, 1024, System.Drawing.Imaging.PixelFormat.Format32bppArgb));
        }

        static bool LoadFont(Flood.GUI.Font font)
        {
            //Debug.Print(String.Format("LoadFont {0}", font.FaceName));
            font.RealSize = font.Size;// * Scale;
            System.Drawing.Font sysFont = (System.Drawing.Font)font.RendererData;

            if (sysFont != null)
                delete(sysFont);

            // apaprently this can't fail @_@
            // "If you attempt to use a font that is not supported, or the font is not installed on the machine that is running the application, the Microsoft Sans Serif font will be substituted."
            sysFont = new System.Drawing.Font(font.FaceName, font.Size);
            font.RendererData = sysFont; 
            return true;
       }

       static void FreeFont(Flood.GUI.Font font)
       {
            //Debug.Print(String.Format("FreeFont {0}", font.FaceName));
            if (font.RendererData == null)
                return;

            //Debug.Print(String.Format("FreeFont {0} - actual free", font.FaceName));
            System.Drawing.Font sysFont = (System.Drawing.Font)font.RendererData;
            if (sysFont == null)
            //    throw new System.InvalidOperationException("Freeing empty font");
                return;

            //sysFont.Dispose();
            font.RendererData = null;
        }

        static  System.Drawing.Font ConvertFont(Flood.GUI.Font font)
        {
            System.Drawing.Font sysFont = (System.Drawing.Font)font.RendererData;
            if (sysFont == null || Math.Abs(font.RealSize - font.Size /** Scale*/) > 2)
            {
                FreeFont(font);
                LoadFont(font);
                sysFont = (System.Drawing.Font)font.RendererData;
            }
            return sysFont;
        }

        public static System.Drawing.Point MeasureText(System.Drawing.Font font, System.String text, System.Drawing.StringFormat stringFormat)
        {
            System.Drawing.SizeF size = m_Graphics.MeasureString(text, font, System.Drawing.Point.Empty, stringFormat);
            return System.Drawing.Point((int)floor(size.Width+0.5), (int)floor(size.Height+0.5));
        }

        static Flood.GUI.Texture GetTexture(Flood.GUI.Font font, System.String text){
            System.Tuple<System.String, Flood.GUI.Font> key = new System.Tuple<System.String, Flood.GUI.Font>(text, font);
            if (m_StringCache.ContainsKey(key))
                return m_StringCache[key];
            return null;
        }

        static void AddTexture(Flood.GUI.Font font, System.String text, Flood.GUI.Texture texture){
            System.Tuple<System.String, Flood.GUI.Font> key = new System.Tuple<System.String, Flood.GUI.Font>(text, font);
            m_StringCache.Add(key,texture);
        }

        public static void ClearCache(){
            foreach(Flood.GUI.Texture tex in m_StringCache.Values){
                delete(tex);
            }
            m_StringCache.Clear();
        }

        //TODO use scale, remove renderer
        public static Flood.GUI.Texture StringToTexture(System.String text, Flood.GUI.Font font, Flood.GUI.Renderers.Renderer renderer)
        {
            System.Drawing.Brush brush = System.Drawing.Brushes.White;
            Flood.GUI.Texture texture = GetTexture(font,text);
            if(texture != null){ //TODO Check stringFormat
                return texture;
            }

            System.Drawing.Font sysFont = ConvertFont(font);

            System.Drawing.Point size = TextRenderer.MeasureText(sysFont, text, m_StringFormat);
            texture = new Flood.GUI.Texture(renderer);
            texture.Width = size.X;
            texture.Height = size.Y;

            System.Drawing.Bitmap bmp = new System.Drawing.Bitmap(size.X, size.Y, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            System.Drawing.Graphics gfx = System.Drawing.Graphics.FromImage(bmp);

            // NOTE:    TextRenderingHint.AntiAliasGridFit looks sharper and in most cases better
            //          but it comes with a some problems.
            //
            //          1.  Graphic.MeasureString and format.MeasureCharacterRanges 
            //              seem to return wrong values because of this.
            //
            //          2.  While typing the kerning changes in random places in the sentence.
            // 
            //          Until 1st problem is fixed we should use TextRenderingHint.AntiAlias...  :-(

            gfx.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
            gfx.Clear(System.Drawing.Color.Transparent);

            gfx.DrawString(text, sysFont, brush, System.Drawing.Point.Empty, m_StringFormat); // render text on the bitmap
            TextureUtil.LoadTextureInternal(texture,bmp);
            AddTexture(font,text,texture);
            return texture;
        }

        public static System.Drawing.Point MeasureText(System.String text, Flood.GUI.Font font)
        {
            System.Drawing.Font sysFont = ConvertFont(font);
            System.Drawing.SizeF size = m_Graphics.MeasureString(text, sysFont, System.Drawing.Point.Empty, m_StringFormat);
            return System.Drawing.Point((int)floor(size.Width+0.5), (int)floor(size.Height+0.5));
        }

    };
    public class GwenRenderer : Flood.GUI.Renderers.Renderer
    {
        ManagedGeometryBuffer buffer;

        System.Drawing.Color m_Color;
        System.Collections.Generic.Dictionary<System.Tuple<System.String, Flood.GUI.Font>, TextRenderer> m_StringCache;
    
        bool m_ClipEnabled;

        public GwenRenderer()
        {
        
            buffer = new ManagedGeometryBuffer();
        }

        void Render(RenderBlock rb){
            buffer.Render(rb);
        }

        void Clear(){
            buffer.Clear();
        }


        public override void DrawFilledRect(System.Drawing.Rectangle rect) 
        {
            rect = Translate(rect);

            buffer.AddRectangle(rect, m_Color);
        }

        public override System.Drawing.Color DrawColor 
        {
            get { return m_Color; }
            set { m_Color = value;}
        }


        public override void StartClip()
        {
            m_ClipEnabled = true;
        }

        public override void EndClip()
        {
            m_ClipEnabled = false;
        }

        public override void DrawTexturedRect(Flood.GUI.Texture t, System.Drawing.Rectangle rect, float u1, float v1, float u2, float v2)
        {
            if(t.RendererData == null){
                DrawFilledRect(rect);
            }

            var hId = (uint)t.RendererData;

            rect = Translate(rect);

            if (m_ClipEnabled)
            {
                // cpu scissors test
                if (rect.Y < ClipRegion.Y)
                {
                    int oldHeight = rect.Height;
                    int delta = ClipRegion.Y - rect.Y;
                    rect.Y = ClipRegion.Y;
                    rect.Height -= delta;

                    if (rect.Height <= 0)
                    {
                        return;
                    }

                    float dv = (float)delta / (float)oldHeight;

                    v1 += dv * (v2 - v1);
                }

                if ((rect.Y + rect.Height) > (ClipRegion.Y + ClipRegion.Height))
                {
                    int oldHeight = rect.Height;
                    int delta = (rect.Y + rect.Height) - (ClipRegion.Y + ClipRegion.Height);

                    rect.Height -= delta;

                    if (rect.Height <= 0)
                    {
                        return;
                    }

                    float dv = (float)delta / (float)oldHeight;

                    v2 -= dv * (v2 - v1);
                }

                if (rect.X < ClipRegion.X)
                {
                    int oldWidth = rect.Width;
                    int delta = ClipRegion.X - rect.X;
                    rect.X = ClipRegion.X;
                    rect.Width -= delta;

                    if (rect.Width <= 0)
                    {
                        return;
                    }

                    float du = (float)delta / (float)oldWidth;

                    u1 += du * (u2 - u1);
                }

                if ((rect.X + rect.Width) > (ClipRegion.X + ClipRegion.Width))
                {
                    int oldWidth = rect.Width;
                    int delta = (rect.X + rect.Width) - (ClipRegion.X + ClipRegion.Width);

                    rect.Width -= delta;

                    if (rect.Width <= 0)
                    {
                        return;
                    }

                    float du = (float)delta / (float)oldWidth;

                    u2 -= du * (u2 - u1);
                }
            }

            buffer.AddRectangle(rect,new Vector2(u1,v1),new Vector2(u2,v2), hId, m_Color);
        }

        public override System.Drawing.Point MeasureText(Flood.GUI.Font font, System.String text) 
        {
            return TextRenderer.MeasureText(text,font);
        }

        public override void RenderText(Flood.GUI.Font font, System.Drawing.Point position, System.String text)
        {
           Flood.GUI.Texture texture = TextRenderer.StringToTexture(text, font, this); // renders string on the texture
           var rect = new System.Drawing.Rectangle(position.X, position.Y, texture.Width, texture.Height);
           DrawTexturedRect(texture, rect,0,0,1,1);
        }

        public override void LoadTexture(Flood.GUI.Texture t)
        {
            var options = new ResourceLoadOptions(); 
            options.Name = t.Name;
            options.AsynchronousLoad = false;
            uint iHandle = FloodEngine.GetEngine().GetResourceManager().LoadResource(options);
        
            if(iHandle == HandleInvalid){
                t.RendererData = null;
                return;
            }

            Image img = iHandle.Resolve();
            t.Width =	(int)img.GetWidth();
            t.Height = (int)img.GetHeight();
            t.RendererData = iHandle;
        }

        public override void LoadTextureBitmap(Flood.GUI.Texture t, System.Drawing.Bitmap bitmap)
        {
            TextureUtil.LoadTextureInternal(t,bitmap);
        }

        public override void FreeTexture(Flood.GUI.Texture t)
        {
            if (t.RendererData == null)
                return;

            ImageHandle iHandle;
            HandleId hId = (HandleId)t.RendererData;
            iHandle.setId(hId);

            Image* img = iHandle.Resolve();

           FloodEngine.GetEngine().GetResourceManager().RemoveResource(img);
        }

        public override System.Drawing.Color PixelColor(Flood.GUI.Texture texture, System.UInt32 x, System.UInt32 y, System.Drawing.Color defaultColor)
        {
            if(texture.RendererData == null){
                return defaultColor;
            }

            ImageHandle iHandle;
            var hId = (uint)texture.RendererData;
            iHandle.setId(hId);

            Image img = iHandle.Resolve();

            System.Drawing.Color pixel;
            long offset = 4 * (x + y * texture.Width);
            std.vector<uint8>& data = img.getBuffer();

            pixel = System.Drawing.Color.FromArgb(data[offset + 3], data[offset + 0], data[offset + 1], data[offset + 2]);
        
            // Retrieving the entire texture for a single pixel read
            // is kind of a waste - maybe cache this pointer in the texture
            // data and then release later on? It's never called during runtime
            // - only during initialization.
            return pixel;
        }

    };

    class GwenInput
    {
        InputManager inputManager;

        Flood.GUI.Controls.Canvas m_Canvas;

        int m_MouseX;
        int m_MouseY;

        bool m_AltGr;

        public GwenInput(InputManager inputManager)
        {
            this.inputManager = inputManager;
            
            m_Canvas = null;
            m_MouseX = 0;
            m_MouseY = 0;
            m_AltGr = false;

            inputManager.getMouse().onMouseMove.Connect(this, GwenInput.ProcessMouseMove);
            inputManager.getMouse().onMouseDrag.Connect(this, GwenInput.ProcessMouseDrag);
            inputManager.getMouse().onMouseButtonPress.Connect(this, GwenInput.ProcessMouseButtonPressed);
            inputManager.getMouse().onMouseButtonRelease.Connect(this, GwenInput.ProcessMouseButtonReleased);
            inputManager.getMouse().onMouseWheelMove.Connect(this, GwenInput.ProcessMouseWheel);

            inputManager.getKeyboard().onKeyPress.Connect(this, GwenInput.ProcessKeyDown);
            inputManager.getKeyboard().onKeyRelease.Connect(this, GwenInput.ProcessKeyUp);
        }

        public void Initialize(Flood.GUI.Controls.Canvas c)
        {
            m_Canvas = c;
        }

        Flood.GUI.Key TranslateKeyCode(Keys key)
        {
            switch (key)
            {
            case Keys.Return: return Flood.GUI.Key.Return;
            case Keys.Escape: return Flood.GUI.Key.Escape;
            case Keys.Tab: return Flood.GUI.Key.Tab;
            case Keys.Space: return Flood.GUI.Key.Space;
            case Keys.Up: return Flood.GUI.Key.Up;
            case Keys.Down: return Flood.GUI.Key.Down;
            case Keys.Left: return Flood.GUI.Key.Left;
            case Keys.Right: return Flood.GUI.Key.Right;
            case Keys.Home: return Flood.GUI.Key.Home;
            case Keys.End: return Flood.GUI.Key.End;
            case Keys.Delete: return Flood.GUI.Key.Delete;
            case Keys.LControl:
                this.m_AltGr = true;
                return Flood.GUI.Key.Control;
            case Keys.LAlt: return Flood.GUI.Key.Alt;
            case Keys.LShift: return Flood.GUI.Key.Shift;
            case Keys.RControl: return Flood.GUI.Key.Control;
            case Keys.RAlt: 
                if (this.m_AltGr)
                {
                    this.m_Canvas.Input_Key(Flood.GUI.Key.Control, false);
                }
                return Flood.GUI.Key.Alt;
            case Keys.RShift: return Flood.GUI.Key.Shift;
                
            }
            return Flood.GUI.Key.Invalid;
        }

        static char TranslateChar(Keys key)
        {
            if (key >= Keys.A && key <= Keys.Z)
                return (char)('a' + ((int)key - (int) Keys.A));
            return ' ';
        }

        void ProcessMouseMove(MouseMoveEvent mouseMoveEvent)
        {
            int dx = mouseMoveEvent.x - m_MouseX;
            int dy = mouseMoveEvent.y - m_MouseY;

            m_MouseX = mouseMoveEvent.x;
            m_MouseY = mouseMoveEvent.y;

            m_Canvas.Input_MouseMoved(m_MouseX, m_MouseY, dx, dy);
        }

        void ProcessMouseDrag(MouseDragEvent mouseDragEvent)
        {
            int dx = mouseDragEvent.x - m_MouseX;
            int dy = mouseDragEvent.y - m_MouseY;

            m_MouseX = mouseDragEvent.x;
            m_MouseY = mouseDragEvent.y;

            m_Canvas.Input_MouseMoved(m_MouseX, m_MouseY, dx, dy);
        }

        void ProcessMouseButtonPressed(MouseButtonEvent mouseButtonEvent)
        {
            m_Canvas.Input_MouseButton((int) mouseButtonEvent.button, true);
        }

        void ProcessMouseButtonReleased(MouseButtonEvent mouseButtonEvent)
        {
            m_Canvas.Input_MouseButton((int) mouseButtonEvent.button, false);
        }

        void ProcessMouseWheel(MouseWheelEvent mouseWheelEvent)
        {
            m_Canvas.Input_MouseWheel(mouseWheelEvent.delta*60);
        }

        void ProcessKeyDown(KeyEvent keyEvent)
        {
            var ch = TranslateChar(keyEvent.keyCode);

            if (Flood.GUI.Input.InputHandler.DoSpecialKeys(m_Canvas, ch))
                return;
        
            if (ch != ' ')
            {
                m_Canvas.Input_Character(ch);
            }
        
            Flood.GUI.Key iKey = TranslateKeyCode(keyEvent.keyCode);

            m_Canvas.Input_Key(iKey, true);
        }

        void ProcessKeyUp(KeyEvent keyEvent)
        {
            char ch = TranslateChar(keyEvent.keyCode);

            Flood.GUI.Key iKey = TranslateKeyCode(keyEvent.keyCode);

            m_Canvas.Input_Key(iKey, false);
        }
    }
#endif
}
