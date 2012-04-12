%modules = ( # path to module name map
    "QtContacts" => "$basedir/src/contacts",
    "QtOrganizer" => "$basedir/src/organizer",
    "QtVersit" => "$basedir/src/versit",
    "QtVersitOrganizer" => "$basedir/src/versitorganizer",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);
%classnames = (
    "qtcontactsversion.h" => "QtContactsVersion",
    "qtorganizerversion.h" => "QtOrganizerVersion",
    "qtversitversion.h" => "QtVersitVersion",
    "qtversitorganizerversion.h" => "QtVersitOrganizerVersion",
);
%mastercontent = (
    "contacts" => "#include <QtContacts/QtContacts>\n",
    "organizer" => "#include <QtOrganizer/QtOrganizer>\n",
    "versit" => "#include <QtVersit/QtVersit>\n",
    "versitorganizer" => "#include <QtVersitOrganizer/QtVersitOrganizer>\n",
);
# Module dependencies.
# Every module that is required to build this module should have one entry.
# Each of the module version specifiers can take one of the following values:
#   - A specific Git revision.
#   - any git symbolic ref resolvable from the module's repository (e.g. "refs/heads/master" to track master branch)
#
%dependencies = (
    "qtbase" => "refs/heads/master",
    "qtxmlpatterns" => "refs/heads/master",
    "qtdeclarative" => "refs/heads/master",
    "qtjsbackend" => "refs/heads/master",
    "qtjsondb" => "refs/heads/master",
);
