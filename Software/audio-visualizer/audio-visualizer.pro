QT       += core gui multimedia openglwidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audiolistener.cpp \
    glbarspectrumscene.cpp \
    glwaveformscene.cpp \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    signalprocessor.cpp \
    engine.cpp \
    frequencyspectrum.cpp \
    utils.cpp \
    spectrumanalyser.cpp

HEADERS += \
    audiolistener.h \
    glbarspectrumscene.h \
    glscene.h \
    glwaveformscene.h \
    glwidget.h \
    mainwindow.h \
    signalprocessor.h \
    engine.h \
    frequencyspectrum.h \
    spectrum.h \
    utils.h \
    spectrumanalyser.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lopengl32

INCLUDEPATH += $$PWD/libs/fftw-3.3.5-dll64
QMAKE_LIBDIR += $$PWD/libs/fftw-3.3.5-dll64
LIBS += -L/$$PWD/libs/fftw-3.3.5-dll64/ -llibfftw3-3
LIBS += -L/$$PWD/libs/fftw-3.3.5-dll64/ -llibfftw3f-3
LIBS += -L/$$PWD/libs/fftw-3.3.5-dll64/ -llibfftw3l-3
QMAKE_CXXFLAGS += I$$PWD/libs/fftw-3.3.5-dll64

RESOURCES += \
    icons.qrc
