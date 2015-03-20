//
//  StarMath.cpp
//  StarEngine
//
//  Created by SAMRET WAJANASATHIAN on 16/4/2556.
//
//
#include "StarMath.h"
#include "math.h"
#pragma mark -
#pragma mark CRC
#pragma mark -
static u8  s_CRC8Table[256];
static u16 s_CRC16Table[256];
static u32 s_CRC32Table[256];

void _InitCRC8Table(u8 poly);
void _InitCRC16Table(u16 poly);
void _InitCRC32Table(u32 poly);

void _InitCRC8Table(u8 poly){
    u32     r;
    u32     i, j;
    u8     *t = s_CRC8Table;
    
    for (i = 0; i < 256; i++)
    {
        r = i;
        for (j = 0; j < 8; j++)
        {
            if (r & 0x80)
            {
                r = (r << 1) ^ poly;
            }
            else
            {
                r <<= 1;
            }
        }
        t[i] = (u8)r;
    }
    
}

void _InitCRC16Table(u16 poly){
    u32     r;
    u32     i, j;
    u16    *t = s_CRC16Table;
    
    for (i = 0; i < 256; i++)
    {
        r = i << 8;
        for (j = 0; j < 8; j++)
        {
            if (r & 0x8000)
            {
                r = (r << 1) ^ poly;
            }
            else
            {
                r <<= 1;
            }
        }
        t[i] = (u16)r;
    }
}

void _InitCRC32Table(u32 poly){
    u32     r;
    u32     i, j;
    u32    *t = s_CRC32Table;
    
    for (i = 0; i < 256; i++)
    {
        r = i << 24;
        for (j = 0; j < 8; j++)
        {
            if (r & 0x80000000U)
            {
                r = (r << 1) ^ poly;
            }
            else
            {
                r <<= 1;
            }
        }
        t[i] = r;
    }
}

void STOS_InitCRCTable(){
    _InitCRC8Table(STAR_CRC8_STANDARD_POLY);
    _InitCRC16Table(STAR_CRC16_STANDARD_POLY);
    _InitCRC32Table(STAR_CRC32_STANDARD_POLY);
}
u8      STOS_CRC8(const void    *input,u32 dataLength){
    u8 context = STAR_CRC8_STANDARD_INIT;
    u32     r;
    u32     i;
    const u8 *t = s_CRC8Table;
    u8     *data = (u8 *)input;
    
    r = (u32)context;
    for (i = 0; i < dataLength; i++) {
        r = t[(r ^ *data) & 0xff];
        data++;
    }
    context = (u8)r;
    return context;
}
u16     STOS_CRC16(const void   *input,u16 dataLength){
    u16     context = STAR_CRC16_STANDARD_INIT;
    u32     r;
    u32     i;
    const u16 *t = s_CRC16Table;
    u8     *data = (u8 *)input;
    
    r = (u32)context;
    for (i = 0; i < dataLength; i++)
    {
        r = (r << 8) ^ t[((r >> 8) ^ *data) & 0xff];
        data++;
    }
    context = (u16) r;
    
    return context;
}
u32     STOS_CRC32(const void   *input,u32 dataLength){
    u32     context = STAR_CRC32_STANDARD_INIT;
    u32     r;
    u32     i;
    const u32 *t = s_CRC32Table;
    u8     *data = (u8 *)input;
    
    r = (u32)context;
    for (i = 0; i < dataLength; i++)
    {
        r = (r << 8) ^ t[((r >> 24) ^ *data) & 0xff];
        data++;
    }
    context = (u32) r;
    return context;
}

#pragma mark -
#pragma mark MTX33
#pragma mark -
void MTX33FillData(STMTX33 *mtxOut,f32* glData){
    mtxOut->data[0][0] = glData[0]; mtxOut->data[0][1] = glData[4]; mtxOut->data[0][2] = glData[8];
    mtxOut->data[1][0] = glData[1]; mtxOut->data[1][1] = glData[5]; mtxOut->data[1][2] = glData[9];
    mtxOut->data[2][0] = glData[2]; mtxOut->data[2][1] = glData[6]; mtxOut->data[2][2] = glData[10];
    
}

