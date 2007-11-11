## Process this file with automake to produce Makefile.in

## Created by Anjuta

INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" 

EXTRA_DIST = aiof.glade

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = aiof

aiof_SOURCES = \
	gaiof.h \
	gaiof.cpp \
	main.cpp \
	classname.h \
	neuron.cpp \
	neuron.h \
	perceptron.cpp \
	perceptron.h \
	recognition_engine.cpp \
	recognition_engine.h \
	utils.cpp \
	utils.h \
	zernike.cpp \
	zernike.h \
	interface.h \
	typedefs.h

aiof_CPPFLAGS = \
  $(DEPS_CFLAGS)

aiof_LDFLAGS = -export-dynamic

aiof_LDADD = \
  $(DEPS_LIBS)
