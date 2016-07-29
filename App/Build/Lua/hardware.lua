--[[*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:此文件是用来表述各个硬件平台属性，请结合自身硬件据实填写
******************************************************************/--]]

--ar0230,ar0237,ar0330,,ov4689,ov5658,imx123,imx178,imx185,imx117
sensor_modle = "ar0130"

--the path of sensor configs (ini file)
sensor_configs_path="configs/sensor"

--the path of sensor lib.so
sensor_libs_path="configs/sensor"

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
--]]
--H18E
array_gpio = {-1, 1, -1, 51, -1, 0, 3, 4, -1}
--H16C
--array_gpio = {-1, 1, -1, 51, -1, 0, 3, 4, -1}
--H16D
--array_gpio = {-1, -1, -1, -1, -1, 57, -1, -1, -1}
--H16A
--array_gpio = {-1, -1, -1, -1, -1, 57, -1, -1, -1}


--[[
S2L22M
S2L33M
S2L55M
S2L65
S2L66

H18EV100
H18EV200
H18EV201
H18C
H18A
H16CV100
H16CV200
H16A
H16D
H19

H20D
H35
H36

WIN32
WIN64

LIN32
LIN64
--]]
cpu_model = "H18EV100" 