TEMPLATE = subdirs

# Default projects
VRN_PROJECTS = tgt core qt voreenve

!exists(config.txt) {
  error("config.txt not found! copy config-default.txt to config.txt and edit!")
}
include(config.txt)

contains(VRN_PROJECTS, tgt):      SUBDIRS += sub_tgt
contains(VRN_PROJECTS, core):     SUBDIRS += sub_core
contains(VRN_PROJECTS, qt):       SUBDIRS += sub_qt
contains(VRN_PROJECTS, voreenve): SUBDIRS += sub_voreenve
contains(VRN_PROJECTS, voltool):  SUBDIRS += sub_voltool
contains(VRN_PROJECTS, simple-qt):  SUBDIRS += sub_simple-qt
contains(VRN_PROJECTS, simple-glut):  SUBDIRS += sub_simple-glut
contains(VRN_PROJECTS, simple-memcheck):  SUBDIRS += sub_simple-memcheck
contains(VRN_PROJECTS, simple-memcheckinit):  SUBDIRS += sub_simple-memcheckinit
contains(VRN_PROJECTS, serializertest):  SUBDIRS += sub_serializertest
contains(VRN_PROJECTS, voreenblastest):  SUBDIRS += sub_voreenblastest
contains(VRN_PROJECTS, itk_wrapper):  SUBDIRS += sub_itk_wrapper
contains(VRN_PROJECTS, descriptiontest):  SUBDIRS += sub_descriptiontest
contains(VRN_PROJECTS, coveragetest):  SUBDIRS += sub_coveragetest
contains(VRN_PROJECTS, varianttest): SUBDIRS += sub_varianttest

sub_tgt.file = ext/tgt/tgt.pro

sub_core.file = src/core/voreenlib_core.pro
sub_core.depends = sub_tgt

sub_qt.file = src/qt/voreenlib_qt.pro
sub_qt.depends = sub_tgt sub_core

sub_voreenve.file = apps/voreenve/voreenve.pro
sub_voreenve.depends = sub_tgt sub_core sub_qt

sub_voltool.file = apps/voltool/voltool.pro
sub_voltool.depends = sub_tgt sub_core

sub_simple-qt.file = apps/simple/simple-qt.pro
sub_simple-qt.depends = sub_tgt sub_core sub_qt

sub_simple-glut.file = apps/simple/simple-glut.pro
sub_simple-glut.depends = sub_tgt sub_core

sub_simple-memcheck.file = apps/tests/simple-memcheck.pro
sub_simple-memcheck.depends = sub_tgt sub_core

sub_simple-memcheckinit.file = apps/tests/simple-memcheckinit.pro
sub_simple-memcheckinit.depends = sub_tgt sub_core

sub_serializertest.file = apps/tests/serializertest/serializertest.pro
sub_serializertest.depends = sub_tgt sub_core

sub_voreenblastest.file = apps/tests/voreenblastest/voreenblastest.pro
sub_voreenblastest.depends = sub_tgt  sub_core

sub_descriptiontest.file = apps/tests/descriptiontest/descriptiontest.pro
sub_descriptiontest.depends = sub_tgt  sub_core

sub_coveragetest.file = apps/tests/coveragetest/coveragetest.pro
sub_coveragetest.depends = sub_tgt  sub_core

sub_itk_wrapper.file = apps/itk_wrapper/itk_wrapper.pro
sub_itk_wrapper.depends =sub_tgt  sub_core

sub_varianttest.file = apps/tests/varianttest/varianttest.pro
sub_varianttest.depends = sub_tgt sub_core

unix {
  # update browser file for the emacs class hierarchy browser
  ebrowse.target = ebrowse
  EBROWSE_DIRS=include src apps/voreenve apps/voltool ext/tgt
  ebrowse.commands = @echo \"Updating ebrowse file...\"; \
                     (find $$EBROWSE_DIRS -name \"*.h\"; \
                      find $$EBROWSE_DIRS -not -name \"moc_*\" -and -name \"*.cpp\") | \
                      ebrowse --output-file=BROWSE
  QMAKE_EXTRA_TARGETS += ebrowse

  # clean up old Makefiles also
  mrproper.target = mrproper
  MRPROPER_DIRS=src/core src/qt modules apps/*
  mrproper.commands = @echo \"Removing Makefiles and module registration file...\"; \
                            find $$MRPROPER_DIRS -name \"Makefile*\" -or -name "Makefile" -or -name "gen_moduleregistration.h" | \
                            xargs /bin/rm -fv
  QMAKE_EXTRA_TARGETS += mrproper

}

### Local Variables:
### mode:conf-unix
### End: