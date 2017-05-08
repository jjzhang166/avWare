#Top Dir
cmake_minimum_required(VERSION 3.1)

#set (_AV_WARE_VERSION_MAJOR 1)
#set (_AV_WARE_VERSION_MINOR 1)
#set (_AV_WARE_VERSION_PATCH 3)
#set (_AV_WARE_VERSION_OEM Gp)
#set (_AV_WARE_VERSION_RUNTIME RC) #base、alpha、beta 、RC release(R)


include("CMakeConfig.cmake")
set(SYS_CHN_NUM "64" CACHE STRING "define system Channels")
option (CMAKE_WITH_UI "Build avWare With Ui On Qt5" OFF)
option (HAVE_MemPool "use memory pool for stream buffer " OFF)
option (UI_CLOSE_CAPUTRE "Close Caputre" ON)


############################################
#option (_AV_WARE_M_HAVE_FACE_DETECT "Face Detect" OFF)
#option (_AV_WARE_M_HAVE_PLATERECOGNITION "Plate Recognition" OFF)
option (_AV_WARE_CODE_OPENSOURCE "Open Source" ON)
option (_AV_WARE_CODE_LITE "Lite Version" OFF)
############################################


if (WIN32)
	option (_AV_WARE_M_HAVE_PROC "use proc" OFF)
	#option (_AV_WARE_RENDER_BY_POSWINEOWS "Render Windows By Pos" ON)
	if (COMPILER MATCHES "PC_WIN32")
		set (CMAKE_QT_PATH ${QT_WIN32_PATH})
	elseif (COMPILER MATCHES "PC_WIN64")
		set (CMAKE_QT_PATH ${QT_WIN64_PATH})
	else()
	endif()
	
elseif (UNIX)
	option (_AV_WARE_M_HAVE_PROC "use proc" ON)
	
	
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
		#set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4")
		#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4")
	elseif (COMPILER MATCHES "HISI_16C_18E_V200")
		set(SYS_CHN_NUM "1")
		set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")
		add_definitions("-DCHIP_ID=0x3518E200")
	elseif (COMPILER MATCHES "HISI_3535")
		set (CMAKE_QT_PATH ${QT_NVR35_PATH})
		option (_AV_WARE_RENDER_BY_RECT "Render Windows By Rect" ON)
		set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")
	elseif (COMPILER MATCHES "HISI_3536")
		set (CMAKE_QT_PATH ${QT_NVR36_PATH})
		option (_AV_WARE_RENDER_BY_RECT "Render Windows By Rect" ON)
		set (CMAKE_C_COMPILER "arm-hisiv400-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv400-linux-g++")
		#set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		#set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")

	elseif (COMPILER MATCHES "HISI_3798M")
		set (CMAKE_QT_PATH ${QT_NVR3798M_PATH})
		option (_AV_WARE_RENDER_BY_RECT "Render Windows By Rect" ON)
		set (CMAKE_C_COMPILER "arm-hisiv400-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv400-linux-g++")
	elseif (COMPILER MATCHES "AMBA")
		set(SYS_CHN_NUM "1")
		set (CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
		set (CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
		
	elseif (COMPILER MATCHES "ALLWINNER_A33")
		set(SYS_CHN_NUM "4")
		set (CMAKE_C_COMPILER "arm-linux-gnueabi-gcc")
		set (CMAKE_CXX_COMPILER "arm-linux-gnueabi-g++")
	elseif (COMPILER MATCHES "PC_LIN32")

	elseif (COMPILER MATCHES "PC_LIN64")

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



message(STATUS "")
message(STATUS "-------------------------")

set(CMAKE_BUILD_TYPE ${BUILD_TYPE})

if (WIN32)
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
	add_definitions("-D_WINSOCK_DEPRECATED_NO_WARNINGS")
	if (COMPILER MATCHES "PC_WIN32")
		add_definitions("-D_WIN32")
	elseif (COMPILER MATCHES "PC_WIN64")
		add_definitions("-D_WIN64")
	else()
	endif()
	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /openmp")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /openmp")
	set(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO /MANIFEST:NO")
	#set(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO /MANIFEST:NO /NODEFAULTLIB:MSVCRTD.lib")
	
elseif (UNIX)
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	
	if (${BUILD_TYPE} MATCHES "Release")
		message("[Top CMakeLists] Use Build Type ${CMAKE_BUILD_TYPE}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O2 -pipe -fPIC -Wall -MMD -fno-strict-aliasing -fmessage-length=0 -Wl,-gc-sections")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -O2 -pipe -fPIC -Wall -MMD -fno-strict-aliasing -fmessage-length=0 -Wl,-gc-sections")
	elseif (${BUILD_TYPE} MATCHES "Debug")
		message("[Top CMakeLists] Use Build Type ${CMAKE_BUILD_TYPE}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -ggdb -pipe -fPIC -Wall -MMD -fno-strict-aliasing")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -ggdb -pipe -fPIC -Wall -MMD -fno-strict-aliasing")
	
		if (COMPILER MATCHES "PC_LIN32" OR COMPILER MATCHES "PC_LIN64")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
		endif()
	else()
		message(FATAL_ERROR "Please use cmake -DCMAKE_BUILD_TYPE=Debug(Release) dir current Type[${BUILD_TYPE}]")
	endif()
	
	if (COMPILER MATCHES "PC_LIN32")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
	elseif (COMPILER MATCHES "PC_LIN64")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
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
message("[Top CMakeLists]COMPILER USE [${COMPILER}]")


add_subdirectory(App)

	message(STATUS "-------------------------")
	message(STATUS "")
	


###record for knight
#set(TOOLCHAIN_DIR "/home/ProgamApp/ToolChain/hisi-linux-nptl/arm-hisiv300-linux/target")
#set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR})
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/arm-hisiv300-linux-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-hisiv300-linux-g++)
#file(DOWNLOAD "http://graceport.tpddns.cn:880/bg.jpg" "${PROJECT_BINARY_DIR}/bg.jpg"  TIMEOUT 600 STATUS __status)

