I *liberated\** and modified two of the *internal* [xfce4-panel] plugins:  
  - windowmenu
  - ~tasklist~\*

*\*tastlist mod is currently broken and disabled*  

The modifications made are changing the action triggered when
a window is "*selected*" to `i3run --winid WINDOW_ID`.

This make the plugins work much smoother with [**i3wm**].  
For it to work you need to have [**i3ass**] installed as well.  

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

---

*\*: with liberated i meant that i made it so you don't need to build
all of xfce4-panel to build the plugins. They are also liberated from 
autotools.*

[xfce4-panel-i3-plugins]: https://aur.archlinux.org/packages/xfce4-panel-i3-plugins
[AUR]: https://aur.archlinux.org/
[xfce4-dev-tools]: https://gitlab.xfce.org/xfce/xfce4-dev-tools
[xfce4-panel]: https://gitlab.xfce.org/xfce/xfce4-panel
[**i3ass**]: https://github.com/budlabs/i3ass
[**i3wm**]: https://i3wm.org
[bug in windowmenu]: https://gitlab.xfce.org/xfce/xfce4-panel/-/merge_requests/68
