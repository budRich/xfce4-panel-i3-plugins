#include "windowmenu.h"

#if (!LIBXFCE4PANEL_CHECK_VERSION(4,17,2))
void
xfce_panel_plugin_popup_menu (XfcePanelPlugin *plugin,
                              GtkMenu         *menu,
                              GtkWidget       *widget,
                              const GdkEvent  *trigger_event)
{
  GdkGravity widget_anchor, menu_anchor;
  gboolean   popup_at_widget = TRUE;

  g_return_if_fail (XFCE_IS_PANEL_PLUGIN (plugin));
  g_return_if_fail (GTK_IS_MENU (menu));

  /* check if conditions are met to pop up menu at widget */
  if (widget != NULL)
    {
      switch (xfce_panel_plugin_get_screen_position (plugin))
        {
          case XFCE_SCREEN_POSITION_NW_H:
          case XFCE_SCREEN_POSITION_N:
          case XFCE_SCREEN_POSITION_NE_H:
            widget_anchor = GDK_GRAVITY_SOUTH_WEST;
            menu_anchor = GDK_GRAVITY_NORTH_WEST;
            break;

          case XFCE_SCREEN_POSITION_NW_V:
          case XFCE_SCREEN_POSITION_W:
          case XFCE_SCREEN_POSITION_SW_V:
            widget_anchor = GDK_GRAVITY_NORTH_EAST;
            menu_anchor = GDK_GRAVITY_NORTH_WEST;
            break;

          case XFCE_SCREEN_POSITION_NE_V:
          case XFCE_SCREEN_POSITION_E:
          case XFCE_SCREEN_POSITION_SE_V:
            widget_anchor = GDK_GRAVITY_NORTH_WEST;
            menu_anchor = GDK_GRAVITY_NORTH_EAST;
            break;

          case XFCE_SCREEN_POSITION_SW_H:
          case XFCE_SCREEN_POSITION_S:
          case XFCE_SCREEN_POSITION_SE_H:
            widget_anchor = GDK_GRAVITY_NORTH_WEST;
            menu_anchor = GDK_GRAVITY_SOUTH_WEST;
            break;

          default:
            popup_at_widget = FALSE;
            break;
        }
    }
  else
    popup_at_widget = FALSE;

  /* register the menu */
  xfce_panel_plugin_register_menu (plugin, menu);

  /* pop up the menu */
  if (popup_at_widget)
    gtk_menu_popup_at_widget (menu, widget, widget_anchor, menu_anchor, trigger_event);
  else
    gtk_menu_popup_at_pointer (menu, trigger_event);
}
#endif
