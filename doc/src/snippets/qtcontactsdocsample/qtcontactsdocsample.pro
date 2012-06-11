######################################################################
#
# Simple example of how to use the contacts API
#
######################################################################

TEMPLATE = lib
TARGET = qtcontactsdocsample
INCLUDEPATH += ../../../../src/global \
               ../../../../src/contacts \
               ../../../../src/contacts/requests \
               ../../../../src/contacts/filters \
               ../../../../src/contacts/details

CONFIG += console
QT += contacts

SOURCES += qtcontactsdocsample.cpp qtcontactsdocsampleasync.cpp
HEADERS += requestexample.h
