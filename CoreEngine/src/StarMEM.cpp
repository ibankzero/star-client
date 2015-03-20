//
//  StarMEM.cpp
//  StarNDKEngine
//
//  Created by SAMRET WAJANASATHIAN on 1/6/2556.
//  Copyright (c) พ.ศ. 2556 SAMRET WAJANASATHIAN. All rights reserved.
//

#include "StarMEM.h"
#include "memory.h"
#include "assert.h"

#ifndef STAR_WINDOWS_VERSION
	#include <pthread.h>
#endif

#define MAX_MEMORY_TABLE      1024*256

static u8* s_pHeapMemory = NULL;
static u8* s_pStartAddr;
static u8* s_pEndAddr;

static char s_szFreeHeapString[16];
static u32 s_ulFreeHeapSize;
static u32 s_ulHeapSize;
static u32 s_ulMaxIdx;
//static bool s_bLockResource; // if call from thread;
#ifdef STAR_WINDOWS_VERSION
	static HANDLE s_MutexResource;
#else
	static pthread_mutex_t   s_MutexResource = PTHREAD_MUTEX_INITIALIZER;
#endif
u32 s_ulPhysicUse;
static bool s_bHaveFlagment;
struct MemoryBlockBase
{
#ifdef STAR_WINDOWS_VERSION
	LPVOID startAddr;
#else
    void* startAddr;
#endif
    u32   size;
    u32   skipSize;
    u32   lock;
};


MemoryBlockBase  MemoryBlock[MAX_MEMORY_TABLE];


void STOS_HeapInit(){
    
    s_pHeapMemory = NULL;
    s_bHaveFlagment = false;
    STOS_SetHeapSize(STOS_DEFAULT_HEAPSIZE);
    SLOG("========================================\n");
    SLOG("Heap Alloc %d\n",STOS_DEFAULT_HEAPSIZE);
#ifndef STAR_ANDROID_VERSION
    SLOG("Address 0x%p --> 0x%p\n",s_pHeapMemory,(u32*)(s_pHeapMemory + STOS_DEFAULT_HEAPSIZE));
#endif
    SLOG("========================================\n");
    s_ulPhysicUse = 0;

#ifdef STAR_WINDOWS_VERSION
	s_MutexResource = CreateMutex(NULL,FALSE,NULL);
#endif
    
}
bool STOS_SetHeapSize(u32 heapSize){
    STOS_FreeHeap();
    
    s_pHeapMemory = (u8*)malloc(heapSize);
    
    if(s_pHeapMemory)
    {
        s_ulHeapSize     = heapSize;
        memset(MemoryBlock,0,sizeof(MemoryBlock));
        s_pStartAddr     = s_pHeapMemory;
        s_pEndAddr       = s_pStartAddr + heapSize;
        s_ulFreeHeapSize = s_ulHeapSize;
        s_ulMaxIdx      = 0;
        //s_bLockResource  = false;
        return true;
    }
    else
    {
        SLOG("ERROR !!!!Allocate Heap Error\n");
        return false;
    }
}
bool STOS_FreeHeap(){
    if(s_pHeapMemory)
    {
        free(s_pHeapMemory);
        return true;
    }
    
    return false;
}

u32   STOS_GetMaxMEMBlock(){
    return MAX_MEMORY_TABLE;
}
u32   STOS_GetFreeMEMBlock(){
    return MAX_MEMORY_TABLE - s_ulMaxIdx;
}

static void _DefrageMemoryBlock(){
    
    int idx,deIdx,fIdx;
    int lastIdx = s_ulMaxIdx;
 //   bool foundFragment = false;
    idx = 0;
    //for (idx = 0; idx < lastIdx-1; idx++) {
        
    while (idx < lastIdx-1) {
      //  SLOG"Deflage Memory %d\n",idx);
        if(MemoryBlock[idx].lock == 0)
        {
            for (deIdx = idx+1; deIdx < lastIdx; deIdx++) {
                if(MemoryBlock[deIdx].lock == 0)
                {
                    MemoryBlock[idx].size += MemoryBlock[deIdx].size;
                    MemoryBlock[idx].skipSize += MemoryBlock[deIdx].skipSize;
                    s_ulMaxIdx--;
                }
                else
                {
                    break;
                }
            }
            
            for (fIdx = deIdx; fIdx < lastIdx; fIdx++) {
                MemoryBlock[idx+1+(fIdx - deIdx)] = MemoryBlock[fIdx];
            }
            
            lastIdx = s_ulMaxIdx;
            idx++;
        }
        else
        {
            idx++;
        }
    }
    
    s_bHaveFlagment = false;
}

