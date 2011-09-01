TEMPLATE = app
CONFIG += qt warn_on console depend_includepath testcase

qtAddLibrary(QtTest)

!contains(QT_CONFIG, jsondb): DEFINES += QT_NO_JSONDB
