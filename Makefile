.PHONY: clean check all install uninstall      \
	      install-tasklist   uninstall-tasklist  \
	      install-windowmenu uninstall-windowmenu

.ONESHELL:

SHELL           := /bin/bash

include config.mak

common_src        := $(wildcard common/*.c)
common_obj        := $(addprefix build/common/,$(notdir $(common_src:.c=.o) ))

tasklist_src      := $(wildcard src/i3-tasklist/*.c)
tasklist_obj      := $(addprefix build/tasklist/,$(notdir $(tasklist_src:.c=.o) ))

windowmenu_src    := $(wildcard src/i3-windowmenu/*.c)
windowmenu_obj    := $(addprefix build/windowmenu/,$(notdir $(windowmenu_src:.c=.o) ))

all_obj           := $(common_obj) $(tasklist_obj) $(windowmenu_obj)
deps              := $(all_obj:.o=.d)

windowmenu_installed_plugin       := \
	$(libdir)/$(namespace)/libi3windowmenu.so
windowmenu_installed_desktop_file := \
	$(datadir)/$(namespace)/i3-windowmenu.desktop
windowmenu_installed_script       := \
	$(bindir)/xfce4-popup-i3-windowmenu

tasklist_installed_plugin         := \
	$(libdir)/$(namespace)/libi3tasklist.so
tasklist_installed_desktop_file   := \
	$(datadir)/$(namespace)/i3-tasklist.desktop

tasklist_ui   := build/tasklist-dialog_ui.h
windowmenu_ui := build/windowmenu-dialog_ui.h

all: build/libi3tasklist.so build/libi3windowmenu.so build/xfce4-popup-i3-windowmenu

install:   install-windowmenu   install-tasklist
uninstall: uninstall-windowmenu uninstall-tasklist

-include $(deps)

build/libi3tasklist.so: build/libcommon.a $(tasklist_ui) $(tasklist_obj) | build/
	$(CC) -shared -fPIC -o $@ $(tasklist_obj) $(ALL_LDFLAGS) -Lbuild -lcommon

build/libi3windowmenu.so: build/libcommon.a $(windowmenu_ui) $(windowmenu_obj) | build/
	$(CC) -shared -fPIC -o $@ $(windowmenu_obj) $(ALL_LDFLAGS) -Lbuild -lcommon

$(tasklist_obj): build/tasklist/%.o : src/i3-tasklist/%.c | build/
	$(CC) $(ALL_CFLAGS) -Ii3-tasklist -DG_LOG_DOMAIN=\"libi3tasklist\" -c $< -o $@ -fPIC 

$(windowmenu_obj): build/windowmenu/%.o : src/i3-windowmenu/%.c | build/
	$(CC) $(ALL_CFLAGS) -Ii3-windowmenu -DG_LOG_DOMAIN=\"libi3windowmenu\" -c $< -o $@ -fPIC

install-tasklist: build/libi3tasklist.so
	install -Dm755 $< $(tasklist_installed_plugin)
	install -Dm644 data/i3-tasklist.desktop $(tasklist_installed_desktop_file)

install-windowmenu: build/libi3windowmenu.so build/xfce4-popup-i3-windowmenu
	install -Dm755 build/libi3windowmenu.so $(windowmenu_installed_plugin)
	install -Dm755 build/xfce4-popup-i3-windowmenu $(windowmenu_installed_script)
	install -Dm644 data/i3-windowmenu.desktop $(windowmenu_installed_desktop_file)

uninstall-tasklist:
	rm $(tasklist_installed_desktop_file)
	rm $(tasklist_installed_plugin)

uninstall-windowmenu:
	rm $(windowmenu_installed_desktop_file)
	rm $(windowmenu_installed_script)
	rm $(windowmenu_installed_plugin)

build/xfce4-popup-i3-windowmenu: src/i3-windowmenu/xfce4-popup-i3-windowmenu.sh
	$(SED) -e "s,\@bindir\@,$(bindir),g" \
	       -e "s,\@localedir\@,$(localedir),g" $< >$@

$(tasklist_ui): src/i3-tasklist/tasklist-dialog.glade | build/
	xdt-csource --static --strip-comments --strip-content --name=tasklist_dialog_ui $< >$@

$(windowmenu_ui): src/i3-windowmenu/windowmenu-dialog.glade | build/
	xdt-csource --static --strip-comments --strip-content --name=windowmenu_dialog_ui $< >$@

$(common_obj): build/common/%.o : common/%.c | build/
	$(CC)  $(ALL_CFLAGS)  -c $< -o $@ -fPIC 

build/libcommon.a: $(common_obj) | build/
	$(AR) rcs $@ $^

build/:
	mkdir -p build/{common,windowmenu,tasklist}

clean:
	rm -r build
