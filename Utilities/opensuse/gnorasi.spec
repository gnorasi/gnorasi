#
# spec file for package dlib
#

# norootforbuild

%define _datadir /usr/share
Name:           gnorasi
Version:        0.1.0
Release:        1
Summary:        Gnorasi is a knowledge-based remote sensing processing engine
Group:          Development/Libraries/C and C++
License:        GPL
URL:            http://www.gnorasi.gr
Source0:        %{name}-%{version}.tar.gz
Source1:	gnorasi.desktop
Source2:	gnorasi.png
Patch1:		VRE30_opensuse_OBS.patch
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
# BuildArch:      noarch

BuildRequires:  qt-creator libtiff-devel  gcc-c++ gcc zlib-devel DevIL-devel
BuildRequires:  fdupes python-devel libqt4-devel OrfeoToolbox-devel
BuildRequires:  glew-devel >= 1.6.0

%description
Gnorasi aims to advance the current state of the art in research and development of knowledge-based image interpretation applications by providing a generic, modular, platform for the systematic coupling of image processing algorithms and formal knowledge.

%package        devel
Summary:        Development files for Gnorasi
Group:          Development/Libraries/C and C++

%description    devel
Gnorasi is a knowledge-based remote sensing processing engine


%prep
%setup
%patch1


%build
%if "%{_lib}" == "lib"
sed -i 's:/usr/lib64/otb:/usr/lib/otb:' config.txt
%endif
export SUSE_ASNEEDED=0
qmake voreen.pro
make

%install
mkdir -p %{buildroot}/usr/%{_lib}
mkdir -p %{buildroot}/usr/include
mkdir -p %{buildroot}/usr/bin
install -m 755 ./bin/*.so* %{buildroot}/usr/%{_lib}/
install -m 755 ./bin/gnorasi %{buildroot}/usr/bin/gnorasi
mv ./include/voreen %{buildroot}/usr/include/
install -d $RPM_BUILD_ROOT%{_datadir}/applications
install -m 644 %SOURCE1  $RPM_BUILD_ROOT%{_datadir}/applications/gnorasi.desktop
install -d $RPM_BUILD_ROOT%{_datadir}/icons
install -m 644 %SOURCE2  $RPM_BUILD_ROOT%{_datadir}/icons/gnorasi.png
#%%makeinstall
%fdupes %{buildroot}

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_libdir}/lib*.so.*
%{_datadir}/applications/gnorasi.desktop
%{_datadir}/icons/gnorasi.png

%files devel
%defattr(-,root,root,-)
%{_libdir}/*.so
%{_includedir}/voreen/

%changelog
