TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c

INCLUDEPATH += $$PWD/libs/fftw-3.3.5-dll64
QMAKE_LIBDIR += $$PWD/libs/fftw-3.3.5-dll64
LIBS += -L/$$PWD/libs/fftw-3.3.5-dll64/ -llibfftw3-3
LIBS += -L/$$PWD/libs/fftw-3.3.5-dll64/ -llibfftw3f-3
LIBS += -L/$$PWD/libs/fftw-3.3.5-dll64/ -llibfftw3l-3
QMAKE_CXXFLAGS += I$$PWD/libs/fftw-3.3.5-dll64
