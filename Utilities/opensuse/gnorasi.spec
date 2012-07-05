#
# spec file for package dlib
#

# norootforbuild

%define _datadir /usr/share
Name:           gnorasi
Version:        0.1.8
Release:        1
Summary:        Gnorasi is a knowledge-based remote sensing processing engine
Group:          Development/Libraries/C and C++
License:        GPL
URL:            http://www.gnorasi.gr
Source0:        %{name}-%{version}.tar.gz
Source1:	gnorasi.desktop
Source2:	gnorasi.png
Source3:        %{name}.rpmlintrc
#Patch1:		VRE30_opensuse_OBS.patch
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
# BuildArch:      noarch

BuildRequires:  gcc-c++ gcc cmake boost-devel
BuildRequires:  fdupes libqt4-devel OrfeoToolbox-devel 
BuildRequires:  glew-devel >= 1.6.0

%description
Gnorasi aims to advance the current state of the art in research and development of knowledge-based image interpretation applications by providing a generic, modular, platform for the systematic coupling of image processing algorithms and formal knowledge.

#%package        devel
#Summary:        Development files for Gnorasi
#Group:          Development/Libraries/C and C++

#%description    devel
#Gnorasi is a knowledge-based remote sensing processing engine


%prep
%setup
##%patch1


%build
%if "%{_lib}" == "lib"
sed -i 's:/usr/lib64/otb:/usr/lib/otb:' ./modules/otb/otb.cmake
%endif
export SUSE_ASNEEDED=0
mkdir build
cd build

cmake  -DVRN_BUILD_GNORASI:BOOL=ON \
       -DVRN_BUILD_LIB_TGTGNORASI:BOOL=ON \
       -DVRN_BUILD_LIB_VOREENCORE:BOOL=ON \
       -DVRN_BUILD_LIB_VOREENQT:BOOL=ON \
       -DVRN_DEBUG:BOOL=OFF \
       -DVRN_MODULE_OTB:BOOL=ON \
       -DVRN_PRECOMPILED_HEADER:BOOL=OFF \
       -DVRN_SHARED_LIBS:BOOL=ON \
       -DCMAKE_INSTALL_PREFIX:PATH=/usr \
       -DCMAKE_SKIP_RPATH:BOOL=ON \
       -DCMAKE_BUILD_TYPE:STRING="Release" ../

make VERBOSE=1 %{?_smp_mflags}

%install

mkdir -p %{buildroot}/usr/%{_lib}
mkdir -p %{buildroot}/usr/include
mkdir -p %{buildroot}/usr/bin
mkdir -p %{buildroot}%{_datadir}/gnorasi
install -m 755 ./bin/*.so* %{buildroot}/usr/%{_lib}/
install -m 755 ./bin/gnorasi %{buildroot}/usr/bin/gnorasi
#mv ./include/voreen %{buildroot}/usr/include/
install -d $RPM_BUILD_ROOT%{_datadir}/applications
install -m 644 %SOURCE1  $RPM_BUILD_ROOT%{_datadir}/applications/gnorasi.desktop
install -d $RPM_BUILD_ROOT%{_datadir}/icons
install -m 644 %SOURCE2  $RPM_BUILD_ROOT%{_datadir}/icons/gnorasi.png

cat > %{buildroot}%{_datadir}/gnorasi/standard.vws << EOF
<?xml version="1.0" ?>
<VoreenData version="1">
    <Workspace version="2" GlobalDescription="">
        <ProcessorNetwork version="12">
            <Processors />
            <Connections />
            <CoProcessorConnections />
            <PropertyLinks />
        </ProcessorNetwork>
        <Animation />
    </Workspace>
</VoreenData>
EOF

#%%makeinstall
%fdupes %{buildroot}

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_libdir}/*
%{_datadir}/applications/gnorasi.desktop
%{_datadir}/icons/gnorasi.png
%{_datadir}/gnorasi/standard.vws

#%files devel
#%defattr(-,root,root,-)
#%{_includedir}/voreen/

%changelog
