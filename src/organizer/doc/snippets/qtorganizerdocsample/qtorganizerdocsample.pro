######################################################################
#
# Simple example of how to use the contacts API
#
######################################################################

TEMPLATE = app
TARGET = qtorganizerdocsample
include(../../../../features/basic_examples_setup.pri)
INCLUDEPATH += ../../../../src/global \
               ../../../../src/organizer \
               ../../../../src/organizer/details \
               ../../../../src/organizer/requests \
               ../../../../src/organizer/items \
               ../../../../src/organizer/filters

CONFIG += console
QT = organizer

SOURCES += qtorganizerdocsample.cpp
