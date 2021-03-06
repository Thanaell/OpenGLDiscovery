QT       += core gui

RESOURCES   = textures.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Shape/barshape.cpp \
    game.cpp \
    GLObject/glbackgroundrectangle.cpp \
    GLObject/globject.cpp \
    GLObject/glsquare.cpp \
    glview.cpp \
    Shape/ilshape.cpp \
    Shape/lshape.cpp \
    main.cpp \
    mainwindow.cpp \
    Shape/movableshape.cpp \
    Shape/shape.cpp \
    Shape/sqshape.cpp \
    Shape/sshape.cpp \
    Shape/tshape.cpp \
    Shape/zshape.cpp

HEADERS += \
    Shape/barshape.h \
    game.h \
    GLObject/glbackgroundrectangle.h \
    GLObject/globject.h \
    GLObject/glsquare.h \
    glview.h \
    Shape/ilshape.h \
    Shape/lshape.h \
    mainwindow.h \
    Shape/movableshape.h \
    Shape/shape.h \
    Shape/sqshape.h \
    Shape/sshape.h \
    Shape/tshape.h \
    Shape/zshape.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
