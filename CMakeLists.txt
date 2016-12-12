#Top Dir
cmake_minimum_required(VERSION 3.1)

#set (_AV_WARE_VERSION_MAJOR 1)
#set (_AV_WARE_VERSION_MINOR 1)
#set (_AV_WARE_VERSION_PATCH 3)
#set (_AV_WARE_VERSION_OEM Gp)
#set (_AV_WARE_VERSION_RUNTIME RC) #base、alpha、beta 、RC 、 release(R)


include("CMakeConfig.cmake")
set(SYS_CHN_NUM "64" CACHE STRING "define system Channels")
option (CMAKE_WITH_UI "Build avWare With Ui On Qt5" OFF)
option (HAVE_MemPool "use memory pool for stream buffer " OFF)
option (UI_CLOSE_CAPUTRE "Close Caputre" OFF)
if (WIN32)
	#set (COMPILER PC_WIN32)
	#set (COMPILER PC_LINUX)
	#set (COMPILER AMBA)
	#set (COMPILER HISI_16C_18ACE_V100)
	#set (COMPILER HISI_16C_18E_V200)
	#set (COMPILER HISI_16AD)
	#set (COMPILER HISI_3535)
	
	option (_AV_WARE_M_HAVE_PROC "use proc" OFF)
	if (COMPILER MATCHES "PC_WIN32")
		set (CMAKE_QT_PATH ${QT_WIN32_PATH})
	elseif (COMPILER MATCHES "PC_WIN64")
		set (CMAKE_QT_PATH ${QT_WIN64_PATH})
	else()
	endif()
	
elseif (UNIX)
	option (_AV_WARE_M_HAVE_PROC "use proc" ON)
	#COMPILER
	#set (COMPILER PC_LINUX)
	#set (COMPILER AMBA)
	#set (COMPILER HISI_16C_18ACE_V100)
	#set (COMPILER HISI_16C_18E_V200)
	#set (COMPILER HISI_16AD)
	#set (COMPILER HISI_3535)
	
	set(CMAKE_SYSTEM_NAME Linux)
	if (COMPILER MATCHES "HISI_16C_18ACE_V100")
		set(SYS_CHN_NUM "1")
		option (HAVE_SensorInput "use ffmpeg for encodeing " ON)
		set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")
	elseif (COMPILER MATCHES "HISI_16AD")
		set(SYS_CHN_NUM "1")
		set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")
	elseif (COMPILER MATCHES "HISI_16C_18E_V200")
		set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")
		add_definitions("-DCHIP_ID=0x3518E200")
	elseif (COMPILER MATCHES "HISI_3535")
		set(SYS_CHN_NUM "32")
		set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")
	elseif (COMPILER MATCHES "AMBA")
		set(SYS_CHN_NUM "1")
		set (CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
		set (CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
	elseif (COMPILER MATCHES "LINUX")

	endif()
endif()



set(CMAKE_SUPPRESS_REGENERATION 1)
set(CMAKE_USE_RELATIVE_PATHS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
set(PROJDIR ${CMAKE_CURRENT_SOURCE_DIR})
set (AVWARE_HOME_DIR ${PROJDIR})
set (CMAKE_CONFIGURATION_TYPES Debug Release)


project (avWare)

if (WIN32)
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
	add_definitions("-D_WINSOCK_DEPRECATED_NO_WARNINGS")
	message("${COMPILER}")
	if (COMPILER MATCHES "PC_WIN32")
		add_definitions("-D_WIN32")
	elseif (COMPILER MATCHES "PC_WIN64")
		add_definitions("-D_WIN64")
	else()
	endif()
	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /openmp")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /openmp")
	set(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO /MANIFEST:NO")
	
elseif (UNIX)
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		message("*****Use Build Type ${CMAKE_BUILD_TYPE}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O2 -pipe -fPIC -Wall -MMD -fno-strict-aliasing -fmessage-length=0")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -O2 -pipe -fPIC -Wall -MMD -fno-strict-aliasing -fmessage-length=0")
	elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
		message("*****Use Build Type ${CMAKE_BUILD_TYPE}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -ggdb -pipe -fPIC -Wall -MMD -fno-strict-aliasing")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -ggdb -pipe -fPIC -Wall -MMD -fno-strict-aliasing")
	
		if (COMPILER MATCHES LINUX)  
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
		endif()
	else()
		message(FATAL_ERROR "Please use cmake -DCMAKE_BUILD_TYPE=Debug(Release) dir current Type[${CMAKE_BUILD_TYPE}]")
	endif()
endif()
	
add_definitions("-D_AV_WARE_")

if (CMAKE_WITH_UI STREQUAL "ON")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
	set(_AV_WARE_M_HAVE_UI "ON")
	
	if (UI_CLOSE_CAPUTRE STREQUAL "ON")
		set (_AV_WARE_M_HAVE_UI_CLOSE_CAPUTRE "ON")
	else()
		set (_AV_WARE_M_HAVE_UI_CLOSE_CAPUTRE "OFF")
	endif()
	#With_ui for cmakefiles
	#_AV_WARE_HAVE_UI for avware code;
endif()

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

include_directories(${PROJECT_BINARY_DIR})

if (COMPILER MATCHES "PC_LIN32" OR COMPILER MATCHES "PC_LIN64" OR COMPILER MATCHES "PC_WIN32" OR COMPILER MATCHES "PC_WIN64")
#	add_subdirectory(BspPlatform/PC)
else()
#	add_subdirectory(BspPlatform/${COMPILER})
endif()

add_subdirectory(App)



###record for knight
#set(TOOLCHAIN_DIR "/home/ProgamApp/ToolChain/hisi-linux-nptl/arm-hisiv300-linux/target")
#set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR})
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/arm-hisiv300-linux-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-hisiv300-linux-g++)
#file(DOWNLOAD "http://graceport.tpddns.cn:880/bg.jpg" "${PROJECT_BINARY_DIR}/bg.jpg"  TIMEOUT 600 STATUS __status)

