QT += core
QT -= gui

CONFIG += c++11

TARGET = solver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src/bitwisemask.cpp \
    src/board.cpp \
    src/lookup.cpp \
    src/main.cpp \
    src/piecevector.cpp \
    src/placement.cpp \
    src/tile.cpp

HEADERS += \
    src/bitwisemask.h \
    src/board.h \
    src/boardh.h \
    src/boardv.h \
    src/env.h \
    src/lookup.h \
    src/piece.h \
    src/piecevector.h \
    src/placement.h \
    src/solver.h \
    src/tile.h \
    src/tilearraylookup.h \
    src/tilearraylookuparray.h \
    src/tilearraylookupmap.h \
    src/tilearraylookupvector.h