static void _FreeMemoryBlock(int idx){
    MemoryBlock[idx].lock = 0;
    s_ulFreeHeapSize += MemoryBlock[idx].size + MemoryBlock[idx].skipSize;
    //ถ้าข้าง ๆ ก็ unlock ด้วย ก็ให้รวมเข้าด้วยกันไปเลย
    if(idx == 0 && s_ulMaxIdx==1)
    {
        s_ulMaxIdx = 0;
        return;
    }
    
    // first index merge
    if(idx == 0 && s_ulMaxIdx > 1)
    {
        if(MemoryBlock[idx+1].lock == 0)
        {
            // Merge 2 index together
            MemoryBlock[idx].size += MemoryBlock[idx+1].size;
            MemoryBlock[idx].skipSize += MemoryBlock[idx+1].skipSize;
            
            for (int i=idx+1; i<s_ulMaxIdx-1; i++) {
                MemoryBlock[i] = MemoryBlock[i+1];
            }
            
            s_ulMaxIdx--;
            return;
        }
    }
    
    // If last idx remove last idx
    if(idx == s_ulMaxIdx - 1)
    {
        // ติดกันก็ว่างด้วย
        if(MemoryBlock[idx-1].lock == 0)
            s_ulMaxIdx -=2;
        else
            s_ulMaxIdx--;
     
        return;
    }
    
   // SLOG"Free Memory Block %d\n",idx);
    
   // s_bHaveFlagment = true;
    
    
    // Between first and last idx
    // Remove on version 2.73 Auto at DefrageMemory Fuction
    if(MemoryBlock[idx+1].lock == 0)
    {
        // Merge 2 index together
        MemoryBlock[idx].size += MemoryBlock[idx+1].size;
        MemoryBlock[idx].skipSize += MemoryBlock[idx+1].skipSize;
        
        if(idx+1 < s_ulMaxIdx -1)
            memcpy(&MemoryBlock[idx+1], &MemoryBlock[idx+2], sizeof(MemoryBlockBase) * ( (s_ulMaxIdx - 1) - (idx+1)));
        //
        //for (int i=idx+1; i<s_ulMaxIdx-1; i++) {
        //    MemoryBlock[i] = MemoryBlock[i+1];
       // }
        
        s_ulMaxIdx--;
    }
    
    if(MemoryBlock[idx-1].lock == 0)
    {
        MemoryBlock[idx-1].size     += MemoryBlock[idx].size;
        MemoryBlock[idx-1].skipSize += MemoryBlock[idx].skipSize;
        
        if(idx < s_ulMaxIdx -1)
            memcpy(&MemoryBlock[idx], &MemoryBlock[idx+1], sizeof(MemoryBlockBase) * ( (s_ulMaxIdx - 1) - (idx)));
//        for (int i=idx; i<s_ulMaxIdx-1; i++) {
//            MemoryBlock[i] = MemoryBlock[i+1];
//        }
        
        s_ulMaxIdx--;
    }
}

