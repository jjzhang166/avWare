#!/bin/sh
echo "Run Start"



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
	
	if [ ! -d "/app/ko" ]; then
		echo "Have No Ko dir";
		#return ;
	else
		cd /app/ko/;
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
		report_error
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
			;;
		*)
		
		echo "xxxx Invalid platform type $platform xxxx"
		cd -;
		report_error
		;;
	esac
	
	
	cd -;
	
	
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

	if [ -f /app/avWare.tar.gz ]
	then
		tar -xvf /app/avWare.tar.gz -C /tmp;
		cd /tmp;
		ulimit -s 4096;
		./avWare &
		sleep 1;
		rm -rf /tmp/avWare;
		echo 1 > /proc/sys/vm/drop_caches;
		echo 2 > /proc/sys/vm/drop_caches;
		echo 3 > /proc/sys/vm/drop_caches;
	else
		cd /app
		./avWare &
	fi

}


start_env()
{
	export PATH="/app:/app/bin:$PATH"
	export LD_LIBRARY_PATH="/app/lib:$LD_LIBRARY_PATH"
	#ln -s /app/Lua/platform.lua /app/Lua/hardware.lua;	
}

start_init()
{

	echo "start_init "
}

if [ $# ==  0 ];then
	start_init;
	return;
fi


cd /app/
start_env;
start_ko $1 $2;
system_conf;
start_app;


#.
