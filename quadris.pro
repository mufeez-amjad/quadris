TEMPLATE = app
CONFIG += c++17
TARGET = quadris

QT += core gui widgets

INCLUDEPATH += ./

# Source Files
SOURCES = $$files(*.cc, true)
HEADERS = $$files(*.h, true)

# Output Files
OBJECTS_DIR = ./bin
MOC_DIR = ./bin

# Warn deprecated Qt features
DEFINES += QT_DEPRECATED_WARNINGS
