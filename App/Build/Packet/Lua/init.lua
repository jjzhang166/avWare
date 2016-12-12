--[[*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作�? Knight [knight@graceport.cn]
|  日期: 2016�?�?�
|  说明:
|
|  版本: 1.1
|  作�?
|  日期:
|  说明:此文件是用来表述AvWare 属性�
******************************************************************/--]]

avWare_default_hostname = "avWare";
avWare_default_ipaddr 	= "192.168.1.90";
avWare_default_netmask 	= "255.255.255.0";
avWare_default_gateway 	= "192.168.1.1";
avWare_default_dns1		= "8.8.8.8";
avWare_default_dns2		= "8.8.4.4";


avWare_MemPool_bOpen 	= true;
avWare_MemPool_Size		= 8192;


MapPlatformConfigs = 
{
	["H18EV100"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H18EV200"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H18EV201"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H18C"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H18A"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H16CV100"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H16CV200"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H16CV300"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H16A"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H16D"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H19"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["HIPC_RES_1"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["HIPC_RES_2"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H31"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H21"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H35"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["H36"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["HNVR_RES_1"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["HNVR_RES_2"] 		= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["A22M"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["A33M"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["A55M"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["A65"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["A66"] 			= {"/mnt/AvRecord", "/appex/configs",  "/app/webroot", "login.html"},
	["WINDOWS_32"] 		= {"D:\\AvRecord",  "./configs",  	   "./webroot",    "login.html"},
	["WINDOWS_64"] 		= {"D:\\AvRecord",  "./configs",  	   "./webroot",    "login.html"},
	["LINUX_32"] 		= {"/mnt/AvRecord", "./configs", 	   "./webroot",    "login.html"},
	["LINUX_64"] 		= {"/mnt/AvRecord", "./configs",       "./webroot",    "login.html"},
	["MAC_32"] 			= {"/mnt/AvRecord", "./configs",       "./webroot",    "login.html"},
	["MAC_64"] 			= {"/mnt/AvRecord", "./configs",       "./webroot",    "login.html"},
}

function avWarePlatformConfigs(Platform)
	return MapPlatformConfigs[Platform];
end
