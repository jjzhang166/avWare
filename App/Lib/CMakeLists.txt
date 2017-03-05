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
set(OnvifSRC_LIST 
#		Onvif/alarm.c
#		Onvif/profile.c
#		Onvif/network.c
#		Onvif/group.c
#		Onvif/list.c
#		Onvif/userdefine.c
#		Onvif/usrmem.c
#		Onvif/xml.c
#		Onvif/onvifinit.c
#		Onvif/onvifmain.c
	)
set(OnvifINC_LIST 
		Onvif/alarm.h
		Onvif/profile.h
		Onvif/common.h
		Onvif/network.h
		Onvif/group.h
		Onvif/list.h
		Onvif/userdefine.h
		Onvif/usrmem.h
		Onvif/xml.h
		Onvif/onvifinit.h
		Onvif/onvifAPI.h
	)
	
#add_library(Onvif STATIC ${OnvifSRC_LIST} ${OnvifINC_LIST})
#set_property(TARGET Onvif PROPERTY FOLDER "AvLibs")

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
	
set (CommonSRC_ICMP_LIST
		Common/icmp.cpp
	)
set (CommonINC_ICMP_LIST
		Common/icmp.h
)

set (CommonSRC_ARP_LIST
		Common/arp.cpp
)
set (CommonINC_ARP_LIST
		Common/arp.h
)

set (CommonSRC_LIST
	${CommonSRC_JSONCPP_LIST}
	${CommonSRC_BASE64_LIST}
	${CommonSRC_GTKUTF8_LIST}
	${CommonSRC_ICMP_LIST}
	${CommonSRC_ARP_LIST}
	)
