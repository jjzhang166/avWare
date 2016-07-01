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
|  作者:
|  日期:
|  说明:
******************************************************************/
#include "AvMemoryPool/AvMemoryPool.h"
SINGLETON_IMPLEMENT(CAvMemoryPool)

#define SYNC_MEM 0x98763
#define AssertMem(head) assert (head->sync == SYNC_MEM)
typedef struct {
	av_u32 size;
	av_u32 sync;
}MemDumpHead;

av_bool CAvMemoryPool::Initialize()
{
	ptrMemPool = new MemPool::CMemoryPool(D_AVMEMORYPOOL_SIZE, D_AVMEMORYPOOL_CHUNK_SIZE);
	return av_true;
}
av_void *CAvMemoryPool::malloc(size_t size)
{
	av_char *ptr = NULL;
	ptr = (av_char *)ptrMemPool->GetMemory(size + sizeof(MemDumpHead));
	if (NULL == ptr) return NULL;
	MemDumpHead *pHead = (MemDumpHead *)ptr;
	pHead->size = size + sizeof(MemDumpHead);
	pHead->sync = SYNC_MEM;
	return (av_void *)(ptr + sizeof(MemDumpHead));
}
av_void *CAvMemoryPool::realloc(av_void *ptr, size_t size)
{
	av_char *lptr = (av_char *)((av_char *)ptr - sizeof(MemDumpHead));
	MemDumpHead *pHead = (MemDumpHead *)lptr;
	AssertMem(pHead);
	if (pHead->size >= size){
		return ptr;
	}

	av_char *newPtr = (av_char *)CAvMemoryPool::malloc(size);
	if (NULL == newPtr) {
		av_error("CAvMemoryPool::malloc return NULL \n");
		return NULL;
	}
	memcpy(newPtr, ptr, pHead->size - sizeof(MemDumpHead));
	CAvMemoryPool::free(ptr);

	return newPtr;
}
av_void CAvMemoryPool::free(av_void *ptr)
{
	av_char *lptr = (av_char *)((av_char *)ptr - sizeof(MemDumpHead));
	MemDumpHead *pHead = (MemDumpHead *)lptr;
	AssertMem(pHead);
	ptrMemPool->FreeMemory(lptr, pHead->size);
}
av_void CAvMemoryPool::dump()
{

}
av_void *CAvMemoryPool::GetRealAddr(av_void *ptr)
{
	av_char *lptr = (av_char *)((av_char *)ptr - sizeof(MemDumpHead));
	MemDumpHead *pHead = (MemDumpHead *)lptr;
	AssertMem(pHead);
	return lptr;
}
av_u32   CAvMemoryPool::GetMemSize(av_void *ptr)
{
	av_char *lptr = (av_char *)((av_char *)ptr - sizeof(MemDumpHead));
	MemDumpHead *pHead = (MemDumpHead *)lptr;
	AssertMem(pHead);

	return pHead->size;
}
CAvMemoryPool::CAvMemoryPool()
{
	ptrMemPool = NULL;
}
CAvMemoryPool::~CAvMemoryPool()
{

}


av_void *AvMemoryPoolMalloc(size_t size)
{
	return g_AvMemoryPool.malloc(size);
}
av_void  AvMemoryPoolFree(av_void *ptr)
{
	g_AvMemoryPool.free(ptr);
}
av_void *AvMemoryPoolRealloc(av_void *ptr, size_t size)
{
	return g_AvMemoryPool.realloc(ptr, size);
}