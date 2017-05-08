#!/bin/sh
echo "Run Start"

avWareHomeDir=/app;

report_error()
{
	echo "******* Error: There's something wrong, please check! *****"
	exit 1
}


start_ko()
{
	local platform=$1;
	local sensor=$2;
	local script_ko;
	local sensorsns;
	
	if [ ! -d "$avWareHomeDir/ko" ]; then
		echo "Have No Ko dir";
	else
		cd $avWareHomeDir/ko;
	fi
	
	case $sensor in
		OV9712|ov9712)
			sensorsns=ov9712;
		;;
		OV9732|ov9732)
			sensorsns=ov9732;
		;;
		OV4689|ov4689)
			sensorsns=ov4689;
		;;
		OV5658|ov5658)
			sensorsns=ov5658;
		;;
		AR0130|ar0130)
			sensorsns=ar0130;
		;;
		AR0230|ar0230)
			sensorsns=ar0230;
		;;
		AR0330|ar0330)
			sensorsns=ar0330;
		;;
		AR0237|ar0237)
			sensorsns=ar0237;
		;;
		IMX138|imx138)
			sensorsns=imx138;
		;;
		IMX122|imx122)
			sensorsns=imx122;
		;;
		IMX236|imx236)
			sensorsns=imx236;
		;;
		IMX178|imx178)
			sensorsns=imx178;
		;;
		IMX185|imx185)
			sensorsns=imx185;
		;;
		IMX117|imx117)
			sensorsns=imx117;
		;;
		IMX123|imx123)
			sensorsns=imx123;
		;;
		IMX290|imx290)
			sensorsns=imx290;
		;;
		IMX225|imx225)
			sensorsns=imx225;
		;;
		BT1120|bt1120)
			sensorsns=bt1120;
		;;
		BT656|bt656)
			sensorsns=bt656;
		;;
		IMX291|imx291)
			sensorsns=imx290;
		;;
		AUTO|auto)
			echo "XXXXXX Have No sensor Var"
			;;
		*)
		echo "xxxx Invalid sensor type $sensor xxxx"
		;;
	esac
	
		case $platform in
		S2L22M|s2l22m)
			;;
		S2L33M|s2l33m)
        mount -t jffs2 /dev/mtdblock7 /font ;
        /bin/ln -s /font/locale /usr/lib ;
        /bin/ln -s /font/fonts /usr/share ;
        /usr/local/bin/init.sh --$sensorsns ;
        modprobe spidev ;
			;;
		S2L55M|s2l55m)
			;;
		S2L65|s2l65)
			;;
		S2L66|s2l66)
        modprobe spi-ambarella ;
        /usr/local/bin/init.sh --$sensorsns --s2l_6b;
        mount -t jffs2 /dev/mtdblock7 /font;
			;;
		H18EV100|h18ev100)
		./load3518ev100 -i $sensorsns
			;;
		H18EV200|h18ev200)
		./load3518ev200 -a -sensor $sensorsns -osmem 36 -total 64 -offline
			;;
		H18EV201|h18ev201)
		./load3518ev201 -i $sensorsns;
			;;
		H18C|h18c)
		./load3518c -i $sensorsns;
			;;
		H18A|h18a)
		./load3518a -i $sensorsns;
			;;
		H16CV100|h16cv100)
		./load3516cv100 -i $sensorsns;
			;;
		H16CV200|h16cv200)
		./load3516cv200 -i $sensorsns;
			;;
		H16A|h16a)
		script_ko = load3516a;
		./load3516cv200 -i $sensorsns;
			;;
		H16D|h16d)
		./load3516d -a -sensor $sensorsns -osmem 96 -offline
			;;
		H19|h19)
		script_ko = load3519;
			;;
		H20D|h20d)
		script_ko = load3520d;
			;;
		H35|h35)
		script_ko = load3535;
			;;
		H36|h36)
		./load3536 -i
			;;
		H3798MV100)
		./load
		;;
		WIN32|win32)
		script_ko = loadwin32;
			;;
		WIN64|win64)
		script_ko = loadwin64;
			;;		
		LIN32|lin32)
		script_ko = loadlin32;
			;;		
		LIN64|lin64)
		script_ko = loadlin64;
			;;
		AUTO|auto)
			echo "XXXXXX Have No Platform Var"
			if [ -x loaddefault.sh ];then
				./loaddefault.sh
			fi
			;;
		*)
		
		echo "xxxx Invalid platform type $platform xxxx"
		;;
	esac
	
	
	cd $avWareHomeDir;
	
	
}

system_conf()
{
	echo 3 > /proc/sys/vm/drop_caches
	sysctl -w net.ipv4.tcp_keepalive_time=3;
	sysctl -w net.ipv4.tcp_keepalive_probes=2;
	sysctl -w net.ipv4.tcp_keepalive_intvl=2;
}

