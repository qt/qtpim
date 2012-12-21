TEMPLATE = app
CONFIG += console testcase

qtAddLibrary(QtTest)

!qtHaveModule(jsondb): DEFINES += QT_NO_JSONDB
