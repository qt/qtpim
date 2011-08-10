%modules = ( # path to module name map
    "QtAddOnContacts" => "$basedir/src/contacts",
    "QtAddOnOrganizer" => "$basedir/src/organizer",
    "QtAddOnVersit" => "$basedir/src/versit",
    "QtAddOnVersitOrganizer" => "$basedir/src/versitorganizer",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);
%classnames = (
    "qtaddoncontactsversion.h" => "QtAddOnContactsVersion",
    "qtaddonorganizerversion.h" => "QtAddOnOrganizerVersion",
    "qtaddonversitversion.h" => "QtAddOnVersitVersion",
    "qtaddonversitorganizerversion.h" => "QtAddOnVersitOrganizerVersion",
);
%mastercontent = (
    "contacts" => "#include <QtAddOnContacts/QtAddOnContacts>\n",
    "organizer" => "#include <QtAddOnOrganizer/QtAddOnOrganizer>\n",
    "versit" => "#include <QtAddOnVersit/QtAddOnVersit>\n",
    "versitorganizer" => "#include <QtAddOnVersitOrganizer/QtAddOnVersitOrganizer>\n",
);
%modulepris = (
    "QtAddOnContacts" => "$basedir/modules/qt_contacts.pri",
    "QtAddOnOrganizer" => "$basedir/modules/qt_organizer.pri",
    "QtAddOnVersit" => "$basedir/modules/qt_versit.pri",
    "QtAddOnVersitOrganizer" => "$basedir/modules/qt_versitorganizer.pri",
);
# Module dependencies.
# Every module that is required to build this module should have one entry.
# Each of the module version specifiers can take one of the following values:
#   - A specific Git revision.
#   - any git symbolic ref resolvable from the module's repository (e.g. "refs/heads/master" to track master branch)
#
%dependencies = (
    "qtbase" => "refs/heads/master",
    "qtsvg" => "refs/heads/master",
    "qtxmlpatterns" => "refs/heads/master",
    "qtdeclarative" => "refs/heads/master",
);
