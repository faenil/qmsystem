# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.25
# 

Name:       qmsystem

# >> macros
# << macros

Summary:    QmSystem library
Version:    1.4.6
Release:    1
Group:      System/System Control
License:    LGPLv2
URL:        http://github.com/nemomobile/qmsystem
Source0:    %{name}-%{version}.tar.bz2
Source100:  qmsystem.yaml
Requires:   sensorfw >= 0.6.33
Requires:   timed >= 2.31
Requires:   mce
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(mce)
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(dsme)
BuildRequires:  pkgconfig(dsme_dbus_if)
BuildRequires:  pkgconfig(gconf-2.0)
BuildRequires:  pkgconfig(libiphb) >= 0.61.29
BuildRequires:  pkgconfig(QtCore) >= 4.5
BuildRequires:  pkgconfig(sensorfw) >= 0.6.33
BuildRequires:  pkgconfig(timed) >= 2.31
BuildRequires:  fdupes

%description
This package contains the QmSystem library.

%package tests
Summary:    Unit test cases and xml test description for libqmsystem2 library
Group:      Development/System
Requires:   %{name} = %{version}-%{release}
Requires:   testrunner-lite

%description tests
%{summary}.

%package devel
Summary:    Development headers for QmSystem library
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Development headers for compiling applications against the QmSystem library.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
%qmake
make
# << build pre



# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre

# >> install post
%make_install
%fdupes %{buildroot}/%{_docdir}/qmsystem2/html/
# << install post


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
# >> files
%doc debian/copyright
%{_libdir}/libqmsystem2.so.*
%{_sbindir}/qmkeyd2
# From doc
# %doc debian/changelog debian/copyright
# %{_docdir}/qmsystem2/html/*
# << files

%files tests
%defattr(-,root,root,-)
# >> files tests
%doc debian/copyright
/opt/tests/qmsystem-tests/*-test
%{_datadir}/%{name}-tests/tests.xml
# << files tests

%files devel
%defattr(-,root,root,-)
# >> files devel
%doc debian/copyright
%{_includedir}/qmsystem2/*.h
%{_libdir}/libqmsystem2.so
%{_libdir}/pkgconfig/qmsystem2.pc
%{_datadir}/qt4/mkspecs/features/qmsystem2.prf
# << files devel
