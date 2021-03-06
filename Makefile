#############################################################################
# Makefile for building: wave
# Generated by qmake (2.01a) (Qt 4.6.2) on: mi� may 25 22:27:44 2011
# Project:  wave.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile wave.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib -lQtGui -lQtCore -lpthread -lasound -lfftw3
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		opstrama.cpp \
		transceptor.cpp \
		ventana.cpp moc_ventana.cpp \
		qrc_imagenes.cpp
OBJECTS       = main.o \
		opstrama.o \
		transceptor.o \
		ventana.o \
		moc_ventana.o \
		qrc_imagenes.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		wave.pro
QMAKE_TARGET  = wave
DESTDIR       = 
TARGET        = wave

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_ventana.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: wave.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -unix -o Makefile wave.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile wave.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/wave1.0.0 || $(MKDIR) .tmp/wave1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/wave1.0.0/ && $(COPY_FILE) --parents opstrama.h transceptor.h ventana.h .tmp/wave1.0.0/ && $(COPY_FILE) --parents imagenes.qrc .tmp/wave1.0.0/ && $(COPY_FILE) --parents main.cpp opstrama.cpp transceptor.cpp ventana.cpp .tmp/wave1.0.0/ && $(COPY_FILE) --parents ventana.ui .tmp/wave1.0.0/ && (cd `dirname .tmp/wave1.0.0` && $(TAR) wave1.0.0.tar wave1.0.0 && $(COMPRESS) wave1.0.0.tar) && $(MOVE) `dirname .tmp/wave1.0.0`/wave1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/wave1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_ventana.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_ventana.cpp
moc_ventana.cpp: transceptor.h \
		opstrama.h \
		ventana.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ventana.h -o moc_ventana.cpp

compiler_rcc_make_all: qrc_imagenes.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_imagenes.cpp
qrc_imagenes.cpp: imagenes.qrc \
		images/logo.png \
		images/icon.png
	/usr/bin/rcc -name imagenes imagenes.qrc -o qrc_imagenes.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_ventana.h
compiler_uic_clean:
	-$(DEL_FILE) ui_ventana.h
ui_ventana.h: ventana.ui
	/usr/bin/uic-qt4 ventana.ui -o ui_ventana.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

main.o: main.cpp ventana.h \
		transceptor.h \
		opstrama.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

opstrama.o: opstrama.cpp opstrama.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o opstrama.o opstrama.cpp

transceptor.o: transceptor.cpp transceptor.h \
		opstrama.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o transceptor.o transceptor.cpp

ventana.o: ventana.cpp ventana.h \
		transceptor.h \
		opstrama.h \
		ui_ventana.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ventana.o ventana.cpp

moc_ventana.o: moc_ventana.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ventana.o moc_ventana.cpp

qrc_imagenes.o: qrc_imagenes.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_imagenes.o qrc_imagenes.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

