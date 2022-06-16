--- xfce4-panel/plugins/windowmenu/windowmenu.c	2022-06-16 05:57:01.885190218 +0200
+++ src/i3-windowmenu/windowmenu.c	2022-06-16 06:30:46.435211358 +0200
@@ -931,8 +931,9 @@
                                               WindowMenuPlugin *plugin)
 {
   WnckWindow    *window;
-  WnckWorkspace *workspace;
   GtkWidget     *menu;
+  gchar         *command;
+  GError        *error;
 
   panel_return_val_if_fail (GTK_IS_MENU_ITEM (mi), FALSE);
   panel_return_val_if_fail (GTK_IS_MENU_SHELL (gtk_widget_get_parent (mi)), FALSE);
@@ -944,11 +945,15 @@
   window = g_object_get_qdata (G_OBJECT (mi), window_quark);
   if (event->button == 1)
     {
-      /* go to workspace and activate window */
-      workspace = wnck_window_get_workspace (window);
-      if (workspace != NULL)
-        wnck_workspace_activate (workspace, event->time - 1);
-      wnck_window_activate (window, event->time);
+      command = g_strdup_printf ("i3run -d %ld --summon", wnck_window_get_xid (window));
+      if (!xfce_spawn_command_line (gtk_widget_get_screen (mi),
+                                    command, FALSE,
+                                    FALSE, TRUE, &error))
+        {
+          xfce_dialog_show_error (NULL, error,
+                                  _("Failed to execute i3run command"));
+          g_error_free (error);
+        }
     }
   else if (event->button == 2)
     {