void MTX33Identity(STMTX33 *mtx){
    mtx->data[0][0] = 1.0f;  mtx->data[0][1] = 0.0f;  mtx->data[0][2] = 0.0f;
    mtx->data[1][0] = 0.0f;  mtx->data[1][1] = 1.0f;  mtx->data[1][2] = 0.0f;
    mtx->data[2][0] = 0.0f;  mtx->data[2][1] = 0.0f;  mtx->data[2][2] = 1.0f;
    
}
#pragma mark -
#pragma mark MTX34
#pragma mark -
void MTX34Copy(const STMTX34 *src,STMTX34 *des){
    
    des->data[0][0] = src->data[0][0];  des->data[0][1] = src->data[0][1]; des->data[0][2] = src->data[0][2];  des->data[0][3] = src->data[0][3];
    des->data[1][0] = src->data[1][0];  des->data[1][1] = src->data[1][1]; des->data[1][2] = src->data[1][2];  des->data[1][3] = src->data[1][3];
    des->data[2][0] = src->data[2][0];  des->data[2][1] = src->data[2][1]; des->data[2][2] = src->data[2][2];  des->data[2][3] = src->data[2][3];
    
}

void MTX34FillData(STMTX34 *mtxOut,f32* glData){
    mtxOut->data[0][0] = glData[0]; mtxOut->data[0][1] = glData[4]; mtxOut->data[0][2] = glData[8];   mtxOut->data[0][3] = glData[12];
    mtxOut->data[1][0] = glData[1]; mtxOut->data[1][1] = glData[5]; mtxOut->data[1][2] = glData[9];   mtxOut->data[1][3] = glData[13];
    mtxOut->data[2][0] = glData[2]; mtxOut->data[2][1] = glData[6]; mtxOut->data[2][2] = glData[10];  mtxOut->data[2][3] = glData[14];
    
}

void MTX34Identity(STMTX34 *mtx){
    mtx->data[0][0] = 1.0f;  mtx->data[0][1] = 0.0f;  mtx->data[0][2] = 0.0f;  mtx->data[0][3] = 0.0f;
    mtx->data[1][0] = 0.0f;  mtx->data[1][1] = 1.0f;  mtx->data[1][2] = 0.0f;  mtx->data[1][3] = 0.0f;
    mtx->data[2][0] = 0.0f;  mtx->data[2][1] = 0.0f;  mtx->data[2][2] = 1.0f;  mtx->data[2][3] = 0.0f;
    
}

