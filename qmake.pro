QT += core gui widgets
CONFIG += c++11 link_pkgconfig
TARGET = SimplePDF
TEMPLATE = app
SOURCES += spdf.cpp
PKGCONFIG += poppler-qt5
INCLUDEPATH += /usr/include/poppler/qt5
QMAKE_CXXFLAGS += -Wno-deprecated-declarations
