#!/bin/sh

echo "#====================================="
echo "# setting KDE4 desktop background     "
echo "#-------------------------------------"

if [ -d "/usr/share/kde4/" ] && [ -d "/etc/kde4/" ]; then
	# Background
	sed -i 's#name=.*#name=studio#g' /etc/kde4/share/config/plasmarc
	sed -i 's#name=.*#name=studio#g' /etc/kde4/share/config/plasma-desktoprc

	sed -i 's#wallpaper=/usr/share/wallpapers/.*#wallpaper=/usr/share/wallpapers/studio_wallpaper.jpg#g' /etc/kde4/share/config/plasmarc
	sed -i 's#wallpaper=/usr/share/wallpapers/.*#wallpaper=/usr/share/wallpapers/studio_wallpaper.jpg#g' /etc/kde4/share/config/plasma-desktoprc

	# SUSEgreeter background
	sed -i 's#background="/usr/share/kde4/apps/SUSEgreeter/background.png"#background="/usr/share/wallpapers/studio_wallpaper.jpg"#g' /usr/share/kde4/apps/SUSEgreeter/greet_header.html
fi
