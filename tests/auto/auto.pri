TEMPLATE = app
CONFIG += qt debug warn_on console depend_includepath testcase

qtAddLibrary(QtTest)

!contains(QT_CONFIG, jsondb): DEFINES += QT_NO_JSONDB