int MTX34Inverse(const STMTX34 *mtxIn, STMTX34 *mtxOut){
    
    
    STMTX34  mTmp;
    STMTX34 *m;
    f32 det;
    
    
    if( mtxIn == mtxOut )
    {
        m = &mTmp;
    }
    else
    {
        m = mtxOut;
    }
    
    
    // compute the determinant of the upper 3x3 submatrix
    det =   mtxIn->data[0][0]*mtxIn->data[1][1]*mtxIn->data[2][2] + mtxIn->data[0][1]*mtxIn->data[1][2]*mtxIn->data[2][0] + mtxIn->data[0][2]*mtxIn->data[1][0]*mtxIn->data[2][1]
    - mtxIn->data[2][0]*mtxIn->data[1][1]*mtxIn->data[0][2] - mtxIn->data[1][0]*mtxIn->data[0][1]*mtxIn->data[2][2] - mtxIn->data[0][0]*mtxIn->data[2][1]*mtxIn->data[1][2];
    
    
    // check if matrix is singular
    if( det == 0.0f )
    {
        return 0;
    }
    
    
    // compute the inverse of the upper submatrix:
    
    // find the transposed matrix of cofactors of the upper submatrix
    // and multiply by (1/det)
    
    det = 1.0f / det;
    
    
    m->data[0][0] =  (mtxIn->data[1][1]*mtxIn->data[2][2] - mtxIn->data[2][1]*mtxIn->data[1][2]) * det;
    m->data[0][1] = -(mtxIn->data[0][1]*mtxIn->data[2][2] - mtxIn->data[2][1]*mtxIn->data[0][2]) * det;
    m->data[0][2] =  (mtxIn->data[0][1]*mtxIn->data[1][2] - mtxIn->data[1][1]*mtxIn->data[0][2]) * det;
    
    m->data[1][0] = -(mtxIn->data[1][0]*mtxIn->data[2][2] - mtxIn->data[2][0]*mtxIn->data[1][2]) * det;
    m->data[1][1] =  (mtxIn->data[0][0]*mtxIn->data[2][2] - mtxIn->data[2][0]*mtxIn->data[0][2]) * det;
    m->data[1][2] = -(mtxIn->data[0][0]*mtxIn->data[1][2] - mtxIn->data[1][0]*mtxIn->data[0][2]) * det;
    
    m->data[2][0] =  (mtxIn->data[1][0]*mtxIn->data[2][1] - mtxIn->data[2][0]*mtxIn->data[1][1]) * det;
    m->data[2][1] = -(mtxIn->data[0][0]*mtxIn->data[2][1] - mtxIn->data[2][0]*mtxIn->data[0][1]) * det;
    m->data[2][2] =  (mtxIn->data[0][0]*mtxIn->data[1][1] - mtxIn->data[1][0]*mtxIn->data[0][1]) * det;
    
    
    // compute (invA)*(-C)
    m->data[0][3] = -m->data[0][0]*mtxIn->data[0][3] - m->data[0][1]*mtxIn->data[1][3] - m->data[0][2]*mtxIn->data[2][3];
    m->data[1][3] = -m->data[1][0]*mtxIn->data[0][3] - m->data[1][1]*mtxIn->data[1][3] - m->data[1][2]*mtxIn->data[2][3];
    m->data[2][3] = -m->data[2][0]*mtxIn->data[0][3] - m->data[2][1]*mtxIn->data[1][3] - m->data[2][2]*mtxIn->data[2][3];
    
    // copy back if needed
    if( m == &mTmp )
    {
        MTX34Copy( &mTmp,mtxOut);
    }
    
    return 1;
}
void MTX34ShowData(const STMTX34 *data){
    printf("00=%0.4f 01=%0.4f 02=%0.4f 03=%0.4f\n10=%0.4f 11=%0.4f 12=%0.4f 13=%0.4f\n20=%0.4f 21=%0.4f 22=%0.4f 23=%0.4f\n",
           data->data[0][0],data->data[0][1],data->data[0][2],data->data[0][3],data->data[1][0],data->data[1][1],data->data[1][2],data->data[1][3],
           data->data[2][0],data->data[2][1],data->data[2][2],data->data[2][3]);
}

void MTX34ScaleApply(const STMTX34 *src,STMTX34 *des,f32 xS,f32 yS,f32 zS){
    des->data[0][0] = src->data[0][0] * xS;      des->data[0][1] = src->data[0][1] * xS;
    des->data[0][2] = src->data[0][2] * xS;      des->data[0][3] = src->data[0][3] * xS;
    
    des->data[1][0] = src->data[1][0] * yS;      des->data[1][1] = src->data[1][1] * yS;
    des->data[1][2] = src->data[1][2] * yS;      des->data[1][3] = src->data[1][3] * yS;
    
    des->data[2][0] = src->data[2][0] * zS;      des->data[2][1] = src->data[2][1] * zS;
    des->data[2][2] = src->data[2][2] * zS;      des->data[2][3] = src->data[2][3] * zS;
}