set (CommonINC_LIST
	${CommonINC_JSONCPP_LIST}
	${CommonINC_BASE64_LIST}
	${CommonINC_GTKUTF8_LIST}
	${CommonINC_ICMP_LIST}
	${CommonINC_ARP_LIST}
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
set(RtspSRC_LIST 
#		Rtsp/Rbuffer.cpp
#		Rtsp/Rtsp.cpp
#		Rtsp/RSock.cpp
#		Rtsp/decSps.cpp
#		Rtsp/Http.cpp
#		Rtsp/RTCP.cpp
#		Rtsp/RTP.cpp
#		Rtsp/RtspMedia.cpp
#		Rtsp/Session.cpp
#		Rtsp/LibRtspServer.cpp
#		Rtsp/LibRtspClient.cpp
	)
set(RtspINC_LIST 
		Rtsp/Rbuffer.h
		Rtsp/Rtsp.h
		Rtsp/RSock.h
#		Rtsp/decSps.h
		Rtsp/Http.h
		Rtsp/RTCP.h
		Rtsp/RTP.h
		Rtsp/RtspMedia.h
		Rtsp/Session.h
		Rtsp/LibRtspServer.h
		Rtsp/LibRtspClient.h
	)
#add_library(Rtsp STATIC ${RtspSRC_LIST} ${RtspINC_LIST})
#set_property(TARGET Rtsp PROPERTY FOLDER "AvLibs")

include_directories(Rtmp)
set(RtmpSRC_LIST 
#		Rtmp/amf.c
#		Rtmp/hashswf.c
#		Rtmp/log.c
#		Rtmp/parseurl.c
#		Rtmp/rtmp.c
	)
set(RtmpINC_LIST 
		Rtmp/amf.h
		Rtmp/log.h
		Rtmp/bytes.h
		Rtmp/dh.h
		Rtmp/dhgroups.h
		Rtmp/handshake.h
		Rtmp/rtmp_sys.h	
	)
#add_library(Rtmp STATIC ${RtmpSRC_LIST} ${RtmpINC_LIST})
#set_property(TARGET Rtmp PROPERTY FOLDER "AvLibs")

#MP4
#include_directories(Mp4)
#set(Mp4SRC_LIST 
#		Mp4/libmp4.cpp
#	)
#set(Mp4INC_LIST 
#		Mp4/libmp4.h
#		Mp4/libmp4_c.h
#	)
#add_library(Mp4 STATIC ${Mp4SRC_LIST} ${Mp4INC_LIST})
#set_property(TARGET Mp4 PROPERTY FOLDER "AvLibs")

set(WebServiceINC_LIST 
		WebService/WebHttpTask.h
		WebService/LibWebService.h
	)
set(WebServiceSRC_LIST 
#		WebService/WebHttpTask.cpp
#		WebService/LibWebService.cpp
	)

if (WIN32)
#	set(WebServiceINC_LIST ${WebServiceINC_LIST} WebService/dirent.h)
#	set(WebServiceSRC_LIST ${WebServiceSRC_LIST} WebService/dirent.cpp)
endif()

#add_library(WebService STATIC ${WebServiceINC_LIST} ${WebServiceSRC_LIST})
#set_property(TARGET WebService PROPERTY FOLDER "AvLibs")

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

include_directories(uv/include)
include_directories(uv/src)
include_directories(uv)
if(WIN32)
set (uvSrcList
#		uv/src/win/snprintf.c
#		uv/src/win/detect-wakeup.c
		uv/src/win/async.c
		uv/src/win/atomicops-inl.h
		uv/src/win/core.c
		uv/src/win/dl.c
		uv/src/win/error.c
		uv/src/win/fs.c
		uv/src/win/fs-event.c
		uv/src/win/getaddrinfo.c
		uv/src/win/getnameinfo.c
		uv/src/win/handle.c
		uv/src/win/handle-inl.h
		uv/src/win/internal.h
		uv/src/win/loop-watcher.c
		uv/src/win/pipe.c
		uv/src/win/thread.c
		uv/src/win/poll.c
		uv/src/win/process.c
		uv/src/win/process-stdio.c
		uv/src/win/req.c
		uv/src/win/req-inl.h
		uv/src/win/signal.c
		uv/src/win/stream.c
		uv/src/win/stream-inl.h
		uv/src/win/tcp.c
		uv/src/win/tty.c
		uv/src/win/timer.c
		uv/src/win/udp.c
		uv/src/win/util.c
		uv/src/win/winapi.c
		uv/src/win/winapi.h
		uv/src/win/winsock.c
		uv/src/win/winsock.h
	)
else()
set (uvSrcList
		uv/src/unix/async.c
		uv/src/unix/atomic-ops.h
		uv/src/unix/core.c
		uv/src/unix/dl.c
		uv/src/unix/fs.c
		uv/src/unix/getaddrinfo.c
		uv/src/unix/getnameinfo.c
		uv/src/unix/internal.h
		uv/src/unix/loop.c
		uv/src/unix/loop-watcher.c
		uv/src/unix/pipe.c
		uv/src/unix/poll.c
		uv/src/unix/process.c
		uv/src/unix/signal.c
		uv/src/unix/spinlock.h
		uv/src/unix/stream.c
		uv/src/unix/tcp.c
		uv/src/unix/thread.c
		uv/src/unix/timer.c
		uv/src/unix/tty.c
		uv/src/unix/udp.c
		uv/src/unix/linux-core.c
		uv/src/unix/linux-syscalls.c
		uv/src/unix/linux-inotify.c
)
endif()

set (uvCommon
	uv/src/fs-poll.c
	uv/src/heap-inl.h
	uv/src/inet.c
	uv/src/queue.h
	uv/src/threadpool.c
	uv/src/uv-common.c
	uv/src/uv-common.h
	uv/src/version.c
)
add_library(uv STATIC ${uvSrcList} ${uvCommon})
#add_definitions("-D_WIN32")
set_property(TARGET uv PROPERTY FOLDER "AvLibs")

set (uvPlusPlus_List
#	uvPlusPlus/libuvCpp.cpp
	uvPlusPlus/libuvCpp.h
)
#add_library(uvPlusPlus STATIC ${uvPlusPlus_List} )
#set_property(TARGET uvPlusPlus PROPERTY FOLDER "AvLibs")


set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
