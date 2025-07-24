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
    src/kmpattackwidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/nvidia_demangler.cpp \
    src/xortrickwidget.cpp \
    src/z3attackprocess.cpp \
    src/z3attackwidget.cpp

FORMS += \
    src/demanglewidget.ui \
    src/dictionarybrutewidget.ui \
    src/hashwidget.ui \
    src/invhashwidget.ui \
    src/kmpattackwidget.ui \
    src/mainwindow.ui \
    src/xortrickwidget.ui \
    src/z3attackwidget.ui

HEADERS += \
    src/demanglewidget.h \
    src/dictionarybruteprocess.h \
    src/dictionarybrutewidget.h \
    src/hashing.h \
    src/hashwidget.h \
    src/invhashwidget.h \
    src/kmpattackwidget.h \
    src/mainwindow.h \
    src/nvidia_demangler.h \
    src/xortrickwidget.h \
    src/z3attackprocess.h \
    src/z3attackwidget.h

RESOURCES += \
    src/resources.qrc

unix|win32: LIBS += -L$$PWD/libs/z3/bin/ -llibz3

INCLUDEPATH += $$PWD/libs/z3/include
DEPENDPATH += $$PWD/libs/z3/include
