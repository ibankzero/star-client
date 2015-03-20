//
//  StarMath.h
//  StarEngine
//
//  Created by SAMRET WAJANASATHIAN on 16/4/2556.
//
//

#ifndef __StarEngine__StarMath__
#define __StarEngine__StarMath__


#include "StarType.h"
#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>

// CRC 
//----------------------------------------------------------------------------
// Constant Definitions
//----------------------------------------------------------------------------
    
#define STAR_CRC8_STANDARD_POLY     0x07
#define STAR_CRC8_STANDARD_INIT     0
    
#define STAR_CRC16_STANDARD_POLY    0xa001      // Items that carry out bit inversion also invert generator polynomials.
#define STAR_CRC16_STANDARD_INIT    0
#define STAR_CRC16_CCITT_POLY       0x1021
#define STAR_CRC16_CCITT_INIT       0xffff
    
#define STAR_CRC32_STANDARD_POLY    0xedb88320  // Items that carry out bit inversion also invert generator polynomials.
#define STAR_CRC32_STANDARD_INIT    0xffffffff
#define STAR_CRC32_POSIX_POLY       0x04c11db7
#define STAR_CRC32_POSIX_INIT       0

EXTERN_C_BEGIN
    WIN_DLL void    STOS_InitCRCTable(); // Auto Init CRC 8 16 and 32 Table
EXTERN_C_END

u8      STOS_CRC8(const void    *data,u32 dataLength);
u16     STOS_CRC16(const void   *data,u16 dataLength);
u32     STOS_CRC32(const void   *data,u32 dataLength);
// VECTOR 3
struct STVEC3_{
    f32 x;
    f32 y;
    f32 z;  
};
    
typedef struct STVEC3_ STVEC3;

struct STMTX33_{
    // ROW = 3
    // Colum = 3
    f32 data[3][3];
};

typedef struct STMTX33_ STMTX33;
    
struct STMTX34_{
    // ROW = 3
    // COLUM = 4
    f32 data[3][4];
};
    
typedef struct STMTX34_ STMTX34;

struct STMTX44_{
    f32 data[4][4];
};
    
typedef struct STMTX44_ STMTX44;
//==============================================
// MTX33
//==============================================
void MTX33FillData(STMTX33 *mtxOut,f32* glData);
void MTX33Identity(STMTX33 *mtx); // set to identity matrix
//==============================================
// MTX34
//==============================================
void MTX34FillData(STMTX34 *mtxOut,f32* glData);
void MTX34Identity(STMTX34 *mtx); // set to identity matrix
int  MTX34Inverse(const STMTX34 *mtxIn,STMTX34 *mtxOut);
void MTX34Copy(const STMTX34 *src,STMTX34 *des);
void MTX34ShowData(const STMTX34 *data);
void MTX34ScaleApply(const STMTX34 *src,STMTX34 *des,f32 xS,f32 yS,f32 zS);
//==============================================
// MTX44
//==============================================
void MTX44FillData(STMTX44 *mtxOut,f32* glData);
void MTX44Identity(STMTX44 *mtx); // set to identity matrix
int  MTX44Inverse(const STMTX44 *mtxIn,STMTX44 *mtxOut);
void MTX44Copy(const STMTX44 *src,STMTX44 *des);
void MTX44Concat(const STMTX44 *a,const STMTX44 *b,STMTX44 *ab);
void MTX44Transpose(const STMTX44 * src,STMTX44 *xPos);
void MTX44Trans(STMTX44 *mOut,f32 xT,f32 yT,f32 zT);
void MTX44TransApply(const STMTX44 *src,STMTX44 *des,f32 xT,f32 yT,f32 zT);
void MTX44Scale(STMTX44 *mOut,f32 xS,f32 yS,f32 zS);
void MTX44ScaleApply(const STMTX44 *src,STMTX44 *des,f32 xS,f32 yS,f32 zS);
    
    
    
STVEC3* VEC3Normalize(STVEC3* p1,STVEC3* pOut);
STVEC3* VEC3Invert(STVEC3* p1,STVEC3* pOut);
STVEC3* VEC3Add(STVEC3* p1,STVEC3* p2,STVEC3* pOut);
STVEC3* VEC3Sub(STVEC3* p1,STVEC3* p2,STVEC3* pOut);
STVEC3* VEC3Multi(STVEC3* p1,STVEC3* p2,STVEC3* pOut);
STVEC3* VEC3Scale(STVEC3* p,f32 scale,STVEC3* pOut);
STVEC3* VEC3Cross(STVEC3* p1,STVEC3 *p2,STVEC3* pOut);
STVEC3* VEC3Lerp(STVEC3* p1,STVEC3* p2,f32 t,STVEC3* pOut); // Don't know
STVEC3* VEC3AddScale(STVEC3* p1,STVEC3* pAdd,f32 scale,STVEC3* pOut);
f32     VEC3Dot(const STVEC3* p1, const STVEC3* p2);
f32     VEC3SquareLen(const STVEC3* p);
f32     VEC3Len(const STVEC3* p); // or Magnitude Vector
f32     VEC3Dist( STVEC3* p1,STVEC3* p2 );
f32     VEC3SquareDist(STVEC3* p1,STVEC3* p2);
STVEC3* VEC3Transform(STVEC3* pOut,const STMTX34* pM,const STVEC3 *pV);
    
#endif /* defined(__StarEngine__StarMath__) */
