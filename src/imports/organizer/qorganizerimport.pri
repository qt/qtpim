load(qt_module)

TEMPLATE = lib
CONFIG += qt plugin

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release

isEmpty(TARGETPATH) {
    error("qimportbase.pri: You must provide a TARGETPATH!")
}

isEmpty(TARGET) {
    error("qimportbase.pri: You must provide a TARGET!")
}

# Copy qmldir file to build folder
QMLDIRFILE = $${_PRO_FILE_PWD_}/qmldir
copyqmldir2build.input = QMLDIRFILE
copyqmldir2build.output = $$QT.organizer.imports/$$TARGETPATH/qmldir

!contains(TEMPLATE_PREFIX, vc):copyqmldir2build.variable_out = PRE_TARGETDEPS
copyqmldir2build.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copyqmldir2build.name = COPY ${QMAKE_FILE_IN}
copyqmldir2build.CONFIG += no_link
copyqmldir2build.CONFIG += no_clean
QMAKE_EXTRA_COMPILERS += copyqmldir2build

# Copy plugins.qmltypes file to build folder
QMLTYPEFILE = $${_PRO_FILE_PWD_}/plugins.qmltypes
copyqmltypes2build.input = QMLTYPEFILE
copyqmltypes2build.output = $$QT.organizer.imports/$$TARGETPATH/plugins.qmltypes

!contains(TEMPLATE_PREFIX, vc):copyqmltypes2build.variable_out = PRE_TARGETDEPS
copyqmltypes2build.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copyqmltypes2build.name = COPY ${QMAKE_FILE_IN}
copyqmltypes2build.CONFIG += no_link
copyqmltypes2build.CONFIG += no_clean
QMAKE_EXTRA_COMPILERS += copyqmltypes2build

TARGET = $$qtLibraryTarget($$TARGET)
contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols

load(qt_targets)
