
#UI
if (!CMAKE_QT_PATH)
	message(FATAL_ERROR ${CMAKE_QT_PATH})
endif()
message(STATUS "CMakeLists AvGui Dir ${CMAKE_QT_PATH} ")
SET(CMAKE_PREFIX_PATH ${CMAKE_QT_PATH})
find_package(Qt5Core)
find_package(Qt5Widgets)
#find_package(Qt5Quick)
find_package(Qt5LinguistTools)
ADD_DEFINITIONS (${QT_DEFINITIONS})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(AvUiComm)
include_directories(AvForm)
include_directories(AvSource)

set(CMAKE_AUTOMOC ON)

set(AvGuiSRC_LIST 
		AvGui.cpp
		AvForm/frmmessagebox.cpp
		AvForm/frmmainwindows.cpp
		AvForm/frmdevice.cpp
		AvForm/dlgnetset.cpp
		AvForm/dlgupgrade.cpp
		
		AvForm/dlghandadddevice.cpp
		AvForm/dlgdeviceset.cpp
		AvForm/dlgptzwindows.cpp
		AvForm/wgetvideoinfo.cpp
		AvForm/formptzadvanced.cpp
		AvForm/dlgareaselect.cpp
		AvForm/dlgoverlay.cpp
		AvForm/dlgrecordset.cpp
		AvForm/dlgalarmmessage.cpp
		AvForm/wgtrecordshow.cpp
		AvForm/frmrecordsearch.cpp
		AvForm/frmmainmenu.cpp
		AvForm/frmnotificationbox.cpp
		AvForm/dlgsnapshot.cpp
		AvForm/dlgtimeselect.cpp
		AvForm/dlgabout.cpp
		AvUiComm/IconComm.cpp
		AvUiComm/IpAddrEdit.cpp
		AvUiComm/IpPartLineEdit.cpp
		AvUiComm/FloatingButton.cpp
		AvUiComm/progressbardelegate.cpp
		AvUiComm/tablemodel.cpp
		AvUiComm/AvUiConfigIni.cpp
		AvUiComm/StartupWindow.cpp
		AvSource/videowidget.cpp
		AvSource/VideoWidgetManager.cpp
		AvSource/umslider.cpp
		AvSource/QAvEvent.cpp
		AvSource/AvGuiApplication.cpp
		AvSource/AvGuiStatusMachine.cpp
	)
		
set(AvGuiHeader_LIST 
		${project_headers_pach}/AvGui/AvGui.h
		AvForm/frmmessagebox.h
		AvForm/frmmainwindows.h
		AvForm/frmdevice.h
		AvForm/dlgnetset.h
		AvForm/dlgupgrade.h
		AvForm/dlghandadddevice.h
		AvForm/dlgdeviceset.h
		AvForm/dlgptzwindows.h
		AvForm/wgetvideoinfo.h
		AvForm/formptzadvanced.h
		AvForm/dlgareaselect.h
		AvForm/dlgoverlay.h
		AvForm/dlgrecordset.h
		AvForm/dlgalarmmessage.h
		AvForm/wgtrecordshow.h
		AvForm/frmrecordsearch.h
		AvForm/frmmainmenu.h
		AvForm/frmnotificationbox.h
		AvForm/dlgtimeselect.h
		AvForm/dlgabout.h
		AvForm/dlgsnapshot.h
		AvUiComm/IconComm.h
		AvUiComm/AvUiComm.h
		AvUiComm/IpAddrEdit.h
		AvUiComm/IpPartLineEdit.h
		AvUiComm/FloatingButton.h
		AvUiComm/progressbardelegate.h
		AvUiComm/tablemodel.h
		AvUiComm/StdListDevicesSort.h
		AvUiComm/AvUiConfigIni.h
		AvUiComm/StartupWindow.h
		AvSource/videowidget.h
		AvSource/VideoWidgetManager.h
		AvSource/umslider.h
		AvSource/QAvEvent.h
		AvSource/AvGuiApplication.h
		AvSource/AvQStringsLanguage.h
		AvSource/AvGuiStatusMachine.h
		AvGuiSystem.h
	)
	


set (SOURCE_UI
	AvForm/frmmessagebox.ui
	AvForm/frmmainwindows.ui
	AvForm/frmdevice.ui
	AvForm/dlgnetset.ui
	AvForm/dlgupgrade.ui
	AvForm/dlghandadddevice.ui
	AvForm/dlgdeviceset.ui
	AvForm/dlgptzwindows.ui
	AvForm/wgetvideoinfo.ui
	AvForm/formptzadvanced.ui
	AvForm/dlgareaselect.ui
	AvForm/dlgoverlay.ui
	#AvForm/ctimearea.ui
	AvForm/dlgtimeselect.ui
	AvForm/dlgabout.ui
	AvForm/dlgrecordset.ui
	AvForm/dlgalarmmessage.ui
	AvForm/wgtrecordshow.ui
	AvForm/frmrecordsearch.ui
	AvForm/frmmainmenu.ui
	AvForm/frmnotificationbox.ui
	AvForm/dlgsnapshot.ui
)

if (_AV_WARE_CODE_OPENSOURCE STREQUAL "OFF")
	set(AvGuiSRC_LIST ${AvGuiSRC_LIST}
			AvForm/dlgmkfirmware.cpp
			AvForm/dlgfactoryset.cpp
	)
	
	set(AvGuiHeader_LIST ${AvGuiHeader_LIST}
			AvForm/dlgmkfirmware.h
			AvForm/dlgfactoryset.h
	)
	
	
	set (SOURCE_UI ${SOURCE_UI}
			AvForm/dlgmkfirmware.ui
			AvForm/dlgfactoryset.ui
	)
endif()

set (SOURCE_RC 
	AvResource/avWare.qrc
)

set(SOURCE_TRANSLATIONS
    zh_CN.ts
    en_US.ts
)

qt5_add_resources(RC_FILES ${SOURCE_RC})
qt5_wrap_ui(UI_FILES ${SOURCE_UI})
qt5_create_translation(QM_FILES 
	${AvGuiSRC_LIST}
	${AvGuiHeader_LIST}
	${SOURCE_UI}
	${SOURCE_TRANSLATIONS})

add_custom_target (uiLanguage ALL DEPENDS ${QM_FILES})
set_property(TARGET uiLanguage PROPERTY FOLDER "AvQui")


add_library(AvGui STATIC ${AvGuiSRC_LIST} ${AvGuiHeader_LIST} ${UI_FILES} ${RC_FILES})
set_property(TARGET AvGui PROPERTY FOLDER "AvQui")
target_link_libraries(AvGui Qt5::Widgets)
#target_link_libraries(AvGui Qt5::Core)
#target_link_libraries(AvGui Qt5::Gui)


set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../)

#
