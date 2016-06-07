#App
project (AvWare)
include_directories(Includes)
include_directories(Lib)
include_directories(Lib/NormTools)

set(SRC_LIST 
		MainSrc/main.cpp
	)
set(SRC_HEADER 
	${PROJECT_BINARY_DIR}/../config.h
	)
set (target avWare)


add_subdirectory(Lib)
add_subdirectory(Modules)

if (WIN32)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Build/Platform/IPC_WIN/)
elseif (UNIX)
	if (COMPILER MATCHES "HISI16C")
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Build/Platform/IPC_Hisi16C/)
	elseif (COMPILER MATCHES "HISI16A")
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Build/Platform/IPC_Hisi16A/)
	elseif (COMPILER MATCHES "AMBA")
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Build/Platform/IPC_Amba/)
	elseif (COMPILER MATCHES "LINUX")
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Build/Platform/IPC_Linux/)
	endif()
endif()#WIN32

##文件输出 
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
set(AVWARE_APP_HOME ${AVWARE_HOME_DIR}/App)
add_executable(${target} ${SRC_LIST} ${SRC_HEADER})
set (CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})



#Moudles
target_link_libraries(${target} AvConfigs)
target_link_libraries(${target} AvLog)
target_link_libraries(${target} AvFile)
target_link_libraries(${target} AvExternInterface)
target_link_libraries(${target} AvAlarm)
target_link_libraries(${target} AvCapture)
target_link_libraries(${target} AvAudio)
target_link_libraries(${target} AvDecode)
target_link_libraries(${target} AvDevice)
target_link_libraries(${target} AvGui)
target_link_libraries(${target} AvNetService)
target_link_libraries(${target} AvWatchDog)
target_link_libraries(${target} AvThread)
target_link_libraries(${target} AvUart)
target_link_libraries(${target} AvBsp)
target_link_libraries(${target} AvPacket)
target_link_libraries(${target} AvMemoryPool)
target_link_libraries(${target} AvUser)

#LIB
target_link_libraries(${target} LibWebService)
target_link_libraries(${target} LibRtsp)
target_link_libraries(${target} MemPool)
target_link_libraries(${target} LibOnvif)
target_link_libraries(${target} LibSmtp)
target_link_libraries(${target} LibUpnp)
target_link_libraries(${target} LibDdns)
target_link_libraries(${target} LibFtp)
target_link_libraries(${target} NormTools)

#UI

if (${WITH_UI} STREQUAL "ON")
		if (!QT_PATH)
			message(FATAL_ERROR ${QT_PATH})
		endif()
	SET(CMAKE_PREFIX_PATH ${QT_PATH})
	find_package(Qt5Core)
	find_package(Qt5Widgets)
	find_package(Qt5Quick)
	ADD_DEFINITIONS (${QT_DEFINITIONS})
target_link_libraries(${target} Qt5::Widgets)
endif()

##third lib
if (WIN32)
	include ("Build/Platform/IPC_WIN/link.cmake")
elseif (UNIX)
	if (COMPILER MATCHES "HISI16C")
		include ("Build/Platform/IPC_Hisi16C/link.cmake")
	elseif(COMPILER MATCHES "HISI16A")
		include ("Build/Platform/IPC_Hisi16A/link.cmake")
	elseif(COMPILER MATCHES "AMBA")
		include ("Build/Platform/IPC_Amba/link.cmake")
	elseif(COMPILER MATCHES "LINUX")
	include ("Build/Platform/IPC_Linux/link.cmake")
	endif()
endif()


#SYSTEM Lib
if (WIN32)
	target_link_libraries(${target} pthreadVC2)
	target_link_libraries(${target} ws2_32)
	target_link_libraries(${target} iphlpapi)
	target_link_libraries(${target} libeay32)
	target_link_libraries(${target} ssleay32)
elseif (UNIX)
	target_link_libraries(${target} ssl)
	target_link_libraries(${target} crypto)
	target_link_libraries(${target} pthread)
	target_link_libraries(${target} dl)
	target_link_libraries(${target} rt)
	if(COMPILER MATCHES "LINUX")
		target_link_libraries(${target} bz2)
	endif()
	target_link_libraries(${target} m)
	if(COMPILER MATCHES "LINUX")
		target_link_libraries(${target} z)
	endif()
endif()






##install
if (WIN32)
install (FILES Build/Platform/IPC_WIN/avcodec-56.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/avdevice-56.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/avfilter-5.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/avformat-56.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/avutil-54.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/swresample-1.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/swscale-3.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/zlib1.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/libbz2-2.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/libgcc_s_dw2-1.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/libiconv-2.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/SDL.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/libmfxsw32.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/postproc-53.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/pthreadGC2.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/libx264-148.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/CapEncoder.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/ProtoShare.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/libeay32.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
install (FILES Build/Platform/IPC_WIN/ssleay32.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)




elseif (UNIX)
	if (COMPILER MATCHES "HISI16C")
		install (FILES ${target} DESTINATION /nfsroot/)
	elseif (COMPILER MATCHES "HISI16A")
		#install (FILES "./avWare" DESTINATION /nfsroot/)
		install (FILES  Build/Platform/IPC_Hisi16A/libProtoShare.so DESTINATION /nfsroot/)
	elseif (COMPILER MATCHES "AMBA")
		install (FILES ${target} DESTINATION /nfsroot/)
	elseif (COMPILER MATCHES "LINUX")
		install (DIRECTORY ../Bin/webroot DESTINATION ${EXECUTABLE_OUTPUT_PATH}) 
	endif()
endif()#WIN32

