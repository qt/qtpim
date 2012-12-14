TEMPLATE = app
CONFIG += console testcase

qtAddLibrary(QtTest)

!contains(QT_CONFIG, jsondb): DEFINES += QT_NO_JSONDB
