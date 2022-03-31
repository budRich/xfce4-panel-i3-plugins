PREFIX            ?= /usr
CFLAGS            ?= -Wall -O2
CC                ?= gcc
PKG_CONFIG        ?= pkg-config
SED               ?= sed

bindir            := $(DESTDIR)$(PREFIX)/bin
localedir         := $(DESTDIR)$(PREFIX)/locale
datadir           := $(DESTDIR)$(PREFIX)/share
libdir            := $(DESTDIR)$(PREFIX)/lib
namespace         := xfce4/panel/plugins

DEPS := \
	libxfce4panel-2.0 \
	libxfce4util-1.0  \
	libxfce4ui-2      \
	libxfconf-0       \
	exo-2             \
	libwnck-3.0

DEPS_CFLAGS := $(shell $(PKG_CONFIG) --cflags $(DEPS))
DEPS_LIBS   := $(shell $(PKG_CONFIG) --libs $(DEPS))

INCS := -Ibuild -I.

ALL_CPPFLAGS =                             \
	-DPACKAGE_NAME='"xfce4-panel"'           \
	-DWNCK_I_KNOW_THIS_IS_UNSTABLE           \
  $(CPPFLAGS)

ALL_LDFLAGS =      \
  $(DEPS_LIBS)     \
  -lXext           \
  $(LDFLAGS)

# comment out if you are missing math.h or string.h
ALL_CPPFLAGS += -DHAVE_MATH_H
ALL_LDFLAGS  += -lm
ALL_CPPFLAGS += -DHAVE_STRING_H

ALL_CFLAGS =  \
  $(INCS) $(DEPS_CFLAGS) $(CFLAGS)  -MMD $(ALL_CPPFLAGS)
