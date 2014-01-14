TEMPLATE = app
CONFIG += console testcase

QT += testlib

!qtHaveModule(jsondb): DEFINES += QT_NO_JSONDB
