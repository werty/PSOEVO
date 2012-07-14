#-------------------------------------------------
#
# Project created by QtCreator 2012-05-24T01:11:15
#
#-------------------------------------------------

QT       += core gui opengl xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MGR
TEMPLATE = app

CONFIG(release, debug|release) { #release mode only
    DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG QT_NO_DEBUG_STREAM

}

SOURCES += main.cpp\
    Wykres2d.cpp \
    Wskazniki.cpp \
    struktury.cpp \
    Solution.cpp \
    qtmmlwidget.cpp \
    Problem.cpp \
    Particle.cpp \
    NSGA_II.cpp \
    MOPSO.cpp \
    Forma.cpp \
    qwt3d/qwt3d_types.cpp \
    qwt3d/qwt3d_surfaceplot.cpp \
    qwt3d/qwt3d_scale.cpp \
    qwt3d/qwt3d_plot3d.cpp \
    qwt3d/qwt3d_parametricsurface.cpp \
    qwt3d/qwt3d_meshplot.cpp \
    qwt3d/qwt3d_lighting.cpp \
    qwt3d/qwt3d_label.cpp \
    qwt3d/qwt3d_io_reader.cpp \
    qwt3d/qwt3d_io.cpp \
    qwt3d/qwt3d_gridplot.cpp \
    qwt3d/qwt3d_gridmapping.cpp \
    qwt3d/qwt3d_graphplot.cpp \
    qwt3d/qwt3d_function.cpp \
    qwt3d/qwt3d_extglwidget.cpp \
    qwt3d/qwt3d_enrichment_std.cpp \
    qwt3d/qwt3d_drawable.cpp \
    qwt3d/qwt3d_data.cpp \
    qwt3d/qwt3d_coordsys.cpp \
    qwt3d/qwt3d_color_std.cpp \
    qwt3d/qwt3d_colorlegend.cpp \
    qwt3d/qwt3d_axis.cpp \
    qwt3d/qwt3d_autoscaler.cpp \
    qwt3d/qwt3d_appearance.cpp \
    uzyteczne_funkcje.cpp \
    Chromosome.cpp \
    qcustomplot.cpp \
    GGA.cpp \
    treemodel.cpp \
    treeitem.cpp \
    WorkerThread.cpp

HEADERS  += \
    MOPSO.h \
    Wykres2d.h \
    Wskazniki.h \
    uzyteczne_funkcje.h \
    struktury.h \
    Solution.h \
    qtmmlwidget.h \
    Problem.h \
    Particle.h \
    NSGA_II.h \
    Forma.h \
    qwt3d/qwt3d_volumeplot.h \
    qwt3d/qwt3d_valueptr.h \
    qwt3d/qwt3d_types.h \
    qwt3d/qwt3d_surfaceplot.h \
    qwt3d/qwt3d_scale.h \
    qwt3d/qwt3d_portability.h \
    qwt3d/qwt3d_plot3d.h \
    qwt3d/qwt3d_parametricsurface.h \
    qwt3d/qwt3d_openglhelper.h \
    qwt3d/qwt3d_meshplot.h \
    qwt3d/qwt3d_mapping.h \
    qwt3d/qwt3d_label.h \
    qwt3d/qwt3d_io_reader.h \
    qwt3d/qwt3d_io.h \
    qwt3d/qwt3d_helper.h \
    qwt3d/qwt3d_gridplot.h \
    qwt3d/qwt3d_gridmapping.h \
    qwt3d/qwt3d_graphplot.h \
    qwt3d/qwt3d_global.h \
    qwt3d/qwt3d_function.h \
    qwt3d/qwt3d_extglwidget.h \
    qwt3d/qwt3d_enrichment_std.h \
    qwt3d/qwt3d_enrichment.h \
    qwt3d/qwt3d_drawable.h \
    qwt3d/qwt3d_data.h \
    qwt3d/qwt3d_coordsys.h \
    qwt3d/qwt3d_color_std.h \
    qwt3d/qwt3d_colorlegend.h \
    qwt3d/qwt3d_color.h \
    qwt3d/qwt3d_axis.h \
    qwt3d/qwt3d_autoscaler.h \
    qwt3d/qwt3d_appearance.h \
    WorkerThread.h \
    Chromosome.h \
    qcustomplot.h \
    GGA.h \
    treemodel.h \
    treeitem.h

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += /usr/local/qwt-6.1.0-svn/include
LIBS += -L../muparser_v2_2_2/lib -lmuparser -lglut -lGLU -L/usr/local/qwt-6.1.0-svn/lib -lqwt
FORMS    += Forma.ui