static void _InsertMemoryBlock(int idx,u32 size,u32 skipSize){
    // Insert will sprite memory block and sprite to 2 block
    u32 tempSize     = MemoryBlock[idx].size - size;
    MemoryBlock[idx].size     = size;
    MemoryBlock[idx].skipSize = skipSize;
    MemoryBlock[idx].lock     = 1;
    // move all address
    for (int i=s_ulMaxIdx; i>idx+1; i--) {
        MemoryBlock[i] = MemoryBlock[i-1];
    }
    
    MemoryBlock[idx+1].startAddr= (u8*)MemoryBlock[idx].startAddr + size + skipSize;
    MemoryBlock[idx+1].size     = tempSize;
    MemoryBlock[idx+1].skipSize = 0;
    MemoryBlock[idx+1].lock     = 0;
    
    s_ulMaxIdx++;
    
}
#ifdef STAR_WINDOWS_VERSION
static LPVOID FindSpace(u32 size,u32 skipSize){
#else
static void* FindSpace(u32 size,u32 skipSize){
#endif   
    if(s_ulMaxIdx == 0) // mean first alloc
    {
        MemoryBlock[0].startAddr = s_pStartAddr;
        MemoryBlock[0].size      = size;
        MemoryBlock[0].skipSize  = skipSize;
        MemoryBlock[0].lock      = 1;
        s_ulFreeHeapSize -= size + skipSize;
        s_ulMaxIdx++;
        return MemoryBlock[0].startAddr;
    }
    
    if(s_ulMaxIdx == MAX_MEMORY_TABLE )
    {
        
        SLOG("Erorr Maximum Allocate TABLE\n");
 
        return NULL;
    }
#ifdef STAR_WINDOWS_VERSION
    LPVOID pFound = NULL;
#else
    void* pFound = NULL;
#endif
    u32 totalSize = size + skipSize;
    
    // file from LastIndex First; if not found find from free space index;
    u8* pBegin = (u8*)MemoryBlock[s_ulMaxIdx-1].startAddr + MemoryBlock[s_ulMaxIdx-1].size+MemoryBlock[s_ulMaxIdx-1].skipSize;
    
    if(s_pEndAddr - pBegin > totalSize)
    {
        MemoryBlock[s_ulMaxIdx].startAddr = pBegin;
        MemoryBlock[s_ulMaxIdx].size      = size;
        MemoryBlock[s_ulMaxIdx].skipSize  = skipSize;
        MemoryBlock[s_ulMaxIdx].lock      = 1;
        
    //    SLOG"Allocate At Idx %d\n",s_ulMaxIdx);
        s_ulFreeHeapSize -= totalSize;
        s_ulMaxIdx++;
#ifdef STAR_WINDOWS_VERSION
        pFound = (LPVOID)pBegin;
#else
        pFound = (void *)pBegin;
#endif
        return pFound;
    }
    else // Not Found find from Fragment Block
    {
        for(int i=0;i<s_ulMaxIdx;i++)
        {
            if(MemoryBlock[i].lock == 0)
            {
                if(MemoryBlock[i].size + MemoryBlock[i].skipSize == totalSize)
                {
                    pFound = MemoryBlock[i].startAddr ;
                    MemoryBlock[i].size     = size;
                    MemoryBlock[i].skipSize = skipSize;
                    MemoryBlock[i].lock     = 1;
                   // SLOG"Allocate At Idx %d\n",i);
                    
                    s_ulFreeHeapSize -= totalSize;
                    return pFound;
                }else
                    if(MemoryBlock[i].size + MemoryBlock[i].skipSize > totalSize)
                    {
                     //   SLOG"Allocate At Idx %d\n",i);
                        pFound = MemoryBlock[i].startAddr;
                        _InsertMemoryBlock(i,size,skipSize);
                        s_ulFreeHeapSize -= totalSize;
                        return pFound;
                    }
                
            }
        }
    }
    // Not Have Memory
    //#ifdef STAR_DEBUG
    SLOG("Out off Memory Free %d\n",s_ulFreeHeapSize);
    //#endif
    return NULL;
    
    
}

//void STOS_FragmentMemory(){
//    
//    int idxMerge=0;
//    int i;
//    for (i=0; i<s_ulMaxIdx; i++) {
//        
//    }
//    
//    
//    if(MemoryBlock[idx+1].lock == 0)
//    {
//        // Merge 2 index together
//        MemoryBlock[idx].size += MemoryBlock[idx+1].size;
//        MemoryBlock[idx].skipSize += MemoryBlock[idx+1].skipSize;
//
//        for (int i=idx+1; i<s_ulMaxIdx-1; i++) {
//            MemoryBlock[i] = MemoryBlock[i+1];
//        }
//
//        s_ulMaxIdx--;
//    }
//
//    if(MemoryBlock[idx-1].lock == 0)
//    {
//        MemoryBlock[idx-1].size     += MemoryBlock[idx].size;
//        MemoryBlock[idx-1].skipSize += MemoryBlock[idx].skipSize;
//
//        for (int i=idx; i<s_ulMaxIdx-1; i++) {
//            MemoryBlock[i] = MemoryBlock[i+1];
//        }
//        
//        s_ulMaxIdx--;
//    }
//}

#ifdef STAR_WINDOWS_VERSION
LPVOID STOS_Alloc(u32 size){
#else
void* STOS_Alloc(u32 size){
#endif
//	SLOG"Allocate %d\n",size);
    if(s_pHeapMemory == NULL)
    {
        SLOG("STOS_ALLOC Error: Not have Heap Momory\n");
        assert(s_pHeapMemory != NULL);
        return NULL;
    }
    
    if(size == 0) {
      return NULL;
    }
    
    if(size % 8 != 0)
    {
       // SLOG"Resize %d\n",size);
        size += 8 - (size % 8);
       // SLOG"New Size = %d\n",size);
    }
    
#ifdef STAR_WINDOWS_VERSION    
	WaitForSingleObject(s_MutexResource,INFINITE);
#else
    pthread_mutex_lock(&s_MutexResource);
#endif
//    while (s_bLockResource) {
//    }
//    
//    
//    
//    s_bLockResource = true;
    // wait for locking
   // if(s_bHaveFlagment)
   //     _DefrageMemoryBlock();
#ifdef STAR_WINDOWS_VERSION
    LPVOID pAlloc = FindSpace(size,0);
#else
    void *pAlloc = FindSpace(size,0);
#endif
    if(pAlloc == NULL)
    {
     //   SLOG"Can't Alloc %d %d\n",size,s_ulMaxIdx);
        SLOG("Out off Heap Momory size %d\n",size);
        assert(pAlloc != NULL);
    }
#ifdef STAR_WINDOWS_VERSION
	ReleaseMutex(s_MutexResource);
#else
    pthread_mutex_unlock(&s_MutexResource);
#endif
   // s_bLockResource = false;
    
   // SLOG"Allocate %d %p\n",size,pAlloc);
    return pAlloc;
    
}
#ifdef STAR_WINDOWS_VERSION
    s32 STOS_Free(LPVOID ptr)
#else
        s32 STOS_Free(void *ptr)
#endif
{
 //   int i;
    s32 freeSize = -1;
    if(s_pHeapMemory == NULL)
        return -1;

    if(ptr == NULL) return -1;
    
    if(ptr < s_pStartAddr || ptr > s_pEndAddr)
    {
        SLOG("Error Out off HEAP RANG\n");
        assert(0);
        return -1;
    }
    
#ifdef STAR_WINDOWS_VERSION    
	WaitForSingleObject(s_MutexResource,INFINITE);
#else
    pthread_mutex_lock(&s_MutexResource);
#endif

    int idxL = 0;
    u32 idxR = s_ulMaxIdx - 1;
    int idxC; // Center for search
    
    
    // Use Binary Search
    
    while (idxL != idxR - 1 && idxL != idxR) {
        idxC = (idxL + idxR) / 2;
        if(ptr < MemoryBlock[idxC].startAddr)//frameArray[idxC].frame)
        {
            idxR = idxC;
        }
        else
        {
            idxL = idxC;
        }
    }
    
    if(MemoryBlock[idxR].startAddr == ptr)
    {
        _FreeMemoryBlock(idxR);
        freeSize = MemoryBlock[idxR].size + MemoryBlock[idxR].skipSize;
    }
    else if(MemoryBlock[idxL].startAddr == ptr)
    {
        _FreeMemoryBlock(idxL);
        freeSize = MemoryBlock[idxL].size + MemoryBlock[idxL].skipSize;
        
    }
        
//    
//    
//    
//    for( i=0;i<s_ulMaxIdx;i++)
//    {
//        if(MemoryBlock[i].startAddr == ptr)
//        {
//            // LOGI("Free Block %d  Maxblock= %d",i,s_ulMaxIdx);
//            _FreeMemoryBlock(i);
//            freeSize = MemoryBlock[i].size + MemoryBlock[i].skipSize;
//            // LOGI("Done MaxxB=%d\n",s_ulMaxIdx);
//            break;
//        }
//    }
    
    // find on Heap Table
    //s_bLockResource = false;
#ifdef STAR_WINDOWS_VERSION
	ReleaseMutex(s_MutexResource);
#else
    pthread_mutex_unlock(&s_MutexResource);
#endif
#ifdef STAR_DEBUG
    if(freeSize == -1)
	{
        SLOG("Free Memory %p Error Not Found\n",ptr);
		assert(freeSize >0);
	}
#endif
    //SLOG"Free %d %p\n",freeSize,ptr);
    return  freeSize;
}

u32 STOS_GetFreeHeap(){
    return s_ulFreeHeapSize;
}
u32 STOS_GetMaxHeap(){
    return s_ulHeapSize;
}
u32 STOS_GetPhysicUseMem(){
//    static char phyString[64];
//    
//    if(s_ulPhysicUse >= 1000000)
//        sprintf(phyString,"%ld,%03d,%03d",s_ulPhysicUse/1000000,s_ulPhysicUse%1000000/1000,s_ulPhysicUse%1000);
//    else if(s_ulPhysicUse >=1000)
//        sprintf(phyString,"%ld,%03d",s_ulPhysicUse%1000000/1000,s_ulPhysicUse%1000);
//    else 
//        sprintf(phyString,"%ld",s_ulPhysicUse%1000);
//    
//    
//    return phyString;
    return s_ulPhysicUse;
}
char* STOS_GetFreeHeapString(){
    
    if(s_ulFreeHeapSize >= 1000000)
        sprintf(s_szFreeHeapString,"%d,%03d,%03d",s_ulFreeHeapSize/1000000,s_ulFreeHeapSize%1000000/1000,s_ulFreeHeapSize%1000);
    else if(s_ulFreeHeapSize >=1000)
        sprintf(s_szFreeHeapString,"%d,%03d",s_ulFreeHeapSize%1000000/1000,s_ulFreeHeapSize%1000);
    else if(s_ulFreeHeapSize)
        sprintf(s_szFreeHeapString,"%d",s_ulFreeHeapSize%1000);
    
    
    return s_szFreeHeapString;
    
    
}