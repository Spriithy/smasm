TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c99

SOURCES += src/main.c \
    src/opcodes.c \
    src/interpreter.c \
    src/compiler.c

HEADERS += \
    src/include/colors.h \
    src/include/opcodes.h \
    src/include/interpreter.h \
    src/include/compiler.h
