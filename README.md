I *liberated\** and modified two of the *internal* [xfce4-panel] plugins:  
  - windowmenu
  - tasklist

The modifications made are changing the action triggered when
a window is "*selected*" to `i3run --winid WINDOW_ID`.

This make the plugins work much smoother with [**i3wm**].  
For it to work you need to have [**i3ass**] installed as well.  

I also identified and fixed a [bug in windowmenu], related
to selecting things in the menu with the keyboard.  

Together with the plugins, the script `xfce4-popup-i3-windowmenu`, 
will get installed. The script can be used to bring up the menu
without clicking the button.

```
bindsym Mod1+Tab exec --no-startup-id xfce4-popup-i3-windowmenu
```

To not mess up the xfce4-panel package installed 
by package managers, the plugins are renamed and 
will not overwrite the original internal ones.

## installation

If you are using the best Linux distribution,
Arch, you can install the [xfce4-panel-i3-plugins]
package i added to [AUR].

Otherwise, just clone this repository, install the
build dependencies: [xfce4-panel], [xfce4-dev-tools]
and:

  1. Configure [config.mak](config.mak) if needed
  2. `$ make`
  3. `# make install`

Runtime dependencies: [**i3wm**], [**i3ass**]  


[xfce4-dev-tools]: https://gitlab.xfce.org/xfce/xfce4-dev-tools
[xfce4-panel]: https://gitlab.xfce.org/xfce/xfce4-panel
[**i3ass**]: https://github.com/budlabs/i3ass
[**i3wm**]: https://i3wm.org
[bug in windowmenu]: https://gitlab.xfce.org/xfce/xfce4-panel/-/merge_requests/68