#pragma mark -
#pragma mark MTX44
#pragma mark -
void MTX44Concat(const STMTX44 *a,const STMTX44 *b,STMTX44 *ab){
    
    STMTX44       mTmp;
    STMTX44       *pM;
    
    if( (ab == a) || (ab == b) )
    {
        pM = &mTmp;
    }
    else
    {
        pM = ab;
    }
    
    // compute (a x b) -> m
    
    pM->data[0][0] = a->data[0][0]*b->data[0][0] + a->data[0][1]*b->data[1][0] + a->data[0][2]*b->data[2][0] + a->data[0][3]*b->data[3][0];
    pM->data[0][1] = a->data[0][0]*b->data[0][1] + a->data[0][1]*b->data[1][1] + a->data[0][2]*b->data[2][1] + a->data[0][3]*b->data[3][1];
    pM->data[0][2] = a->data[0][0]*b->data[0][2] + a->data[0][1]*b->data[1][2] + a->data[0][2]*b->data[2][2] + a->data[0][3]*b->data[3][2];
    pM->data[0][3] = a->data[0][0]*b->data[0][3] + a->data[0][1]*b->data[1][3] + a->data[0][2]*b->data[2][3] + a->data[0][3]*b->data[3][3];
    
    pM->data[1][0] = a->data[1][0]*b->data[0][0] + a->data[1][1]*b->data[1][0] + a->data[1][2]*b->data[2][0] + a->data[1][3]*b->data[3][0];
    pM->data[1][1] = a->data[1][0]*b->data[0][1] + a->data[1][1]*b->data[1][1] + a->data[1][2]*b->data[2][1] + a->data[1][3]*b->data[3][1];
    pM->data[1][2] = a->data[1][0]*b->data[0][2] + a->data[1][1]*b->data[1][2] + a->data[1][2]*b->data[2][2] + a->data[1][3]*b->data[3][2];
    pM->data[1][3] = a->data[1][0]*b->data[0][3] + a->data[1][1]*b->data[1][3] + a->data[1][2]*b->data[2][3] + a->data[1][3]*b->data[3][3];
    
    pM->data[2][0] = a->data[2][0]*b->data[0][0] + a->data[2][1]*b->data[1][0] + a->data[2][2]*b->data[2][0] + a->data[2][3]*b->data[3][0];
    pM->data[2][1] = a->data[2][0]*b->data[0][1] + a->data[2][1]*b->data[1][1] + a->data[2][2]*b->data[2][1] + a->data[2][3]*b->data[3][1];
    pM->data[2][2] = a->data[2][0]*b->data[0][2] + a->data[2][1]*b->data[1][2] + a->data[2][2]*b->data[2][2] + a->data[2][3]*b->data[3][2];
    pM->data[2][3] = a->data[2][0]*b->data[0][3] + a->data[2][1]*b->data[1][3] + a->data[2][2]*b->data[2][3] + a->data[2][3]*b->data[3][3];
    
    pM->data[3][0] = a->data[3][0]*b->data[0][0] + a->data[3][1]*b->data[1][0] + a->data[3][2]*b->data[2][0] + a->data[3][3]*b->data[3][0];
    pM->data[3][1] = a->data[3][0]*b->data[0][1] + a->data[3][1]*b->data[1][1] + a->data[3][2]*b->data[2][1] + a->data[3][3]*b->data[3][1];
    pM->data[3][2] = a->data[3][0]*b->data[0][2] + a->data[3][1]*b->data[1][2] + a->data[3][2]*b->data[2][2] + a->data[3][3]*b->data[3][2];
    pM->data[3][3] = a->data[3][0]*b->data[0][3] + a->data[3][1]*b->data[1][3] + a->data[3][2]*b->data[2][3] + a->data[3][3]*b->data[3][3];
    
    // overwrite a or b if needed
    if(pM == &mTmp)
    {
        MTX44Copy(pM, ab);
    }
}

void MTX44Transpose(const STMTX44 * src,STMTX44 *xPos){
    STMTX44       mTmp;
    STMTX44*      pM;
    
    
    if(src == xPos)
    {
        pM = &mTmp;
    }
    else
    {
        pM = xPos;
    }
    
    
    pM->data[0][0] = src->data[0][0];    pM->data[0][1] = src->data[1][0];    pM->data[0][2] = src->data[2][0];    pM->data[0][3] = src->data[3][0];
    pM->data[1][0] = src->data[0][1];    pM->data[1][1] = src->data[1][1];    pM->data[1][2] = src->data[2][1];    pM->data[1][3] = src->data[3][1];
    pM->data[2][0] = src->data[0][2];    pM->data[2][1] = src->data[1][2];    pM->data[2][2] = src->data[2][2];    pM->data[2][3] = src->data[3][2];
    pM->data[3][0] = src->data[0][3];    pM->data[3][1] = src->data[1][3];    pM->data[3][2] = src->data[2][3];    pM->data[3][3] = src->data[3][3];
    
    // copy back if needed
    if( pM == &mTmp )
    {
        MTX44Copy( pM, xPos );
    }
}

