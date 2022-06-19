.PHONY: install-tasklist   uninstall-tasklist

tasklist_src      = $(wildcard src/i3-tasklist/*.c)
tasklist_obj      = $(addprefix build/tasklist/,$(notdir $(tasklist_src:.c=.o) ))
all_obj          += $(tasklist_obj)

tasklist_installed_plugin         := \
	$(libdir)/$(namespace)/libi3tasklist.so
tasklist_installed_desktop_file   := \
	$(datadir)/$(namespace)/i3-tasklist.desktop

tasklist_ui   := build/tasklist-dialog_ui.h

install-all      += install-tasklist
uninstall-all    += uninstall-tasklist
build_all        += build/libi3tasklist.so

build/libi3tasklist.so: build/libcommon.a $(tasklist_ui) $(tasklist_obj) | build/
	$(CC) -shared -fPIC -o $@ $(tasklist_obj) $(ALL_LDFLAGS) -Lbuild -lcommon

$(tasklist_obj): build/tasklist/%.o : src/i3-tasklist/%.c | build/
	$(CC) $(ALL_CFLAGS) -Ii3-tasklist -DG_LOG_DOMAIN=\"libi3tasklist\" -c $< -o $@ -fPIC 

install-tasklist: build/libi3tasklist.so
	install -Dm755 $< $(tasklist_installed_plugin)
	install -Dm644 data/i3-tasklist.desktop $(tasklist_installed_desktop_file)

uninstall-tasklist:
	rm $(tasklist_installed_desktop_file)
	rm $(tasklist_installed_plugin)

$(tasklist_ui): src/i3-tasklist/tasklist-dialog.glade | build/
	xdt-csource --static --strip-comments --strip-content --name=tasklist_dialog_ui $< >$@
