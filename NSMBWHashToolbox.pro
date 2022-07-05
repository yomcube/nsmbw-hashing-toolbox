TEMPLATE = app
TARGET = nsmbw_hash_toolbox

QT = core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    dictionarybruteprocess.cpp \
    dictionarybrutewidget.cpp \
    hashing.cpp \
    invhashwidget.cpp \
    kmpattackwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    symbollistmodel.cpp \
    symbollistwidget.cpp \
    xortrickwidget.cpp \
    z3attackprocess.cpp \
    z3attackwidget.cpp

FORMS += \
    dictionarybrutewidget.ui \
    invhashwidget.ui \
    kmpattackwidget.ui \
    mainwindow.ui \
    symbollistwidget.ui \
    xortrickwidget.ui \
    z3attackwidget.ui

HEADERS += \
    dictionarybruteprocess.h \
    dictionarybrutewidget.h \
    hashing.h \
    invhashwidget.h \
    kmpattackwidget.h \
    mainwindow.h \
    symbollistmodel.h \
    symbollistwidget.h \
    xortrickwidget.h \
    z3attackprocess.h \
    z3attackwidget.h

RESOURCES += \
    resources.qrc

unix|win32: LIBS += -L$$PWD/libs/z3/bin/ -llibz3

INCLUDEPATH += $$PWD/libs/z3/include
DEPENDPATH += $$PWD/libs/z3/include