void MTX44Trans(STMTX44 *mOut,f32 xT,f32 yT,f32 zT){
    mOut->data[0][0] = 1.0f;     mOut->data[0][1] = 0.0f;  mOut->data[0][2] = 0.0f;  mOut->data[0][3] =  xT;
    mOut->data[1][0] = 0.0f;     mOut->data[1][1] = 1.0f;  mOut->data[1][2] = 0.0f;  mOut->data[1][3] =  yT;
    mOut->data[2][0] = 0.0f;     mOut->data[2][1] = 0.0f;  mOut->data[2][2] = 1.0f;  mOut->data[2][3] =  zT;
    mOut->data[3][0] = 0.0f;     mOut->data[3][1] = 0.0f;  mOut->data[3][2] = 0.0f;  mOut->data[3][3] =  1.0f;
}
void MTX44TransApply(const STMTX44 *src,STMTX44 *des,f32 xT,f32 yT,f32 zT){
    
    if ( src != des )
    {
        des->data[0][0] = src->data[0][0];    des->data[0][1] = src->data[0][1];    des->data[0][2] = src->data[0][2];
        des->data[1][0] = src->data[1][0];    des->data[1][1] = src->data[1][1];    des->data[1][2] = src->data[1][2];
        des->data[2][0] = src->data[2][0];    des->data[2][1] = src->data[2][1];    des->data[2][2] = src->data[2][2];
        des->data[3][0] = src->data[3][0];    des->data[3][1] = src->data[3][1];    des->data[3][2] = src->data[3][2];
        des->data[3][3] = src->data[3][3];
    }
    
    des->data[0][3] = src->data[0][3] + xT;
    des->data[1][3] = src->data[1][3] + yT;
    des->data[2][3] = src->data[2][3] + zT;
}

void MTX44Scale(STMTX44 *mOut,f32 xS,f32 yS,f32 zS){
    mOut->data[0][0] = xS;      mOut->data[0][1] = 0.0f;  mOut->data[0][2] = 0.0f;  mOut->data[0][3] = 0.0f;
    mOut->data[1][0] = 0.0f;    mOut->data[1][1] = yS;    mOut->data[1][2] = 0.0f;  mOut->data[1][3] = 0.0f;
    mOut->data[2][0] = 0.0f;    mOut->data[2][1] = 0.0f;  mOut->data[2][2] = zS;    mOut->data[2][3] = 0.0f;
    mOut->data[3][0] = 0.0f;    mOut->data[3][1] = 0.0f;  mOut->data[3][2] = 0.0f;  mOut->data[3][3] = 1.0f;
}

void MTX44ScaleApply(const STMTX44 *src,STMTX44 *des,f32 xS,f32 yS,f32 zS){
    des->data[0][0] = src->data[0][0] * xS;     des->data[0][1] = src->data[0][1] * xS;
    des->data[0][2] = src->data[0][2] * xS;     des->data[0][3] = src->data[0][3] * xS;
    
    des->data[1][0] = src->data[1][0] * yS;     des->data[1][1] = src->data[1][1] * yS;
    des->data[1][2] = src->data[1][2] * yS;     des->data[1][3] = src->data[1][3] * yS;
    
    des->data[2][0] = src->data[2][0] * zS;     des->data[2][1] = src->data[2][1] * zS;
    des->data[2][2] = src->data[2][2] * zS;     des->data[2][3] = src->data[2][3] * zS;
    
    des->data[3][0] = src->data[3][0] ; des->data[3][1] = src->data[3][1];
    des->data[3][2] = src->data[3][2] ; des->data[3][3] = src->data[3][3];
}

