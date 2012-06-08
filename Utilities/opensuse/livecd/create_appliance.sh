#!/bin/bash
# ============================================================================
# Script for creating appliances exported from SUSE Studio
# (http://susestudio.com) on your local system.
#
# Requires kiwi (http://opensuse.github.com/kiwi/).
#
# Author:  James Tan <jatan@suse.de>
# Contact: feedback@susestudio.com
# ============================================================================

# Recover backup files 
function recover_backup {
  for dir in 'source'; do
    if [ -f $dir/config.xml.bak ]; then
      cp $dir/config.xml.bak $dir/config.xml
      echo "  -> Recovered Backup $dir/config.xml.bak to $dir/config.xml"
    fi
  done
}

# Prints and runs the given command. Aborts if the command fails.
function run_cmd {
  command=$1
  echo $command
  $command
  if [ $? -ne 0 ]; then
    echo
    echo "** Appliance creation failed!"
    while true; do
      read -p "Recover config.xml files? [y/n] " yn
      case $yn in
        [Nn]* ) break;;
        [Yy]* ) recover_backup;break;;
     esac
    done
    exit 1
  fi
}

# Display usage.
function usage {
  echo >&2 "Usage:"
  echo >&2 "  create_appliance.sh"
}

# Check that we're root.
if [ `whoami` != 'root' ]; then
  echo "Please run this script as root."
  exit 1
fi

# Check that kiwi is installed.
kiwi=`which kiwi 2> /dev/null`
if [ $? -ne 0 ]; then
  echo "Kiwi is required but not found on your system."
  echo "Run the following command to install kiwi:"
  echo
  echo "  zypper install kiwi kiwi-tools kiwi-desc-* kiwi-doc"
  echo
  exit 1
fi

# Check kiwi version.
kiwi_ver='5.03.13'
installed_kiwi_ver=`rpm -q --whatprovides kiwi`
if [ "$installed_kiwi_ver" != "$kiwi_ver" ]; then
  echo "'$kiwi_ver' expected, but '$installed_kiwi_ver' found."
  while true; do
    read -p "Continue? [y/n] " yn
    case $yn in
      [Yy]* ) break;;
      [Nn]* ) exit;;
    esac
  done
fi


# Check system and it's version for 12.1 (it can't be built on SLES/SLED or
# earlier versions of openSUSE).
sys_name=`head -1 /etc/SuSE-release`
sys_ver=`grep VERSION /etc/SuSE-release | sed -e 's/^[^=]*= *//'`
if [ `echo "$sys_name" | grep -c openSUSE` -eq 0 -o `echo "$sys_ver < 12.1" | bc` -eq 1 ]; then
  echo "This appliance should be built on openSUSE 12.1 or newer (you use '$sys_name')."
  while true; do
    read -p "Continue? [y/n] " yn
    case $yn in
      [Yy]* ) break;;
      [Nn]* ) exit;;
    esac
  done
fi


# Check architecture (i686, x86_64).
image_arch='i686'
sys_arch=`uname -m`
linux32=`which linux32 2>/dev/null`
if [ "$image_arch" = 'i686' ] && [ "$sys_arch" = 'x86_64' ]; then
  if [ "$linux32" = '' ]; then
    echo "'linux32' is required but not found."
    exit 1
  else
    kiwi="$linux32 $kiwi"
  fi
elif [ "$image_arch" = 'x86_64' ] && [ "$sys_arch" = 'i686' ]; then
  echo "Cannot build $image_arch image on a $sys_arch machine."
  exit 1
fi

# Replace internal repositories in config.xml.
echo "** Checking for internal repositories..."
for repo in ; do
  # check if the repos are already replaced
  update_repo=0
  for dir in 'source'; do
    grep -q "{$repo}" $dir/config.xml && update_repo=1
  done

  if [ $update_repo -eq 1 ]; then
    # prompt for repo url
    read -p "Enter repository URL for '$repo': " url
    escaped_url=`echo "$url" | sed -e 's/\//\\\\\//g'`
    for dir in 'source'; do
      # backup config.xml first
      if [ ! -f $dir/config.xml.bak ]; then
        cp $dir/config.xml $dir/config.xml.bak
        echo "  -> Backed up $dir/config.xml to $dir/config.xml.bak"
      fi
      sed -i "s/{$repo}/$escaped_url/g" $dir/config.xml
    done
  fi
done

# Create appliance.
echo
echo "** Creating appliance..."
rm -rf build/root

run_cmd "$kiwi --build source/ -d image"

# And we're done!
image_file="image/gnorasilive.i686-0.1.2.iso"
qemu_options='-snapshot'
[[ "$image_file" =~ \.iso$ ]] && qemu_options='-cdrom'
echo
echo "** Appliance created successfully! ($image_file)"
echo "To boot the image using qemu-kvm, run the following command:"
echo "  qemu-kvm -m 512 $qemu_options $image_file &"
echo
