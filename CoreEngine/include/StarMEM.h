//
//  StarMEM.h
//  StarNDKEngine
//
//  Created by SAMRET WAJANASATHIAN on 1/6/2556.
//  Copyright (c) พ.ศ. 2556 SAMRET WAJANASATHIAN. All rights reserved.
//

#ifndef __StarNDKEngine__StarMEM__
#define __StarNDKEngine__StarMEM__

#include <stdio.h>
#include <stdlib.h>
#include "StarType.h"
#define STOS_DEFAULT_HEAPSIZE       1024*1024*128

EXTERN_C_BEGIN
    WIN_DLL void STOS_HeapInit();
EXTERN_C_END

bool STOS_SetHeapSize(u32 heapSize);
bool STOS_FreeHeap(); // true if can free heap. false if no heap to free
#ifdef STAR_WINDOWS_VERSION
	LPVOID STOS_Alloc(u32 size);
    s32   STOS_Free(LPVOID ptr);
#else
	void* STOS_Alloc(u32 size);
    s32   STOS_Free(void *ptr);
#endif
u32   STOS_GetMaxMEMBlock();
u32   STOS_GetFreeMEMBlock();
u32   STOS_GetFreeHeap();
u32   STOS_GetMaxHeap();
char* STOS_GetFreeHeapString();
u32   STOS_GetPhysicUseMem();
void  STOS_FragmentMemory();  
#endif /* defined(__StarNDKEngine__StarMEM__) */

