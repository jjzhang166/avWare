--[[*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  Ver: 1.0
|  Author：Knight [knight@graceport.cn]
|  Date:
|  description:此文件是用来表述各个硬件平台属性，请结合自身硬件据实填写
|
|  Ver:
|  Author:
|  Date:
|  description:
******************************************************************/--]]

--[[
OV9712,AR0130,MT9P006,AR0330,AR0331,IMX122,
IMX222,IMX322,OV4689,OV5658,IMX178,IMX290,
IMX291,IMX185,IMX123,BT1120,BT656,CVBS, 
USBCAMERA,SCREEN,AUTO
]]
sensor_modle = "AUTO"

--the path of sensor configs (ini file)
sensor_configs_path="/app/lib/sensor"

--the path of sensor lib.so
sensor_libs_path="/app/lib/sensor"

--[[
S2L22M;S2L33M;S2L55M;S2L65;S2L66
H18EV100;H18EV200;H18EV201;H18C;H18A;H16CV100;H16CV200
H16A;H16D;H19
H20D;H35;H36;H3798MV100;
WINDOWS_32;WINDOWS_64
LINUX_32;LINUX_64
MAC_32;MAC_64
--]]
cpu_model = "AUTO";

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

avWareGpioMapTable = {
	["avWare_HV001"] = {
										--[[   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12--]]
							["H18EV100"]	={-1, -1, -1, -1, -1, -1, -1, 0,   1, -1, -1, -1, -1},
							["H16CV100"]	={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
							["H16D"]		={-1, -1, 57, 55, -1, 56, -1, 53, 52, -1, -1, -1, -1},
							["H16A"]		={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
							["S2L33M"]		={24, 38, -1, 96, 97, 95, -1, 110, 109, -1, -1, -1, -1},
							["S2L66"]		={23, 9, -1, 19, 20, 17, -1, 7, 8, 33, 21, 18, -1},
							["H18EV200"]	={-1, 05, -1, -1, -1, -1, -1, 81, 80, -1, -1, -1, -1},
							["H18C"]		={-1,  2, -1, 3, -1,   4, -1,  0,  1, -1, -1, -1, -1},
							["H18A"]		={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
							["H36"]			={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
							["H35"]			={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
							["H3798MV100"]	={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
						},
	["avWare_HV002"] = {	
										--[[   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12--]]
							["H16D"]		={-1, -1, -1, -1, -1, -1, -1, 133, 132, -1, -1, -1, -1},
						},
}

avWareGpioMapTriggerTable = {
	["avWare_HV001"] = {
											--[[   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12--]]
							["H18EV100"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
							["H16CV100"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
							["H16D"]			={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
							["H16A"]			={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
							["S2L33M"]			={ 1,  1,  0,  1,  1,  1,  1,  0,  0,  1,  0,  0,  0},
							["S2L66"]			={ 1,  1,  0,  1,  1,  1,  1,  0,  0,  1,  1,  1,  0},
							["H18EV200"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
							["H18C"]			={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
							["H18A"]			={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},	
							["H36"]				={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
							["H35"]				={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
							["H3798MV100"]		={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
	
						},
	["avWare_HV002"] = {
											--[[   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12--]]
							["H16D"]			={ 0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
						},
}

function GetGpioMapNumber(hardversion, platform)
	return avWareGpioMapTable[hardversion][platform]
end

function GetGpioMapTrigger(hardversion, platform)
	return avWareGpioMapTriggerTable[hardversion][platform]
end


--[[
Sensor 触发
[0]:AgainMin 
[1]:AgainMax
[2]:Res
[3]:Res
--]]
-- TODO:为了让程序跑起来，IMX185值是临时定义,待修改
sensor_map_trigger = 
{
	["OV4689"] 		= {3000, 15000,  0, 0},
	["IMX290"] 		= {6000, 600000, 0, 0},
	["IMX291"] 		= {6000, 600000, 0, 0},
	["IMX122"] 		= {3000, 15000,  0, 0},
	["AR0130"] 		= {3000, 15000,  0, 0},
	["BT1120"] 		= {3000, 15000,  0, 0},
	["IMX123"] 		= {3000, 15000,  0, 0},
	["AR0230"] 		= {3000, 15000,  0, 0},
	["IMX185"] 		= {3000, 15000,  0, 0},
}

function GetSensorAgain(sensor)
	return sensor_map_trigger[sensor];
end


--[[Intelligent Analysis Capacity. 0 nonsupport, 1 support]]
intelligent_analysis_capacity =
{
	FaceDetect = 0,
	PlateRecognition = 0,
}

