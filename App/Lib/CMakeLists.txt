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
set_property(TARGET MemPool PROPERTY FOLDER "AvLibs")

include_directories(Onvif)

include_directories(Common)

set (CommonSRC_JSONCPP_LIST
		Common/JsonCppSrc/json_reader.cpp
		Common/JsonCppSrc/json_value.cpp
		Common/JsonCppSrc/json_writer.cpp
		Common/JsonCppSrc/json_tool.h
		Common/JsonCppSrc/json_valueiterator.inl
		Common/JsonCppSrc/version.h.in
)

set(CommonINC_JSONCPP_LIST
		Common/jsoncpp/json.h
		Common/jsoncpp/autolink.h
		Common/jsoncpp/value.h
		Common/jsoncpp/reader.h
		Common/jsoncpp/writer.h
		Common/jsoncpp/config.h
		Common/jsoncpp/features.h
		Common/jsoncpp/version.h
		Common/jsoncpp/forwards.h
		Common/jsoncpp/allocator.h
		Common/jsoncpp/assertions.h
	)
#Common/jsoncpp/features.h
#Common/jsoncpp/version.h

set (CommonSRC_GTKUTF8_LIST
		Common/gtkutf8.c)
set (CommonINC_GTKUTF8_LIST
		Common/gtkutf8.h)

set(CommonSRC_BASE64_LIST 
		Common/base64.cpp
		Common/md5.cpp
		
	)
	
set(CommonINC_BASE64_LIST 
		Common/base64.h
		Common/md5.h
		
	)
set (CommonSRC_LIST
	${CommonSRC_JSONCPP_LIST}
	${CommonSRC_BASE64_LIST}
	${CommonSRC_GTKUTF8_LIST}
	)
set (CommonINC_LIST
		${CommonINC_JSONCPP_LIST}
	${CommonINC_BASE64_LIST}
	${CommonINC_GTKUTF8_LIST}
	)
	
add_library(Common STATIC ${CommonSRC_LIST} ${CommonINC_LIST})
set_property(TARGET Common PROPERTY FOLDER "AvLibs")

include_directories(Smtp)
set(SmtpSRC_LIST 
		Smtp/CSmtp.cpp
	)
set(SmtpINC_LIST 
		Smtp/CSmtp.h
	)
add_library(Smtp STATIC ${SmtpSRC_LIST} ${SmtpINC_LIST})
set_property(TARGET Smtp PROPERTY FOLDER "AvLibs")

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
add_library(Upnp STATIC ${UpnpSRC_LIST} ${UpnpINC_LIST})	
set_property(TARGET Upnp PROPERTY FOLDER "AvLibs")

if (WIN32)

elseif(UNIX)
include_directories(Dhcp)
set(DhcpSRC_LIST
		Dhcp/dhcp.c
	)
set(DhcpINC_LIST
		Dhcp/dhcp.h
	)
add_library(Dhcp STATIC ${DhcpSRC_LIST} ${DhcpINC_LIST})
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
add_library(Ddns STATIC ${DdnsSRC_LIST} ${DdnsINC_LIST})
set_property(TARGET Ddns PROPERTY FOLDER "AvLibs")

include_directories(Ftp)
set(FtpSRC_LIST
		Ftp/Ftp.cpp
)
set(FtpINC_LIST
		Ftp/Ftp.h
)
add_library(Ftp STATIC ${FtpSRC_LIST} ${FtpINC_LIST})
set_property(TARGET Ftp PROPERTY FOLDER "AvLibs")

##SQLITE
include_directories(SQlite)
set(SQLiteSRC_LIST
		Sqlite/sqlite3.c
)
set(SQLiteINC_LIST
		Sqlite/sqlite3.h
		Sqlite/sqlite3ext.h
)
add_library(SQLite STATIC ${SQLiteINC_LIST} ${SQLiteSRC_LIST})
set_property(TARGET SQLite PROPERTY FOLDER "AvLibs")


include_directories(Rtsp)

include_directories(Rtmp)

set(LUAINC_LIST 
		Lua/lua.h
		Lua/llex.h
		Lua/luaconf.h
		Lua/ltm.h
		Lua/lcode.h
		Lua/lvm.h
		Lua/lparser.h
		Lua/ldo.h
		Lua/lgc.h
		Lua/lstate.h
		Lua/lualib.h
		Lua/lobject.h
		Lua/ltable.h
		Lua/lstring.h
		Lua/lundump.h
		Lua/lzio.h
		Lua/ldebug.h
		Lua/lapi.h
		Lua/lfunc.h
		Lua/lprefix.h
		Lua/lmem.h
		Lua/lopcodes.h
		Lua/lualib.h
		Lua/lctype.h
		Lua/lua.hpp
	)
set(LUASRC_LIST 
		Lua/lapi.c
		Lua/lcode.c
		Lua/lctype.c
		Lua/ldebug.c
		Lua/ldo.c
		Lua/ldump.c
		Lua/lfunc.c
		Lua/lgc.c
		Lua/llex.c
		Lua/lmem.c
		Lua/lobject.c
		Lua/lopcodes.c
		Lua/lparser.c
		Lua/lstate.c
		Lua/lstring.c
		Lua/ltable.c
		Lua/ltm.c
		Lua/lundump.c
		Lua/lvm.c
		Lua/lzio.c
		Lua/lauxlib.c
		Lua/lbaselib.c
		Lua/lbitlib.c
		Lua/lcorolib.c
		Lua/ldblib.c
		Lua/liolib.c
		Lua/lmathlib.c
		Lua/loslib.c
		Lua/lstrlib.c
		Lua/ltablib.c
		Lua/lutf8lib.c
		Lua/loadlib.c
		Lua/linit.c
	)
add_library(Lua STATIC ${LUAINC_LIST} ${LUASRC_LIST})
#add_library(Lua SHARED ${LUAINC_LIST} ${LUASRC_LIST})
set_property(TARGET Lua PROPERTY FOLDER "AvLibs")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
