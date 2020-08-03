TEMPLATE = app
CONFIG += c++17 precompile_header
TARGET = quadris

QT += core gui widgets

INCLUDEPATH += ./

PRECOMPILED_HEADER = ./stable.h

# Source Files
SOURCES = $$files(*.cc, true)
HEADERS = $$files(*.h, true)

# Output Files
OBJECTS_DIR = ./bin
MOC_DIR = ./bin

# Warn deprecated Qt features
DEFINES += QT_DEPRECATED_WARNINGS
