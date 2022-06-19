.PHONY: prep

# use prep to clone upstream xfce4-panel, copy files
# and patch the ones needed

us_tasklist_ignore = xfce4-panel/plugins/tasklist/tasklist.desktop.in.in xfce4-panel/plugins/tasklist/Makefile.am
us_tasklist = $(filter-out $(us_tasklist_ignore), $(wildcard xfce4-panel/plugins/tasklist/*))
ds_tasklist = $(us_tasklist:xfce4-panel/plugins/tasklist/%=src/i3-tasklist/%)

us_windowmenu_ignore = xfce4-panel/plugins/windowmenu/xfce4-popup-windowmenu.sh xfce4-panel/plugins/windowmenu/windowmenu.desktop.in.in xfce4-panel/plugins/windowmenu/Makefile.am
us_windowmenu = $(filter-out $(us_windowmenu_ignore), $(wildcard xfce4-panel/plugins/windowmenu/*))
ds_windowmenu = $(us_windowmenu:xfce4-panel/plugins/windowmenu/%=src/i3-windowmenu/%)

us_common_ignore = xfce4-panel/common/Makefile.am
us_common = $(filter-out $(us_common_ignore), $(wildcard xfce4-panel/common/*))
ds_common = $(us_common:xfce4-panel/common/%=common/%)

prep: $(ds_tasklist) $(ds_windowmenu) $(ds_common) | xfce4-panel/

xfce4-panel/:
	git clone git@gitlab.xfce.org:xfce/xfce4-panel.git

$(ds_common): common/% : xfce4-panel/common/%
	cp -f $^ $@

$(ds_windowmenu): src/i3-windowmenu/% : xfce4-panel/plugins/windowmenu/%
	if [[ $@ = "src/i3-windowmenu/windowmenu.c" ]]
		then
			rm -f $@
			patch -p1 -u $^ -i patch_windowmenu.c -o $@
	else
		cp -f $^ $@
	fi

$(ds_tasklist): src/i3-tasklist/% : xfce4-panel/plugins/tasklist/%
	if [[ $@ = "src/i3-tasklist/tasklist-widget.c" ]]
		then
			rm -f $@
			patch -p1 -u $^ -i patch_tasklist-widget.c -o $@
	else
		cp -f $^ $@
	fi