start_app()
{

	if [ -f $avWareHomeDir/avWare.tar.gz ]
	then
		tar -xvf $avWareHomeDir/avWare.tar.gz -C /tmp;
		cd /tmp;
		ulimit -s 4096;
		./avWare &
		sleep 1;
		rm -rf /tmp/avWare;
		echo 1 > /proc/sys/vm/drop_caches;
		echo 2 > /proc/sys/vm/drop_caches;
		echo 3 > /proc/sys/vm/drop_caches;
	else
		cd $avWareHomeDir
		./avWare &
	fi

}

start_wifi()
{
	if [ -f /app/wifi.tar.bz2 ];then
		tar -xvf /app/wifi.tar.bz2 -C /tmp;
		ln -s /tmp/wifi/firmware/wlan/ /lib/firmware/wlan
		ln -s /tmp/wifi/firmware/athwlan.bin /lib/firmware/athwlan.bin
		ln -s /tmp/wifi/firmware/otp.bin /lib/firmware/otp.bin
		ln -s /tmp/wifi/firmware/otp30.bin /lib/firmware/otp30.bin
		ln -s /tmp/wifi/firmware/qca61x4.bin /lib/firmware/qca61x4.bin
		ln -s /tmp/wifi/firmware/utf.bin /lib/firmware/utf.bin
		ln -s /tmp/wifi/firmware/utf30.bin /lib/firmware/utf30.bin
		ln -s /tmp/wifi/firmware/fakeboar.bin /lib/firmware/fakeboar.bin
		ln -s /tmp/wifi/firmware/qwlan30.bin /lib/firmware/qwlan30.bin
		echo host > /proc/ambarella/usbphy0
		insmod /lib/modules/3.10.73/kernel/net/rfkill/rfkill.ko
		insmod /tmp/wifi/compat.ko
		insmod /tmp/wifi/cfg80211.ko
		insmod /tmp/wifi/wlan.ko		
		ifconfig wlan0 up

		if [ -f /appex/hostapd.conf ];then
			ifconfig wlan0 192.168.99.1
			ln -s /tmp/wifi/udhcpd.conf /etc/udhcpd.conf
			udhcpd /etc/udhcpd.conf
			#/tmp/wifi/hostapd /appex/hostapd.conf &
		fi
	fi
}

start_env()
{
	local platform=$1;

	
		case $platform in
		S2L22M|s2l22m)
		;;
		S2L33M|s2l33m)
		/appex/CloseWatchdog;
		;;
		S2L55M|s2l55m)
		;;
		S2L65|s2l65)
		;;
		S2L66|s2l66)
		/appex/CloseWatchdog;
		;;
		H18EV100|h18ev100)
		;;
		H18EV200|h18ev200)
		;;
		H18EV201|h18ev201)
		;;
		H18C|h18c)
		;;
		H18A|h18a)
		;;
		H16CV100|h16cv100)
		;;
		H16CV200|h16cv200)
		;;
		H16D|h16d)
		;;
		H19|h19)
		;;
		H20D|h20d)
			;;
		H35|h35)
			;;
		H36|h36)
			avWareHomeDir=/tmp
			tar -xvf /app/avWare.tar.gz -C $avWareHomeDir;
			export QT_QPA_FONTDIR="/usr/lib/fonts"
			export QT_QPA_PLATFORM_PLUGIN_PATH="/usr/lib/plugins"
			export QT_QPA_PLATFORM="linuxfb:/dev/fb0"

			;;
		H3798MV100)
			export QT_QPA_FONTDIR="/usr/lib/fonts"
			export QT_QPA_PLATFORM_PLUGIN_PATH="/usr/lib/plugins"
			export QT_QPA_PLATFORM="linuxfb:/dev/fb0"

			;;
		WIN32|win32)
			;;
		WIN64|win64)
			;;		
		LIN32|lin32)
			;;		
		LIN64|lin64)
			;;
		AUTO|auto)
			##only for nvr platform (have no factory info)
			export QT_QPA_FONTDIR="/usr/lib/fonts"
			export QT_QPA_PLATFORM_PLUGIN_PATH="/usr/lib/plugins"
			export QT_QPA_PLATFORM="linuxfb:/dev/fb0"
			echo "XXXXXX Have No Platform Var"
			;;
		*)
		
		echo "xxxx Invalid platform type $platform xxxx"
		report_error
		;;
	esac
	cd $avWareHomeDir
	export PATH="$avWareHomeDir:$avWareHomeDir/bin:$PATH"
	export LD_LIBRARY_PATH="$avWareHomeDir/lib:/lib:/usr/lib:$LD_LIBRARY_PATH"

	
	echo ""
	echo "#########avWare Env########################"
	env
	echo "###########################################"
	echo ""
}

start_init()
{

	echo "start_init "
}

if [ $# ==  0 ];then
	start_init;
	return;
fi



start_env $1;
start_ko $1 $2;
system_conf;
start_wifi;
start_app;

