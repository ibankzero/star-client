//
//  StarSystem.h
//  StariOSEnigne
//
//  Created by SAMRET WAJANASATHIAN on 9/06/2554.
//  Copyright 2554 SAMRET WAJANASATHIAN . All rights reserved.
//

#ifndef _STAR_ENGINE_SYSTEM_
#define _STAR_ENGINE_SYSTEM_

#include "StarType.h"
#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>
#include <time.h>

#define STAR_TYPE_NONE              0x00
#define STAR_TYPE_IPHONE            0x01
#define STAR_TYPE_IPHONE_4          0x02
#define STAR_TYPE_IPHONE_UNIVER     0x03 // Normal and Retina
#define STAR_TYPE_IPAD              0x04
#define STAR_TYPE_UNIVERSAL         0x05 // iPhone,iPad, Retina
#define STAR_TYPE_MACAPP            0x06
#define STAR_TYPE_IPADS             0x07 // iPad Spacial Convert Retina to iPad (ใส่กรอบให้ App).
#define STAR_TYPE_IPADC             0x08 // iPod Spacial Convert iPad to iPod (ลดขนาดจอให้เท่าขนาด iPod).
#define STAR_TYPE_UNIVERSAL_LAYOUT  0x09 // iPod Retina Layout (auto convert retina to ipod).
#define STAR_TYPE_IPAD_LAYOUT       0x10 // iPad Layout
#define STAR_TYPE_UNIWii_LAYOUT     0x11 // iPod Wii Convert Wii Layout to iPod Retina (auto convert retina to ipod).
#define STAR_TYPE_IPADWii_LAYOUT    0x12 // iPad Wii Convert Vii Layout to iPad
#define STAR_TYPE_IPAD_RETINA       0x13 // Retina To iPad โดยการขยายจอ.
#define STAR_TYPE_LAYOUT            0x14 // Don't use this type it auto for init2DLayout
#define STAR_TYPE_MAC_VIEW          0x15

#define STAR_TYPE_ANDROID           0x15
#define STAR_TYPE_IPHONE_5          0x16

#define STAR_TYPE_FLASH             0x17

#define STAR_TYPE_IPHONE_6          0x18
#define STAR_TYPE_IPHONE_6_PLUS     0x19

#define STOS_IMAGE_GIF               0x0001
#define STOS_IMAGE_PNG               0x0002
#define STOS_IMAGE_JPG               0x0003
#define STOS_IMAGE_VOID              0x0004

u32    STOS_GetTickCount(); // Milli Second

#endif