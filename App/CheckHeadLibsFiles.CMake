include (CheckIncludeFiles)
include (CheckLibraryExists)


if (WIN32)
set (LIB_PREFIX )
set (LIB_SUFFIX lib)
else()
set (LIB_PREFIX lib)
set (LIB_SUFFIX a)
endif()

set (REMOTE_PATH_HOME "http://graceport.tpddns.cn:880/avWareOpenSource")
set (REMOTE_PATH_HOME ${REMOTE_PATH_HOME}/${_AV_WARE_VERSION_MAJOR})
set (REMOTE_PATH_HOME "${REMOTE_PATH_HOME}_${_AV_WARE_VERSION_MINOR}")
set (REMOTE_PATH_HOME "${REMOTE_PATH_HOME}_${_AV_WARE_VERSION_PATCH}")
set (REMOTE_PATH_HOME "${REMOTE_PATH_HOME}_${_AV_WARE_VERSION_INWARD}")
set (REMOTE_PATH_HOME "${REMOTE_PATH_HOME}_${_AV_WARE_VERSION_OEM}")
set (REMOTE_PATH_HOME "${REMOTE_PATH_HOME}/${COMPILER}")


set(check_exists_fils ${check_exists_fils} pthread.h)
set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}AvBsp.${LIB_SUFFIX})
set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}AvProto.${LIB_SUFFIX})
set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}AvThread.${LIB_SUFFIX})
set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}AvProto.${LIB_SUFFIX})
set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}AvProto.${LIB_SUFFIX})



set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}uvPlusPlus.${LIB_SUFFIX})
set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}WebService.${LIB_SUFFIX})
set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}Onvif.${LIB_SUFFIX})
set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}Rtmp.${LIB_SUFFIX})
set(check_exists_fils ${check_exists_fils} ${LIB_PREFIX}Rtsp.${LIB_SUFFIX})
foreach(_var ${check_exists_fils})
	if(NOT EXISTS "${PROJECT_BINARY_DIR}/../${_var}")
		 file(DOWNLOAD "${REMOTE_PATH_HOME}/${_var}" "${PROJECT_BINARY_DIR}/../${_var}"  TIMEOUT 1000 STATUS __status)
		 message("DownLoad ${_var} ${__status}")
	endif()
endforeach()


#check_include_files(afdlsfjsdf.h CHECK_AVD_H)
#find_library(FIND_LIB_AVCeEDR NAMES AvCedrShared PATHS ${CMAKE_CURRENT_SOURCE_DIR}/Build/Platform/${COMPILER}/ NO_DEFAULT_PATH)
#check_library_exists(pthreadVC2 "" "" CHECK_LIB_VOIdfCEdf)