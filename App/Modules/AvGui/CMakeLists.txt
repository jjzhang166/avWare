set(AvGuiSRC_LIST 
		AvGui.cpp
	)
set(AvGuiHeader_LIST 
		${project_headers_pach}/AvGui/AvGui.h
	)

add_library(AvGui STATIC ${AvGuiSRC_LIST} ${AvGuiHeader_LIST} ${UI_FILES})