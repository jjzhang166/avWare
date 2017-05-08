#ifndef _LIB_INTELLIGENTANALYSIS_H_
#define _LIB_INTELLIGENTANALYSIS_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "AvWareType.h"
#include "AvEnum.h"
#include "AvSystemStruct.h"

av_bool AvIntelligentGetCaps(C_IntelligentCaps *caps);

av_bool AvIntelligentCreate();

av_bool AvIntelligentDestory();

av_bool AvIntelligentGetHandle(int type,void **handle);


#ifdef __cplusplus
}
#endif

#endif //_LIB_INTELLIGENTANALYSIS_H_