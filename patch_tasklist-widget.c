--- xfce4-panel/plugins/tasklist/tasklist-widget.c	2022-06-16 05:57:01.881856885 +0200
+++ src/i3-tasklist/tasklist-widget.c	2022-06-16 12:23:45.372095600 +0200
@@ -3363,125 +3363,27 @@
 xfce_tasklist_button_activate (XfceTasklistChild *child,
                                guint32            timestamp)
 {
-  WnckWorkspace *workspace;
-  gint           window_x, window_y;
-  gint           workspace_width, workspace_height;
-  gint           screen_width, screen_height;
-  gint           viewport_x, viewport_y;
+  gchar         *command = NULL;
+  // GError        *error = NULL;
 
   panel_return_if_fail (XFCE_IS_TASKLIST (child->tasklist));
   panel_return_if_fail (WNCK_IS_WINDOW (child->window));
   panel_return_if_fail (WNCK_IS_SCREEN (child->tasklist->screen));
 
-  if (wnck_window_is_active (child->window))
+
+  /* go to workspace and activate window */
+  command = g_strdup_printf ("i3run -d %ld --silent --summon", wnck_window_get_xid (child->window));
+  if (!xfce_spawn_command_line (gtk_widget_get_screen (GTK_WIDGET (child)),
+  // if (!xfce_spawn_command_line (child->tasklist->screen,
+                                command, FALSE,
+                                FALSE, TRUE, NULL))
     {
-      /* minimize does not work when this is assigned to the
-       * middle mouse button */
-      if (child->tasklist->middle_click != XFCE_TASKLIST_MIDDLE_CLICK_MINIMIZE_WINDOW)
-        wnck_window_minimize (child->window);
+      xfce_dialog_show_error (NULL, NULL, command);
+      // g_error_free (error);
     }
-  else
-    {
-      /* we only change worksapces/viewports for non-pinned windows
-       * and if all workspaces/viewports are shown or if we have
-       * all blinking enabled and the current button is blinking */
-      if ((child->tasklist->all_workspaces
-          && !wnck_window_is_pinned (child->window))
-          || (child->tasklist->all_blinking
-              && xfce_arrow_button_get_blinking (XFCE_ARROW_BUTTON (child->button))))
-        {
-          workspace = wnck_window_get_workspace (child->window);
-
-          /* only switch workspaces/viewports if switch_workspace is enabled or
-           * we want to restore a minimized window to the current workspace/viewport */
-          if (workspace != NULL
-              && (child->tasklist->switch_workspace
-                  || !wnck_window_is_minimized (child->window)))
-            {
-              if (G_UNLIKELY (wnck_workspace_is_virtual (workspace)))
-                {
-                  if (!wnck_window_is_in_viewport (child->window, workspace))
-                    {
-                      /* viewport info */
-                      workspace_width = wnck_workspace_get_width (workspace);
-                      workspace_height = wnck_workspace_get_height (workspace);
-                      screen_width = wnck_screen_get_width (child->tasklist->screen);
-                      screen_height = wnck_screen_get_height (child->tasklist->screen);
-
-                      /* we only support multiple viewports like compiz has
-                       * (all equally spread across the screen) */
-                      if ((workspace_width % screen_width) == 0
-                          && (workspace_height % screen_height) == 0)
-                        {
-                          wnck_window_get_geometry (child->window, &window_x, &window_y, NULL, NULL);
-
-                          /* lookup nearest workspace edge */
-                          viewport_x = window_x - (window_x % screen_width);
-                          viewport_x = CLAMP (viewport_x, 0, workspace_width - screen_width);
-
-                          viewport_y = window_y - (window_y % screen_height);
-                          viewport_y = CLAMP (viewport_y, 0, workspace_height - screen_height);
-
-                          /* move to the other viewport */
-                          wnck_screen_move_viewport (child->tasklist->screen, viewport_x, viewport_y);
-                        }
-                      else
-                        {
-                          g_warning ("only viewport with equally distributed screens are supported: %dx%d & %dx%d",
-                                     workspace_width, workspace_height, screen_width, screen_height);
-                        }
-                    }
-                }
-              else if (wnck_screen_get_active_workspace (child->tasklist->screen) != workspace)
-                {
-                  /* switch to the other workspace before we activate the window */
-                  wnck_workspace_activate (workspace, timestamp);
-                  gtk_main_iteration ();
-                }
-            }
-          else if (workspace != NULL
-                   && wnck_workspace_is_virtual (workspace)
-                   && !wnck_window_is_in_viewport (child->window, workspace))
-            {
-              /* viewport info */
-              workspace_width = wnck_workspace_get_width (workspace);
-              workspace_height = wnck_workspace_get_height (workspace);
-              screen_width = wnck_screen_get_width (child->tasklist->screen);
-              screen_height = wnck_screen_get_height (child->tasklist->screen);
-
-              /* we only support multiple viewports like compiz has
-               * (all equaly spread across the screen) */
-              if ((workspace_width % screen_width) == 0
-                  && (workspace_height % screen_height) == 0)
-                {
-                  viewport_x = wnck_workspace_get_viewport_x (workspace);
-                  viewport_y = wnck_workspace_get_viewport_y (workspace);
-
-                  /* note that the x and y might be negative numbers, since they are relative
-                   * to the current screen, not to the edge of the screen they are on. this is
-                   * not a problem since the mod result will always be positive */
-                  wnck_window_get_geometry (child->window, &window_x, &window_y, NULL, NULL);
-
-                  /* get the new screen position, with the same screen offset */
-                  window_x = viewport_x + (window_x % screen_width);
-                  window_y = viewport_y + (window_y % screen_height);
-
-                  /* move the window */
-                  wnck_window_set_geometry (child->window,
-                                            WNCK_WINDOW_GRAVITY_CURRENT,
-                                            WNCK_WINDOW_CHANGE_X | WNCK_WINDOW_CHANGE_Y,
-                                            window_x, window_y, -1, -1);
-                }
-              else
-                {
-                  g_warning ("only viewport with equally distributed screens are supported: %dx%d & %dx%d",
-                             workspace_width, workspace_height, screen_width, screen_height);
-                }
-            }
-        }
 
-      wnck_window_activate (child->window, timestamp);
-    }
+  if (command)
+    g_free(command);
 }
 
 
