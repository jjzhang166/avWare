#ifndef _BSP_FACE_DETECT_DEMO_H_
#define _BSP_FACE_DETECT_DEMO_H_

#ifndef _MSC_VER
#include <sys/time.h>
#endif
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "Apis/AvWareCplusplus.h"

typedef struct{
	C_RECT face_rect;
	std::string face_img;
}face_date;

typedef struct{
	struct timeval timestamp;
	int face_count;
	std::string all_view_img;
	std::vector<std::shared_ptr<face_date>> face_result;
}FaceResult;
typedef std::shared_ptr<FaceResult> pFaceResult;

class BlockQueenFaceDetect;

class CFaceDetectDemoProcess{
public:
	CFaceDetectDemoProcess();
	~CFaceDetectDemoProcess();
	
	int Start(const int max_width, const int max_height);
	int Stop();

private:
	static void* ThreadProcProxy(void*arg);
	void ThreadProc();

private:
	int max_width_srcyuv_;
	int max_height_srcyuv_;
	int jpg_quality_;

	unsigned char *cap_yuv_data_;
	unsigned char *jpg_data_;
	unsigned char *crop_yuv_data_;
	BlockQueenFaceDetect *block_queue_;

	pthread_t pid_;
};

#endif //_BSP_FACE_DETECT_DEMO_H_