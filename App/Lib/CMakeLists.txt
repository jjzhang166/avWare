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

include_directories(Common)

set (CommonSRC_JSONCPP_LIST
		Common/JsonCppSrc/json_tool.h
		Common/JsonCppSrc/json_reader.cpp
		Common/JsonCppSrc/json_valueiterator.inl
		Common/JsonCppSrc/json_value.cpp
		Common/JsonCppSrc/json_writer.cpp
		Common/JsonCppSrc/version.h.in
)
set(CommonINC_JSONCPP_LIST
		Common/jsoncpp/json.h
		Common/jsoncpp/autolink.h
		Common/jsoncpp/value.h
		Common/jsoncpp/reader.h
		Common/jsoncpp/writer.h
		Common/jsoncpp/features.h
		Common/jsoncpp/version.h
		Common/jsoncpp/config.h
	)

set(CommonSRC_LIST 
		Common/base64.cpp
		Common/md5.cpp
		${CommonSRC_JSONCPP_LIST}
	)
	
set(CommonINC_LIST 
		Common/base64.h
		Common/md5.h
		${CommonINC_JSONCPP_LIST}
	)
	
add_library(LibCommon STATIC ${CommonSRC_LIST} ${CommonINC_LIST})

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

include_directories(Rtmp)


set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
