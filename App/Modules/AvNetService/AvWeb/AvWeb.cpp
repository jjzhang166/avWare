/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者: Xman [xman@graceport.cn]
|  日期:
|  说明:
******************************************************************/
#include "AvNetService/AvWeb.h"
#include "jsoncpp/json.h"
#include "AvCapture/AvManCapture.h"
#include "AvPacket/AvPacket.h"
#include "AvDevice/AvDevice.h"
SINGLETON_IMPLEMENT(CAvWebSer)

CAvWebSer::CAvWebSer()
{
}

CAvWebSer::~CAvWebSer()
{
}

av_bool CAvWebSer::Start()
{
	RegisterMsgProc();

	WebStart();

	std::string Value;
	CAvDevice::GetEnv(std::string(EKey_WebIndex), Value);
	if (Value.empty() == false){
		WebSetIndex(Value);
	}
	CAvDevice::GetEnv(std::string(EKey_WebRoot), Value);
	if (Value.empty() == false){
		WebSetRootDir(Value);
	}
	return av_true;
}
av_bool CAvWebSer::Stop()
{
	WebStop();
	return av_true;
}

void CAvWebSer::RegisterMsgProc()
{
	map_webmsgproc_["LogIn"] = &av_web::AvWebLogIn;
	map_webmsgproc_["LogOut"] = &av_web::AvWebLogOut;

	map_webmsgproc_["GetDeviceCaps"] = &av_web::AvWebGetDeviceCaps;

	map_webmsgproc_["GetVideoEncodeCaps"] = &av_web::AvWebGetVideoEncodeCaps;
	map_webmsgproc_["GetVideoEncodeProfile"] = &av_web::AvWebGetVideoEncodeProfile;
	map_webmsgproc_["SetVideoEncodeProfile"] = &av_web::AvWebSetVideoEncodeProfile;

	map_webmsgproc_["GetImageProfile"] = &av_web::AvWebGetImageProfile;
	map_webmsgproc_["SetImageProfile"] = &av_web::AvWebSetImageProfile;

	map_webmsgproc_["CallPtzPt"] = &av_web::AvWebCallPtz;
	map_webmsgproc_["CallPtzZoom"] = &av_web::AvWebCallPtz;
	map_webmsgproc_["CallPtzFocus"] = &av_web::AvWebCallPtzFocus;
	map_webmsgproc_["CallPtzIris"] = &av_web::AvWebCallPtzIris;

	map_webmsgproc_["GetVideoCaptureCaps"] = &av_web::AvWebGetVideoCaptureCaps;
	map_webmsgproc_["GetVideoCaptureProfile"] = &av_web::AvWebGetVideoCaptureProfile;
	map_webmsgproc_["SetVideoCaptureProfile"] = &av_web::AvWebSetVideoCaptureProfile;

	map_webmsgproc_["GetRtmpProfile"] = &av_web::AvWebGetRtmpProfile;
	map_webmsgproc_["SetRtmpProfile"] = &av_web::AvWebSetRtmpProfile;
}


static void Delete_specific_char(std::string &str, const char cr)
{
	const char *src = str.c_str();
	
	char dest[4096] = {0};
	int i = 0, j = 0;
    while(src[i] != '\0')
    {
        if(src[i] != cr)
        {
            dest[j++] = src[i++];
        }
        else
        {
            i++;
        }
    }
    dest[j] = '\0';
	str = dest;
}

int CAvWebSer::WebCallBackDataProc(std::string InBuf, std::string &OutBuf)
{
	av_msg("CAvWebSer::WebCallBackProc:InBuf:\n%s\n", InBuf.c_str());

	CWebMsg msg_req;
	CWebMsgReader msg_req_reader;
	CWebMsg msg_resp;
	CWebMsg & msg_resp_header = msg_resp["Header"];

	if (false == msg_req_reader.parse(InBuf, msg_req)) {
		//TODO:请求消息解析失败
		av_msg("Web Parser jsom err\n");
		return 0;
	}
	
	CWebMsg & msg_req_header = msg_req["Header"];
	msg_resp_header["Method"] = msg_req_header["Method"].asString();
	msg_resp_header["Action"] = "RePly";
	//tf 测试先写一个假session
	msg_resp_header["Session"] = "24678";
	auto it = map_webmsgproc_.find(msg_req_header["Method"].asString());
	if (map_webmsgproc_.end() == it) {
		av_msg("No Web Msg Proc\n");
		msg_resp_header["Status"] = "Not suport method";
	} else {
		CWebMsg msg_resp_param;
		av_bool have_resp_param;

		int porc_ret = it->second(msg_req["Param"], msg_resp_param, have_resp_param);

		switch (porc_ret) {
		case WEB_STATUS_OK:{
			msg_resp_header["Status"] = "OK";
			if (have_resp_param) {
				msg_resp["Param"] = msg_resp_param;
			}
			break;
		}
		case WEB_STATUS_PARAM_ERROR:{
			msg_resp_header["Status"] = "Param error"; break;
		}
		case WEB_STATUS_SYSTEM_ERR:{
			msg_resp_header["Status"] = "System error"; break;
		}
		default:break;
		}
	}

	//OutBuf = msg_resp.isString() ? msg_resp.asString() : "Null";
	//OutBuf = msg_resp_writer.write(msg_resp);
#if 1
	//新jsoncpp库写法
	Json::FastWriter msg_resp_writer;
	OutBuf = msg_resp_writer.write(msg_resp);
#else
	Json::FastWriter msg_resp_writer(OutBuf);
	msg_resp_writer.write(msg_resp);
#endif
	av_msg("%d Resp: %s\n",OutBuf.size(), OutBuf.c_str());
	//Delete_specific_char(OutBuf, '\n'); 
	return 0;
}


int CAvWebSer::WebCallBackGetSnap(int Channel, std::string &SnapData)
{
	if (Channel >= (int)g_AvManCapture.GetAvCaptureTotal()){
		SnapData.clear();
		return 0;
	}
	Capture *CapSnap = g_AvManCapture.GetAvCaptureInstance(Channel);
	
	CAvPacket *CapSnapData = CapSnap->GetSnap();
	if (NULL == CapSnapData){
		SnapData.clear();
		return 0;
	}

	SnapData.assign((const char *)CapSnapData->GetRawBuffer(), CapSnapData->GetRawLength());
	CapSnapData->Release();
	return 0;
}