void MTX44Copy(const STMTX44 *src,STMTX44 *des){
    
    des->data[0][0] = src->data[0][0];  des->data[0][1] = src->data[0][1]; des->data[0][2] = src->data[0][2];  des->data[0][3] = src->data[0][3];
    des->data[1][0] = src->data[1][0];  des->data[1][1] = src->data[1][1]; des->data[1][2] = src->data[1][2];  des->data[1][3] = src->data[1][3];
    des->data[2][0] = src->data[2][0];  des->data[2][1] = src->data[2][1]; des->data[2][2] = src->data[2][2];  des->data[2][3] = src->data[2][3];
    des->data[3][0] = src->data[3][0];  des->data[3][1] = src->data[3][1]; des->data[3][2] = src->data[3][2];  des->data[3][3] = src->data[3][3];
    
}

void MTX44FillData(STMTX44 *mtxOut,f32* glData){
    mtxOut->data[0][0] = glData[0]; mtxOut->data[0][1] = glData[4]; mtxOut->data[0][2] = glData[8];   mtxOut->data[0][3] = glData[12];
    mtxOut->data[1][0] = glData[1]; mtxOut->data[1][1] = glData[5]; mtxOut->data[1][2] = glData[9];   mtxOut->data[1][3] = glData[13];
    mtxOut->data[2][0] = glData[2]; mtxOut->data[2][1] = glData[6]; mtxOut->data[2][2] = glData[10];  mtxOut->data[2][3] = glData[14];
    mtxOut->data[3][0] = glData[3]; mtxOut->data[3][1] = glData[7]; mtxOut->data[3][2] = glData[11];  mtxOut->data[3][3] = glData[15];
    
}

void MTX44Identity(STMTX44 *mtx){
    mtx->data[0][0] = 1.0f;  mtx->data[0][1] = 0.0f;  mtx->data[0][2] = 0.0f;  mtx->data[0][3] = 0.0f;
    mtx->data[1][0] = 0.0f;  mtx->data[1][1] = 1.0f;  mtx->data[1][2] = 0.0f;  mtx->data[1][3] = 0.0f;
    mtx->data[2][0] = 0.0f;  mtx->data[2][1] = 0.0f;  mtx->data[2][2] = 1.0f;  mtx->data[2][3] = 0.0f;
    mtx->data[3][0] = 0.0f;  mtx->data[3][1] = 0.0f;  mtx->data[3][2] = 0.0f;  mtx->data[3][3] = 0.0f;
    
}

#define SWAPF(a,b) { f32 tmp; tmp = (a); (a) = (b); (b) = tmp;}

int MTX44Inverse(const STMTX44 *mtxIn,STMTX44 *mtxOut){
    STMTX44 gjm;
    STMTX44 inv;
    s32   i,j,k;
    f32   w;
    
    MTX44Copy(mtxIn, &gjm);
    MTX44Identity(&inv);
    
    // use Gauss-Jordan(with partial pivoting)
    for(i=0; i < 4 ; i++)
    {
        f32 max = 0.0f;
        s32 swp = i;
        
        // ---- partial pivoting -----
        for( k = i ; k < 4 ; k++ )
        {
            f32 ftmp;
            ftmp = fabsf(gjm.data[k][i]);
            if ( ftmp > max )
            {
                max = ftmp;
                swp = k;
            }
        }
        
        // check singular matrix
        //(or can't solve inverse matrix with this algorithm)
        if ( max == 0.0f )
        {
            return 0;
        }
        
        // swap row
        if( swp != i )
        {
            for ( k = 0 ; k < 4 ; k++ )
            {
                SWAPF(gjm.data[i][k], gjm.data[swp][k]);
                SWAPF(inv.data[i][k], inv.data[swp][k]);
            }
        }
        
        // ---- pivoting end ----
        
        w = 1.0F / gjm.data[i][i];
        for ( j = 0 ; j < 4 ; ++j )
        {
            gjm.data[i][j] *= w;
            inv.data[i][j] *= w;
        }
        
        for ( k = 0 ; k < 4 ; ++k )
        {
            if ( k == i )
                continue;
            
            w = gjm.data[k][i];
            for ( j = 0 ; j < 4 ; ++j )
            {
                gjm.data[k][j] -= gjm.data[i][j] * w;
                inv.data[k][j] -= inv.data[i][j] * w;
            }
        }
    }
    MTX44Copy(&inv, mtxOut);
    return 1;
}

#pragma mark -
#pragma mark VECTOR
#pragma mark -

