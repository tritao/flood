﻿using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Flood.GUI.Controls.Layout
{
    public class Tile : Control
    {

        Point   m_TileSize;

        public Tile ( Control parent)
            : base(parent)
        {
            Dock = Pos.Fill;
            SetTileSize( 22, 22 );
        }

        protected override void PostLayout(Skins.Skin skin)
        {
            var  bounds = InnerBounds;
            var pos = new Point( bounds.X, bounds.Y );

            foreach (var pChild in Children){
                if ( pChild.Dock != Pos.None ) continue;

                pChild.SetPosition( pos.X + (m_TileSize.X/2) - (pChild.Width/2), pos.Y + (m_TileSize.Y/2) - (pChild.Height/2) );

                pos.X = pos.X + m_TileSize.X;
                if (pos.X + m_TileSize.X > bounds.X + bounds.Width)
                {
                        pos.X = bounds.X;
                        pos.Y += m_TileSize.Y;
                }

            }
        }

        public void SetTileSize( int x, int y )
        {
            m_TileSize = new Point( x, y );
        }

    }
}
