﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Editor.Client.Extensions;
using Flood.GUI;
using Flood.GUI.Controls;
using Mono.Addins;

namespace Editor.Client.GUI.Controls
{
    public class MenuBar : Control
    {
        private string extensionPoint;

        private MenuStrip menuStrip;

        public MenuBar(Control parent, string extensionPoint) : base(parent)
        {
            base.SetSize(200, 100);

            this.extensionPoint = extensionPoint;

            menuStrip = new MenuStrip(parent);
            menuStrip.Dock = Pos.Top;

            RebuildMenubar();

            AddinManager.AddExtensionNodeHandler(extensionPoint, (s,a) => RebuildMenubar());
        }

        private void RebuildMenubar()
        {
            menuStrip.DeleteAllChildren();
            foreach (var submenu in AddinManager.GetExtensionNodes(extensionPoint).Cast<SubmenuNode>())
            {
                var menuControl = menuStrip.AddItem(submenu.Label);
                menuControl.BringToFront();
                BuildSubmenu(submenu, menuControl);
            }
        }

        private void BuildSubmenu(SubmenuNode menu, MenuItem menuControl)
        {
            foreach (var menuChild in menu.ChildNodes)
            {
                var menuItem = menuChild as MenuItemNode;
                if(menuItem != null)
                {
                    menuControl.Menu.AddItem(menuItem.Label);
                    continue;
                } 

                var submenu = menuChild as SubmenuNode;
                if (submenu != null)
                {
                    //menuControl.
                    //BuildSubmenu(submenu);
                    continue;
                } 

                if (menuChild is MenuSeparatorNode)
                {
                     menuControl.Menu.AddDivider();
                    continue;
                }

               

                throw new NotImplementedException();
            }
        }
    }
}
