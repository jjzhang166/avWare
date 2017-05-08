#ifndef _BLOCK_QUEUE_H_
#define _BLOCK_QUEUE_H_

#include <deque>
#include "bsp_face_detect_demo.h"

class BlockQueenFaceDetect{
public:
	BlockQueenFaceDetect();
	~BlockQueenFaceDetect();
	void put(const pFaceResult &face_result);
	pFaceResult take(void);
	int length();
private:
	MutexLock mutex_;
	PthreadCondition cond_;
	std::deque<pFaceResult> queue_;
};

extern "C"{
	void* BlockQueenFaceDetectFactory();
}
#endif //_BLOCK_QUEUE_H_