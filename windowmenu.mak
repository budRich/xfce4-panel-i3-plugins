.PHONY: install-windowmenu uninstall-windowmenu

windowmenu_src    = $(wildcard src/i3-windowmenu/*.c)
windowmenu_obj    = $(addprefix build/windowmenu/,$(notdir $(windowmenu_src:.c=.o) ))
all_obj          += $(windowmenu_obj)
build_all        += build/libi3windowmenu.so build/xfce4-popup-i3-windowmenu

windowmenu_installed_plugin       := \
	$(libdir)/$(namespace)/libi3windowmenu.so
windowmenu_installed_desktop_file := \
	$(datadir)/$(namespace)/i3-windowmenu.desktop
windowmenu_installed_script       := \
	$(bindir)/xfce4-popup-i3-windowmenu

windowmenu_ui := build/windowmenu-dialog_ui.h

install-all      += install-windowmenu
uninstall-all    += uninstall-windowmenu

build/libi3windowmenu.so: build/libcommon.a $(windowmenu_ui) $(windowmenu_obj) | build/
	$(CC) -shared -fPIC -o $@ $(windowmenu_obj) $(ALL_LDFLAGS) -Lbuild -lcommon


$(windowmenu_obj): build/windowmenu/%.o : src/i3-windowmenu/%.c | build/
	$(CC) $(ALL_CFLAGS) -Ii3-windowmenu -DG_LOG_DOMAIN=\"libi3windowmenu\" -c $< -o $@ -fPIC

install-windowmenu: build/libi3windowmenu.so build/xfce4-popup-i3-windowmenu
	install -Dm755 build/libi3windowmenu.so $(windowmenu_installed_plugin)
	install -Dm755 build/xfce4-popup-i3-windowmenu $(windowmenu_installed_script)
	install -Dm644 data/i3-windowmenu.desktop $(windowmenu_installed_desktop_file)

uninstall-windowmenu:
	rm $(windowmenu_installed_desktop_file)
	rm $(windowmenu_installed_script)
	rm $(windowmenu_installed_plugin)

build/xfce4-popup-i3-windowmenu: src/i3-windowmenu/xfce4-popup-i3-windowmenu.sh
	$(SED) -e "s,\@bindir\@,$(bindir),g" \
	       -e "s,\@localedir\@,$(localedir),g" $< >$@

$(windowmenu_ui): src/i3-windowmenu/windowmenu-dialog.glade | build/
	xdt-csource --static --strip-comments --strip-content --name=windowmenu_dialog_ui $< >$@
