.PHONY: clean check all install uninstall

SHELL           := /bin/bash
.DEFAULT_GOAL   := all
.ONESHELL:

include config.mak

# include prep.mak

common_src        = $(wildcard common/*.c)
common_obj        = $(addprefix build/common/,$(notdir $(common_src:.c=.o) ))

all_obj           = $(common_obj)

include windowmenu.mak
# include tasklist.mak

all: $(build_all)

install:   $(install-all)
uninstall: $(uninstall-all)

deps = $(all_obj:.o=.d)
-include $(deps)

$(common_obj): build/common/%.o : common/%.c | build/
	$(CC)  $(ALL_CFLAGS)  -c $< -o $@ -fPIC 

build/libcommon.a: $(common_obj) | build/
	$(AR) rcs $@ $^

build/:
	mkdir -p build/{common,windowmenu,tasklist}

clean:
	rm -r build
