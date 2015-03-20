//
//  StarSystem.m
//  StariOSEnigne
//
//  Created by SAMRET WAJANASATHIAN on 9/06/2554.
//  Copyright 2554 SAMRET WAJANASATHIAN . All rights reserved.
//

#include "StarSystem.h"
#ifndef STAR_WINDOWS_VERSION
	#include <sys/time.h>
#endif

u32 STOS_GetTickCount(){
    
#ifdef STAR_WINDOWS_VERSION
    return GetTickCount();
#else
    static timeval tv;
    u32 milli;
    gettimeofday(&tv,NULL);
    milli = ((tv.tv_sec) * 1000) + ((tv.tv_usec)/1000);
    return milli;
#endif
}

StarVersion  version={2,87,0};
