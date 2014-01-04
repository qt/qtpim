######################################################################
#
# Simple example of how to use the versit API
#
######################################################################

TEMPLATE = lib
TARGET = qtversitdocsample
include(../../../../features/basic_examples_setup.pri)
INCLUDEPATH += ../../../../src/global \
               ../../../../src/contacts \
               ../../../../src/contacts/requests \
               ../../../../src/contacts/filters \
               ../../../../src/contacts/details \
               ../../../../src/versit

CONFIG += console
QT = contacts versit

SOURCES += qtversitdocsample.cpp
