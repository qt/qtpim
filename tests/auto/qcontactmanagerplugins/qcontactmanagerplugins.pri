testcocoon {
    # Path to dummyplugin is explicitly added to the list of paths to instrument with code coverage.
    # Some projects uses dummyplugin.h in this location and
    # it is not detected by the overall implementation (since it is not in the same
    # directory as the project). If dummyplugin.h is not instrumented it breaks the
    # instrumentation.
    TESTCOCOON_COVERAGE_OPTIONS_ADD = --cs-include-file-abs-regex=\'$$PWD[/\\\\]dummyplugin[/\\\\].*\\.h\$\$\'
    QMAKE_CXXFLAGS += $$TESTCOCOON_COVERAGE_OPTIONS_ADD
    QMAKE_CFLAGS += $$TESTCOCOON_COVERAGE_OPTIONS_ADD
    QMAKE_LFLAGS += $$TESTCOCOON_COVERAGE_OPTIONS_ADD
}

