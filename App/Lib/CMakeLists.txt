#LIB

include_directories(MemPool)
set(MemPoolSRC_LIST 
		MemPool/CMemoryPool.cpp
	)
set(MemPoolINC_LIST 
		MemPool/CMemoryPool.h
		MemPool/IMemoryBlock.h
		MemPool/SMemoryChunk.h
	)
	
add_library(MemPool STATIC ${MemPoolSRC_LIST} ${MemPoolINC_LIST})


include_directories(Onvif)

include_directories(NormTools)

set (NormToolsSRC_JSONCPP_LIST
		NormTools/JsonCppSrc/json_tool.h
		NormTools/JsonCppSrc/json_reader.cpp
		NormTools/JsonCppSrc/json_valueiterator.inl
		NormTools/JsonCppSrc/json_value.cpp
		NormTools/JsonCppSrc/json_writer.cpp
		NormTools/JsonCppSrc/version.h.in
)
set(NormToolsINC_JSONCPP_LIST
		NormTools/jsoncpp/json.h
		NormTools/jsoncpp/autolink.h
		NormTools/jsoncpp/value.h
		NormTools/jsoncpp/reader.h
		NormTools/jsoncpp/writer.h
		NormTools/jsoncpp/features.h
		NormTools/jsoncpp/version.h
		NormTools/jsoncpp/config.h
	)

set(NormToolsSRC_LIST 
		NormTools/base64.cpp
		NormTools/md5.cpp
		${NormToolsSRC_JSONCPP_LIST}
	)
	
set(NormToolsINC_LIST 
		NormTools/base64.h
		NormTools/md5.h
		${NormToolsINC_JSONCPP_LIST}
	)
	
add_library(NormTools STATIC ${NormToolsSRC_LIST} ${NormToolsINC_LIST})

include_directories(Smtp)
set(SmtpSRC_LIST 
		Smtp/CSmtp.cpp
	)
set(SmtpINC_LIST 
		Smtp/CSmtp.h
	)
add_library(LibSmtp STATIC ${SmtpSRC_LIST} ${SmtpINC_LIST})

include_directories(Upnp)
set(UpnpSRC_LIST
		Upnp/connecthostport.c
		Upnp/igd_desc_parse.c
		Upnp/minisoap.c
		Upnp/minissdpc.c
		Upnp/miniupnpc.c
		Upnp/miniwget.c
		Upnp/minixml.c
		Upnp/portlistingparse.c
		Upnp/receivedata.c
		Upnp/upnpcommands.c
		Upnp/upnperrors.c
		Upnp/upnplib.c
		Upnp/upnpreplyparse.c
	)
set(UpnpINC_LIST
		Upnp/connecthostport.h
		Upnp/igd_desc_parse.h
		Upnp/minisoap.h
		Upnp/minissdpc.h
		Upnp/miniupnpc.h
		Upnp/miniwget.h
		Upnp/minixml.h
		Upnp/portlistingparse.h
		Upnp/receivedata.h
		Upnp/upnpcommands.h
		Upnp/upnperrors.h
		Upnp/upnplib.h
		Upnp/bsdqueue.h
		Upnp/codelength.h
		Upnp/declspec.h
		Upnp/miniupnpcstrings.h
		Upnp/miniupnpctypes.h
	)
add_library(LibUpnp STATIC ${UpnpSRC_LIST} ${UpnpINC_LIST})	

if (WIN32)

elseif(UNIX)
include_directories(Dhcp)
set(DhcpSRC_LIST
		Dhcp/dhcp.c
	)
set(DhcpINC_LIST
		Dhcp/dhcp.h
	)
add_library(LibDhcp STATIC ${DhcpSRC_LIST} ${DhcpINC_LIST})
endif()

include_directories(Ddns)
set(DdnsSRC_LIST
		Ddns/Ddns_Enable.c
		Ddns/Ddns_Net.c
)
set(DdnsINC_LIST
		Ddns/Ddns_Global.h
		Ddns/Ddns_Net.h
		Ddns/NetDdns.h
)
add_library(LibDdns STATIC ${DdnsSRC_LIST} ${DdnsINC_LIST})

include_directories(Ftp)
set(FtpSRC_LIST
		Ftp/Ftp.cpp
)
set(FtpINC_LIST
		Ftp/Ftp.h
)
add_library(LibFtp STATIC ${FtpSRC_LIST} ${FtpINC_LIST})

include_directories(Rtsp)




set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
