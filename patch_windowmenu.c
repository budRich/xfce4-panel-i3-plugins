--- xfce4-panel/plugins/windowmenu/windowmenu.c	2022-06-16 05:57:01.885190218 +0200
+++ src/i3-windowmenu/windowmenu.c	2022-06-16 07:49:52.671927566 +0200
@@ -931,8 +931,8 @@
                                               WindowMenuPlugin *plugin)
 {
   WnckWindow    *window;
-  WnckWorkspace *workspace;
   GtkWidget     *menu;
+  gchar         *command;
 
   panel_return_val_if_fail (GTK_IS_MENU_ITEM (mi), FALSE);
   panel_return_val_if_fail (GTK_IS_MENU_SHELL (gtk_widget_get_parent (mi)), FALSE);
@@ -944,11 +944,18 @@
   window = g_object_get_qdata (G_OBJECT (mi), window_quark);
   if (event->button == 1)
     {
-      /* go to workspace and activate window */
-      workspace = wnck_window_get_workspace (window);
-      if (workspace != NULL)
-        wnck_workspace_activate (workspace, event->time - 1);
-      wnck_window_activate (window, event->time);
+      command = g_strdup_printf ("i3run -d %ld --silent --summon", wnck_window_get_xid (window));
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
@@ -968,6 +975,8 @@
       return TRUE;
     }
 
+  
+
   return FALSE;
 }
 
