--- xfce4-panel/plugins/windowmenu/windowmenu.c	2022-06-16 05:57:01.885190218 +0200
+++ src/i3-windowmenu/windowmenu.c	2022-06-16 18:45:00.949000925 +0200
@@ -131,7 +131,6 @@
                                                              WindowMenuPlugin   *plugin);
 
 
-
 /* define the plugin */
 XFCE_PANEL_DEFINE_PLUGIN_RESIDENT (WindowMenuPlugin, window_menu_plugin)
 
@@ -931,8 +930,8 @@
                                               WindowMenuPlugin *plugin)
 {
   WnckWindow    *window;
-  WnckWorkspace *workspace;
   GtkWidget     *menu;
+  gchar         *command;
 
   panel_return_val_if_fail (GTK_IS_MENU_ITEM (mi), FALSE);
   panel_return_val_if_fail (GTK_IS_MENU_SHELL (gtk_widget_get_parent (mi)), FALSE);
@@ -944,11 +943,18 @@
   window = g_object_get_qdata (G_OBJECT (mi), window_quark);
   if (event->button == 1)
     {
-      /* go to workspace and activate window */
-      workspace = wnck_window_get_workspace (window);
-      if (workspace != NULL)
-        wnck_workspace_activate (workspace, event->time - 1);
-      wnck_window_activate (window, event->time);
+      command = g_strdup_printf ("i3run -d %ld --summon", wnck_window_get_xid (window));
+
+      if (!xfce_spawn_command_line (gtk_widget_get_screen (mi),
+                                    command, FALSE,
+                                    FALSE, TRUE, NULL))
+        {
+          xfce_dialog_show_error (NULL, NULL,
+                                  "Failed to execute i3run command");
+        }
+
+        if (command)
+          g_free(command);
     }
   else if (event->button == 2)
     {
@@ -968,6 +974,8 @@
       return TRUE;
     }
 
+  
+
   return FALSE;
 }
 
@@ -1129,6 +1137,18 @@
       fake_event.button = 3;
       break;
 
+    case GDK_KEY_Tab:
+
+      g_signal_emit_by_name ( GTK_MENU (menu), "move-current", GTK_MENU_DIR_NEXT);
+
+      return TRUE;
+
+    /* ISO_LEFT_TAB is result when shift is held down */
+    case GDK_KEY_ISO_Left_Tab:
+      g_signal_emit_by_name ( GTK_MENU (menu), "move-current", GTK_MENU_DIR_PREV);
+      return TRUE;
+
+
     default:
       return FALSE;
     }
@@ -1416,6 +1436,7 @@
   g_signal_connect (G_OBJECT (menu), "deactivate",
       G_CALLBACK (window_menu_plugin_menu_deactivate), plugin);
 
+  
   xfce_panel_plugin_popup_menu (XFCE_PANEL_PLUGIN (plugin), GTK_MENU (menu),
                                 button, (GdkEvent *) event);
 }
