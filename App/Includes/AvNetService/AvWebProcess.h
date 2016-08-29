#ifndef AVWARE_AVNETSERVICE_AVWEBPROCESS_H_
#define AVWARE_AVNETSERVICE_AVWEBPROCESS_H_

#include "jsoncpp/json.h"
#include "Apis/AvWareType.h"

typedef Json::Value CWebMsg;
typedef Json::Reader CWebMsgReader;

typedef int(*WebMsgProc)(CWebMsg &, CWebMsg &, av_bool &);

enum{
	WEB_STATUS_OK = 0,
	WEB_STATUS_PARAM_ERROR,
	WEB_STATUS_NO_SUPPORT_METHOD,
	WEB_STATUS_NO_SUPPORT_ACTION,
	WEB_STATUS_SYSTEM_ERR,
};

namespace av_web{

template <typename EnumType>
struct EnumNameStr
{
    static const char* List[];
};


template<typename EnumType>
EnumType ConvertStringToEnum(const char* pStr)
{
    EnumType fooEnum = static_cast<EnumType>(-1);
	if (nullptr == pStr) { return fooEnum; }

    int count = sizeof(EnumNameStr<EnumType>::List) /
        sizeof(EnumNameStr<EnumType>::List[0]);
    for(int i = 0; i < count; ++i) {
		//printf("%s\n", EnumNameStr<EnumType>::List[i]);
        if (!strcmp(pStr, EnumNameStr<EnumType>::List[i])) {
            fooEnum = static_cast<EnumType>(i);
            break;
        }
    }
    return fooEnum;
}

//使用这个模板函数要求枚举类型的值必须从0开始 而且值要连续
template<typename EnumType>
const char * ConvertEnumToString(EnumType em)
{
    int count = sizeof(EnumNameStr<EnumType>::List) /
        sizeof(EnumNameStr<EnumType>::List[0]);
	if ((0 > em) || (em > count)) {
		av_error("Enum out of rank");
		return "";
		//return nullptr;
	}
	return EnumNameStr<EnumType>::List[em];
}


//LogIn
int AvWebLogIn(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);
//LogOut
int AvWebLogOut(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);

//GetDeviceCaps
int AvWebGetDeviceCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);
//GetVideoEncodeCaps
int AvWebGetVideoEncodeCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);

//GetVideoEncodeProfile
int AvWebGetVideoEncodeProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);
//SetVideoEncodeProfile
int AvWebSetVideoEncodeProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);

//GetImageProfile
int AvWebGetImageProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);
//SetImageProfile
int AvWebSetImageProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);

//CallPtzPt  CallPtzZoom  CallPtzFocus  CallPtzIris
int AvWebCallPtz(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);
int AvWebCallPtzFocus(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);
int AvWebCallPtzIris(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);

//GetVideoCaptureCaps  
int AvWebGetVideoCaptureCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);
//GetVideoCaptureProfile
int AvWebGetVideoCaptureProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);
//SetVideoCaptureProfile
int AvWebSetVideoCaptureProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param);
}

#endif //AVWARE_AVNETSERVICE_AVWEBPROCESS_H_