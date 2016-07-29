#Top Dir
cmake_minimum_required(VERSION 3.1)

if (WIN32)
	option (HAVE_CapCamera "use laptop camera for capture " OFF)
	option (HAVE_CapScreen "use screen for capture " ON)
	option (HAVE_EncIntelGPU "use intel media sdk for encodeing " ON)
	option (HAVE_EncFfmpeg "use ffmpeg for encodeing " OFF)
	option (HAVE_MemPool "use memory pool for stream buffer " OFF)
	option (WITH_UI "Build avWare With Ui On Qt5" OFF)
	set (QT_PATH "F:\\ProgramTools\\Qt\\Qt5.5.0\\5.5\\msvc2013")
elseif (UNIX)
	option (HAVE_MemPool "use memory pool for stream buffer " OFF)
	option (WITH_UI "Build avWare With Ui On Qt5" OFF)
	#COMPILER
	#set (COMPILER LINUX)
	#set (COMPILER AMBA)
	#set (COMPILER HISI16C)
	set (COMPILER HISI18EV100)
	#set (COMPILER HISI18C)
	#set (COMPILER HISI18A)
	#set (COMPILER HISI16D)
	#set (COMPILER HISI16A)
	#set (COMPILER HISI3535)
	
	set(CMAKE_SYSTEM_NAME Linux)
	if (COMPILER MATCHES "HISI16C" OR COMPILER MATCHES "HISI18EV100" OR COMPILER MATCHES "HISI18C" OR COMPILER MATCHES "HISI18A" )
		option (HAVE_SensorInput "use ffmpeg for encodeing " ON)
		#set(TOOLCHAIN_DIR "/home/ProgamApp/ToolChain/hisi-linux-nptl/arm-hisiv300-linux/target")
		#set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR})
		#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
		#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
		#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
		#set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/arm-hisiv300-linux-gcc)
		#set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-hisiv300-linux-g++)
		set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")
	elseif (COMPILER MATCHES "HISI16A" OR COMPILER MATCHES "HISI16D")
		option (HAVE_HdmiInput "use memory pool for stream buffer " ON)
		option (HAVE_SensorInput "use ffmpeg for encodeing " OFF)
		set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")
	elseif (COMPILER MATCHES "HISI3535")
		option (HAVE_HdmiInput "use memory pool for stream buffer " ON)
		option (HAVE_SensorInput "use ffmpeg for encodeing " OFF)
		set (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
		set (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")
	elseif (COMPILER MATCHES "AMBA")
		option (HAVE_HdmiInput "use memory pool for stream buffer " OFF)
		option (HAVE_SensorInput "use ffmpeg for encodeing " ON)
		set (CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
		set (CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
	elseif (COMPILER MATCHES "LINUX")
		option (HAVE_CapCamera "use laptop camera for capture " OFF)
		option (HAVE_CapScreen "use screen for capture " ON)
		option (HAVE_EncFfmpeg "use ffmpeg for encodeing " ON)
	endif()
endif()



set(CMAKE_SUPPRESS_REGENERATION 1)
set(CMAKE_USE_RELATIVE_PATHS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
set(PROJDIR ${CMAKE_CURRENT_SOURCE_DIR})
set (AVWARE_HOME_DIR ${PROJDIR})
set (CMAKE_CONFIGURATION_TYPES Debug Release)

set (CMAKE_BUILD_TYPE "Release")
set (CMAKE_BUILD_TYPE "Debug")
project (avWare)





configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

include_directories(${PROJECT_BINARY_DIR})

	add_definitions("-D_AV_WARE_")
	
if (WIN32)
	set(CMAKE_C_FLAGS_DEBUG, "-g -Wall -O2" )
	set(CMAKE_CXX_FLAGS_DEBUG, "-g -Wall -O2" )

	set(CMAKE_C_FLAGS_RELEASE, "-Wall -O2" )
	set(CMAKE_CXX_FLAGS_RELEASE, "-Wall -O2" )
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
	set(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO ")
elseif (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -O2")
endif()

if (WITH_UI STREQUAL "ON")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
endif()
	
ADD_SUBDIRECTORY(App)

