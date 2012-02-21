TEMPLATE = subdirs
SUBDIRS += auto

# disable 'make check' on Mac OS X and Windows for the time being
mac|win32 {
    auto.CONFIG += no_check_target
}
