#App
project (AvWare)
include_directories(Includes)
include_directories(Lib)
include_directories(Lib/Common)

set(SRC_LIST 
		MainSrc/main.cpp
	)
file(GLOB_RECURSE ApisHeaders ${CMAKE_CURRENT_SOURCE_DIR}/Includes/Apis/*.h)
set(SRC_HEADER 
	${PROJECT_BINARY_DIR}/../config.h
	${ApisHeaders}
	)
set (target avWare)
##全局变量使用
#set(UI_RCFILES "" CACHE INTERNAL "GLOBAL VAR RC FILES")


if (WIN32)
	set (SRC_RCFILES ${SRC_RCFILES} MainSrc/main.rc)
endif()


link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Build/Platform/${COMPILER}/)

add_subdirectory(Lib)
add_subdirectory(Modules)

##文件输出 
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/..)
set(AVWARE_APP_HOME ${AVWARE_HOME_DIR}/App)
if (CMAKE_BUILD_TYPE MATCHES "Release")
		add_executable(${target} WIN32 ${SRC_LIST} ${SRC_HEADER} ${SRC_RCFILES})
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
		add_executable(${target} ${SRC_LIST} ${SRC_HEADER} ${SRC_RCFILES})
else()
		message(FATAL_ERROR "packet mast use  cmake -DCMAKE_BUILD_TYPE=Release dir")
endif()


set (CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})


#Moudles
target_link_libraries(${target} AvDevice)
target_link_libraries(${target} AvConfigs)
target_link_libraries(${target} AvRecord)
target_link_libraries(${target} AvLog)
target_link_libraries(${target} AvFile)
target_link_libraries(${target} AvAlarm)
target_link_libraries(${target} AvCapture)

if (${CMAKE_WITH_UI} STREQUAL "ON")
	target_link_libraries(${target} AvGui)
	link_directories(${CMAKE_QT_PATH}/bin/)
endif()
target_link_libraries(${target} AvThread)
target_link_libraries(${target} AvPreview)
target_link_libraries(${target} AvNetService)
target_link_libraries(${target} AvUart)
target_link_libraries(${target} AvPacket)
target_link_libraries(${target} AvIntelligentAnalysis)
target_link_libraries(${target} AvBsp)
target_link_libraries(${target} AvMemoryPool)
target_link_libraries(${target} AvUser)
target_link_libraries(${target} AvF)
target_link_libraries(${target} AvProc)
target_link_libraries(${target} AvProto)
#LIB
target_link_libraries(${target} uvPlusPlus)
target_link_libraries(${target} uv)
target_link_libraries(${target} WebService)
target_link_libraries(${target} Rtsp)
target_link_libraries(${target} MemPool)
target_link_libraries(${target} Onvif)
target_link_libraries(${target} Smtp)
target_link_libraries(${target} Upnp)
target_link_libraries(${target} Ddns)
target_link_libraries(${target} Ftp)
target_link_libraries(${target} Common)
target_link_libraries(${target} Rtmp)
target_link_libraries(${target} Lua)
target_link_libraries(${target} SQLite)




##third lib
include ("Build/Platform/${COMPILER}/link.cmake")

#UI

##third lib
include ("Build/Platform/${COMPILER}/link.cmake")


if (${CMAKE_WITH_UI} STREQUAL "ON")
		if (!CMAKE_QT_PATH)
			message(FATAL_ERROR ${CMAKE_QT_PATH})
		endif()
	SET(CMAKE_PREFIX_PATH ${CMAKE_QT_PATH})
	find_package(Qt5Core)
	find_package(Qt5Widgets)
	#find_package(qtpcre)
	#find_package(Qt5Quick)
	ADD_DEFINITIONS (${QT_DEFINITIONS})
	#target_link_libraries(${target} qtpcre)
	#qt5_use_modules(${target} Core Widgets Gui)
	#get_target_property(${target} Qt5::Core LOCATION)
endif()

#SYSTEM Lib
if (WIN32)
	target_link_libraries(${target} pthreadVC2)
	target_link_libraries(${target} ws2_32)
	target_link_libraries(${target} iphlpapi)
	target_link_libraries(${target} libeay32)
	target_link_libraries(${target} ssleay32)
	target_link_libraries(${target} zlibstatic)
	target_link_libraries(${target} Psapi)
	target_link_libraries(${target} Winmm)
elseif (UNIX)
	target_link_libraries(${target} ssl)
	target_link_libraries(${target} crypto)
	target_link_libraries(${target} pthread)
	target_link_libraries(${target} dl)
	target_link_libraries(${target} rt)
	target_link_libraries(${target} z)
	if(COMPILER MATCHES "PC_LINUX")
		target_link_libraries(${target} bz2)
	endif()
	target_link_libraries(${target} m)
endif()

include (CPack.CMake)
#include (CheckHeadLibsFiles.CMake)