#!/bin/bash
#================
# FILE          : config.sh
#----------------
# PROJECT       : OpenSuSE KIWI Image System
# COPYRIGHT     : (c) 2006 SUSE LINUX Products GmbH. All rights reserved
#               :
# AUTHOR        : Marcus Schaefer <ms@suse.de>
#               :
# BELONGS TO    : Operating System images
#               :
# DESCRIPTION   : configuration script for SUSE based
#               : operating systems
#               :
#               :
# STATUS        : BETA
#----------------
#======================================
# Functions...
#--------------------------------------
test -f /.kconfig && . /.kconfig
test -f /.profile && . /.profile

#======================================
# Greeting...
#--------------------------------------
echo "Configure image: [$name]..."

#======================================
# SuSEconfig
#--------------------------------------
echo "** Running suseConfig..."
suseConfig

echo "** Running ldconfig..."
/sbin/ldconfig

sed --in-place -e 's/icewm/icewm-session/' /usr/bin/wmlist

#======================================
# Firewall Configuration
#--------------------------------------
echo '** Configuring firewall...'
chkconfig SuSEfirewall2_init on
chkconfig SuSEfirewall2_setup on

#======================================
# RPM GPG Keys Configuration
#--------------------------------------
echo '** Importing GPG Keys...'
rpm --import /studio/studio_rpm_key_0
rm /studio/studio_rpm_key_0
rpm --import /studio/studio_rpm_key_1
rm /studio/studio_rpm_key_1
rpm --import /studio/studio_rpm_key_2
rm /studio/studio_rpm_key_2
rpm --import /studio/studio_rpm_key_3
rm /studio/studio_rpm_key_3
rpm --import /studio/studio_rpm_key_4
rm /studio/studio_rpm_key_4
rpm --import /studio/studio_rpm_key_5
rm /studio/studio_rpm_key_5
rpm --import /studio/studio_rpm_key_6
rm /studio/studio_rpm_key_6
rpm --import /studio/studio_rpm_key_7
rm /studio/studio_rpm_key_7
rpm --import /studio/studio_rpm_key_8
rm /studio/studio_rpm_key_8
rpm --import /studio/studio_rpm_key_9
rm /studio/studio_rpm_key_9
rpm --import /studio/studio_rpm_key_10
rm /studio/studio_rpm_key_10
rpm --import /studio/studio_rpm_key_11
rm /studio/studio_rpm_key_11

sed --in-place -e 's/# solver.onlyRequires.*/solver.onlyRequires = true/' /etc/zypp/zypp.conf

# Enable sshd
chkconfig sshd on
test -d /studio || mkdir /studio
cp /image/.profile /studio/profile
cp /image/config.xml /studio/config.xml
rm -rf /studio/overlay-tmp
true

sh /studio/configure_kdm4_theme.sh



sh /studio/configure_kde4_background.sh

