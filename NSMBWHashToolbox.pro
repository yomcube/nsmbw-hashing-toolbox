TEMPLATE = app
TARGET = nsmbw_hash_toolbox

QT = core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/demanglewidget.cpp \
    src/dictionarybruteprocess.cpp \
    src/dictionarybrutewidget.cpp \
    src/hashing.cpp \
    src/hashwidget.cpp \
    src/invhashwidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/nvidia_demangler.cpp \
    src/symbollistmodel.cpp \
    src/symbollistwidget.cpp \
    src/xortrickwidget.cpp

FORMS += \
    src/demanglewidget.ui \
    src/dictionarybrutewidget.ui \
    src/hashwidget.ui \
    src/invhashwidget.ui \
    src/mainwindow.ui \
    src/symbollistwidget.ui \
    src/xortrickwidget.ui

HEADERS += \
    src/demanglewidget.h \
    src/dictionarybruteprocess.h \
    src/dictionarybrutewidget.h \
    src/hashing.h \
    src/hashwidget.h \
    src/invhashwidget.h \
    src/mainwindow.h \
    src/nvidia_demangler.h \
    src/symbollistmodel.h \
    src/symbollistwidget.h \
    src/xortrickwidget.h

RESOURCES += \
    src/resources.qrc
