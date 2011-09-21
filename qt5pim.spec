Name:           qt5pim
Version:        5.0.0
Release:        1
License:        LGPL
Summary:        Qt 5 AddOn Pim module
Url:            http://qt.nokia.com/
Source:         %name.tar.bz2
BuildRequires:  libqt5core-minimal
BuildRequires:  libqt5base-devel
BuildRequires:  libqt5declarative-devel
BuildRequires:  qt5jsondb-devel
BuildRoot:      %{_tmppath}/%{name}-%{version}-build


%description
The Qt 5 AddOn Pim module

%package -n libqt5contacts
Summary:        Qt 5 Contacts library
Group:          System/Libraries
%description -n libqt5contacts
Qt 5 Contacts libraries

%package -n libqt5organizer
Summary:        Qt 5 Organizer library
Group:          System/Libraries
Requires:       libqt5contacts
%description -n libqt5organizer
Qt 5 Organizer libraries

%package -n libqt5versit
Summary:        Qt 5 Versit library
Group:          System/Libraries
Requires:       libqt5contacts
%description -n libqt5versit
Qt 5 Versit libraries

%package -n libqt5versitorganizer  
Summary:    Qt 5 Versit Organizer library  
Group:      Development/Tools
Requires:   libqt5contacts
Requires:   libqt5organizer
Requires:   libqt5versit
%description -n libqt5versitorganizer
Qt 5 Versit Organizer libraries

%package -n libqt5pim-devel
Summary:        Qt 5 Pim files
Group:          System/Libraries
Requires:       libqt5contacts
Requires:       libqt5organizer
Requires:       libqt5versit
Requires:       libqt5versitorganizer
%description -n libqt5pim-devel
Qt 5 Pim development files


%prep
%setup -q -n %{name}

%build
# point to the location where the mkspecs can be found
export QTDIR=/usr/share/qt5 && qmake

make %{?_smp_mflags}

# Make sure this is built  
make %{?_smp_mflags} module-qtpim-tests  

%install
make install INSTALL_ROOT=%{buildroot}
make -C tests install INSTALL_ROOT=%{buildroot}  

# Fix wrong path in pkgconfig files
find %{buildroot}%{_libdir}/pkgconfig -type f -name '*.pc' \
-exec perl -pi -e "s, -L%{_builddir}/%{name}/?\S+,,g" {} \;

# Fix wrong path in prl files
find %{buildroot}%{_libdir} -type f -name '*.prl' \
-exec sed -i -e "/^QMAKE_PRL_BUILD_DIR/d;s/\(QMAKE_PRL_LIBS =\).*/\1/" {} \;

rm -rf %{buildroot}%{_prefix}/tests
rm -f %{buildroot}%{_libdir}/libQt*.la

%check
export QT_QPA_PLATFORM=Minimal  
export QT_PLUGIN_PATH=$RPM_BUILD_ROOT/%{_libdir}/qt5/plugins:$QT_PLUGIN_PATH  
export LD_LIBRARY_PATH=%{buildroot}/%{_libdir}:$LD_LIBRARY_PATH  
# Some tests don't work in OBS at the moment.
# make -k -C tests check 

cd tests/auto
qlatin1constant/qlatin1constant
qmalgorithms/qmalgorithms
qcontact/qcontact
qcontactdetail/qcontactdetail
qcontactdetaildefinition/qcontactdetaildefinition
qcontactdetails/qcontactdetails
qcontactfilter/qcontactfilter
# qcontactmanagerplugins/unittest/unittest
qcontactrelationship/qcontactrelationship
# qcontactmemusage/qcontactmemusage !segmentation fault

qorganizercollection/qorganizercollection 
qorganizeritem/qorganizeritem
# qorganizeritemasync/unittest/unittest
qorganizeritemdetail/qorganizeritemdetail
qorganizeritemdetails/qorganizeritemdetails
qorganizeritemdetaildefinition/qorganizeritemdetaildefinition
qorganizeritemfilter/qorganizeritemfilter
# qorganizermanager/qorganizermanager
# qorganizermanagerdetails/qorganizermanagerdetails

qvcard21writer/qvcard21writer
qvcard30writer/qvcard30writer
qversitcontactexporter/qversitcontactexporter
qversitcontactimporter/qversitcontactimporter
# qversitcontactplugins/unittest/unittest
qversitdocument/qversitdocument
qversitproperty/qversitproperty
qversitreader/qversitreader
qversitwriter/qversitwriter
qversitorganizerexporter/qversitorganizerexporter
qversitorganizerimporter/qversitorganizerimporter
qversit/qversit

rm -rf %{buildroot}%{_prefix}/tests

%clean


%post -n libqt5contacts -p /sbin/ldconfig

%postun -n libqt5contacts -p /sbin/ldconfig


%post -n libqt5organizer -p /sbin/ldconfig

%postun -n libqt5organizer -p /sbin/ldconfig


%post -n libqt5versit -p /sbin/ldconfig

%postun -n libqt5versit -p /sbin/ldconfig


%post -n libqt5versitorganizer -p /sbin/ldconfig

%postun -n libqt5versitorganizer -p /sbin/ldconfig


%files -n libqt5contacts
%defattr(-,root,root)
%{_libdir}/libQtAddOnContacts.so.*
%{_libdir}/qt5/imports/QtAddOn/contacts
%{_libdir}/qt5/plugins/contacts/libqtcontacts_jsondb.so

%files -n libqt5organizer
%defattr(-,root,root)
%{_libdir}/libQtAddOnOrganizer.so.*
%{_libdir}/qt5/imports/QtAddOn/organizer
%{_libdir}/qt5/plugins/organizer/libqtorganizer_jsondb.so

%files -n libqt5versit
%defattr(-,root,root)
%{_libdir}/libQtAddOnVersit.so.*
%{_libdir}/qt5/plugins/versit/libqtversit_backuphandler.so
%{_libdir}/qt5/plugins/versit/libqtversit_vcardpreserver.so

%files -n libqt5versitorganizer
%defattr(-,root,root)
%{_libdir}/libQtAddOnVersitOrganizer.so.*

%files -n libqt5pim-devel
%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/*.so
%{_datadir}/qt5/mkspecs/*
%{_libdir}/pkgconfig/*

%changelog
