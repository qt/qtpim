TEMPLATE = app
TARGET = todo
QT += organizer widgets

HEADERS += todoeditor.h \
           window.h

SOURCES += window.cpp \
           todoeditor.cpp \
           main.cpp

target.path  = $$[QT_INSTALL_EXAMPLES]/organizer/todo
INSTALLS    += target
