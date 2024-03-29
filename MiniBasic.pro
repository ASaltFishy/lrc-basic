QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11\
    exception

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# for exception
#QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
#QMAKE_CXXFLAGS_STL_ON = /EHa


SOURCES += \
    evalstate.cpp \
    exp.cpp \
    main.cpp \
    mainwindow.cpp \
    parser.cpp \
    program.cpp \
    statement.cpp \
    tokenizer.cpp

HEADERS += \
    evalstate.h \
    exp.h \
    mainwindow.h \
    parser.h \
    program.h \
    statement.h \
    token.h \
    tokenizer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