STVEC3* VEC3Transform(STVEC3* pOut,const STMTX34* pM,const STVEC3 *pV){
    STVEC3 tmp;
    
    tmp.x = pM->data[0][0] * pV->x + pM->data[0][1]*pV->y + pM->data[0][2]*pV->z + pM->data[0][3];
    tmp.y = pM->data[1][0] * pV->x + pM->data[1][1]*pV->y + pM->data[1][2]*pV->z + pM->data[1][3];
    tmp.z = pM->data[2][0] * pV->x + pM->data[2][1]*pV->y + pM->data[2][2]*pV->z + pM->data[2][3];
    
    pOut->x = tmp.x;
    pOut->y = tmp.y;
    pOut->z = tmp.z;
    
    return pOut;
}

STVEC3* VEC3Normalize(STVEC3* p1,STVEC3* pOut)
{
    
    f32 l = VEC3Len(p1);
    if(l>0)
    {
        pOut->x = p1->x/l;
        pOut->y = p1->y/l;
        pOut->z = p1->z/l;
    }
    return pOut;
}

STVEC3* VEC3Invert(STVEC3* p1,STVEC3* pOut){
    pOut->x = -p1->x;
    pOut->y = -p1->y;
    pOut->z = -p1->z;
    return pOut;
}

STVEC3* VEC3Add(STVEC3* p1,STVEC3* p2,STVEC3* pOut){
    pOut->x = p1->x + p2->x;
    pOut->y = p1->y + p2->y;
    pOut->z = p1->z + p2->z;
    return pOut;
}

STVEC3* VEC3Sub(STVEC3* p1,STVEC3* p2,STVEC3* pOut){
    pOut->x = p1->x - p2->x;
    pOut->y = p1->y - p2->y;
    pOut->z = p1->z - p2->z;
    return pOut;
}

STVEC3* VEC3Multi(STVEC3* p1,STVEC3* p2,STVEC3* pOut){
    pOut->x = p1->x * p2->x;
    pOut->y = p1->y * p2->y;
    pOut->z = p1->z * p2->z;
    return pOut;
}

STVEC3* VEC3Scale(STVEC3* p,f32 scale,STVEC3* pOut){
    pOut->x = scale * p->x;
    pOut->y = scale * p->y;
    pOut->z = scale * p->z;
    return pOut;
}

STVEC3* VEC3Lerp(STVEC3* p1,STVEC3* p2,f32 t,STVEC3* pOut){
    pOut->x = p1->x + t * (p2->x - p1->x);
    pOut->y = p1->y + t * (p2->y - p1->y);
    pOut->z = p1->z + t * (p2->z - p1->z);
    return pOut;
}

STVEC3* VEC3AddScale(STVEC3* p1,STVEC3* pAdd,f32 scale,STVEC3* pOut){
    pOut->x = p1->x + pAdd->x * scale;
    pOut->y = p1->y + pAdd->y * scale;
    pOut->z = p1->z + pAdd->z * scale;
    return pOut;
}

STVEC3* VEC3Cross(STVEC3* p1,STVEC3 *p2,STVEC3* pOut){
    pOut->x = p1->y*p2->z - p1->z*p2->y;
    pOut->y = p1->z*p2->x - p1->x*p2->z;
    pOut->z = p1->x*p2->y - p1->y*p2->x;
    return pOut;
}

f32 VEC3Dot(const STVEC3* p1, const STVEC3* p2)
{
    return p1->x * p2->x + p1->y * p2->y + p1->z * p2->z;
}

f32 VEC3SquareLen(const STVEC3* p)
{
    return p->x * p->x + p->y * p->y + p->z * p->z;
}

f32 VEC3Len(const STVEC3* p)
{
    //assert(p);
    return sqrtf( VEC3SquareLen( p ) );
}
f32     VEC3SquareDist( STVEC3* p1, STVEC3* p2){
    STVEC3 temp;
    
    return VEC3SquareLen(VEC3Sub(p2,p1,&temp));
    
}

f32 VEC3Dist(  STVEC3* p1,  STVEC3* p2 )
{
    //assert( p1 );
    //assert( p2 );
    
    return sqrtf( VEC3SquareDist( p1, p2 ) );
}