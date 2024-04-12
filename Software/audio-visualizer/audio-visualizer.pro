QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# ---PORTAUDIO---

# Path to the PortAudio source code directory
PORTAUDIO_DIR = $$PWD/libs/portaudio

win32 {

    # Include path for PortAudio headers
    INCLUDEPATH += $$PORTAUDIO_DIR/include

    # Link against the PortAudio library
    LIBS += -L$$PORTAUDIO_DIR/lib/.libs -lportaudio
}

linux-g++* {
    INCLUDEPATH += $$PORTAUDIO_DIR/include
    LIBS += -L$$PORTAUDIO_DIR/lib -lportaudio -lrt -lm -pthread -lasound -ljack
}
