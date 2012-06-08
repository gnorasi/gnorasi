#!/bin/sh

echo "#====================================="
echo "# Setting up KDM4 theme               "
echo "#-------------------------------------"

# Enable theme
sed -i 's/DISPLAYMANAGER_KDM_THEME=.*/DISPLAYMANAGER_KDM_THEME=\"studio\"/g' /etc/sysconfig/displaymanager

if [ -d "/usr/share/kde4/" ] && [ -d "/etc/kde4/" ]; then
	# Use neutral ksplashx theme
	config="/usr/share/kde4/config/ksplashrc"
	echo "[KSplash]" >> $config
	echo "Engine=Simple" >> $config
	echo "Theme=SimpleSmall" >> $config

	sed -i 's/^Theme=.*/Theme=SimpleSmall/g' /etc/kde4/share/config/ksplashrc
	sed -i 's/^Engine=.*/Engine=Simple/g' /etc/kde4/share/config/ksplashrc

	cat <<-EOT >/etc/kde4/share/config/startupconfigkeys
		kcminputrc Mouse cursorTheme 'Oxygen_Black'
		kcminputrc Mouse cursorSize ''
		ksplashrc KSplash Theme SimpleSmall
		ksplashrc KSplash Engine Simple
		kcmrandrrc Display ApplyOnStartup true
		kcmrandrrc [Screen0]
		kcmrandrrc [Screen1]
		kcmrandrrc [Screen2]
		kcmrandrrc [Screen3]
		kcmfonts General forceFontDPI 0
		kdeglobals Locale Language '' # trigger requesting languages from KLocale
	EOT

	# Fix up greeter background
	sed -i 's/^Wallpaper=.*/Wallpaper=studio_wallpaper.jpg/g' /usr/share/kde4/config/kdm/backgroundrc
	sed -i 's/^Wallpaper=.*/Wallpaper=studio_wallpaper.jpg/g' /etc/kde4/share/config/kdm/backgroundrc

	# Enable greeter theme
	sed -i 's/^#UseTheme=.*/UseTheme=true/g' /usr/share/kde4/config/kdm/kdmrc
	sed -i 's#^\#Theme=.*#Theme=/usr/share/kde4/apps/kdm/themes/studio#g' /usr/share/kde4/config/kdm/kdmrc
fi
