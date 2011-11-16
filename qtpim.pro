TEMPLATE = subdirs
CONFIG += ordered

module_qtpim_src.subdir = src
module_qtpim_src.target = module-qtpim-src

module_qtpim_tests.subdir = tests
module_qtpim_tests.target = module-qtpim-tests
module_qtpim_tests.depends = module_qtpim_src
module_qtpim_tests.CONFIG = no_default_install
!contains(QT_BUILD_PARTS,tests) {
    module_qtpim_tests.CONFIG += no_default_target
}

module_qtpim_examples.subdir = examples
module_qtpim_examples.target = module-qtpim-examples
module_qtpim_examples.depends = module_qtpim_src
!contains(QT_BUILD_PARTS,examples) {
    module_qtpim_examples.CONFIG = no_default_target no_default_install
    warning("No examples being used")
}

SUBDIRS += module_qtpim_src \
           module_qtpim_tests \
           module_qtpim_examples
