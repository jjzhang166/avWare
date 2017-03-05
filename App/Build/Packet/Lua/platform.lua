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
|  说明:此文件是用来表述各个硬件平台属性，请结合自身硬件据实填�
******************************************************************/--]]

--[[
OV9712,AR0130,MT9P006,AR0330,AR0331,IMX122,
IMX222,IMX322,OV4689,OV5658,IMX178,IMX290,
IMX291,IMX185,IMX123,BT1120,BT656,CVBS, 
USBCAMERA,SCREEN,AUTO
]]
sensor_modle = "USBCAMERA"

--the path of sensor configs (ini file)
sensor_configs_path="/app/configs/sensor"

--the path of sensor lib.so
sensor_libs_path="/app/configs/sensor"

--[[
S2L22M;S2L33M;S2L55M;S2L65;S2L66
H18EV100;H18EV200;H18EV201;H18C;H18A;H16CV100;H16CV200
H16A;H16D;H19
H20D;H35;H36
WINDOWS_32;WINDOWS_64
LINUX_32;LINUX_64
MAC_32;MAC_64
--]]
cpu_model = "WINDOWS_32"; 

--[[Video Out format  ["HDMI"; "CVBS"; "AUTO"]--]]
videoout_format = "HDMI";

videoencode_use_hardware = true;

--[[
[0]:System Reset
[1]:Sensor Reset
[2]:System Status Led
[3]:Alarm In1
[4]:Alarm In2
[5]:Alarm Out
[6]:LDR
[7]:IRC A
[8]:IRC B
[9]:IRLED
[10]:ReS
[11]:ReS
[12]:ReS
--]]
--H18E
gpio_map_number = 
{
				--[[   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12--]]
	["H18EV100"]	={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	["H16CV100"]	={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	["H16D"]		={-1, -1, 57, 55, -1, 56, -1, 53, 52, -1, -1, -1, -1},
	["H16A"]		={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	["S2L33M"]		={-1, -1, 57, 55, -1, 56, -1, 52, 53, -1, -1, -1, -1},
	["S2L66"]		={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	["H18EV200"]	={-1, 05, -1, -1, -1, -1, -1, 81, 80, -1, -1, -1, -1},
	["H18C"]		={-1,  2, -1, 3, -1,   4, -1,  0,  1, -1, -1, -1, -1},
	["H18A"]		={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	
}
gpio_map_trigger = 
{
			    --[[   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12--]]
	["H18EV100"]	={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
	["H16CV100"]	={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
	["H16D"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
	["H16A"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
	["S2L33M"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
	["S2L66"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
	["H18EV200"]	={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
	["H18C"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
	["H18A"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
}

function GetGpioMapNumber(platform)
	return gpio_map_number[platform];
end

function GetGpioMapTrigger(platform)
	return gpio_map_trigger[platform];
end


--[[
Sensor 触发
[0]:AgainMin 
[1]:AgainMax
[2]:Res
[3]:Res
--]]

sensor_map_trigger = 
{
	["OV4689"] 		= {3000, 15000,  0, 0},
	["IMX290"] 		= {6000, 600000, 0, 0},
	["IMX122"] 		= {3000, 15000,  0, 0},
	["AR0130"] 		= {3000, 15000,  0, 0},
	["BT1120"] 		= {3000, 15000,  0, 0},
}

function GetSensorAgain(sensor)
	return sensor_map_trigger[sensor];
end

