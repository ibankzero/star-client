//
//  StarClient.cpp
//  StarMacOS
//
//  Created by SAMRET WAJANASATHIAN on 6/11/2556.
//
//
// Protocal version 2.0 22/2/2557
// ปิงทุกวิ
// ถ้ามีความถี่จะปรับได้เป็น 10 paket/sec
// จัดการ การส่ง อัตนโนมัตร
// ส่ง file และ resume โดยอัตโนมัติ
//  1 Frame send 10 Packet
//  1 Packet max 64 K
//  1 Frame can send 640KByte Max
//

#include "StarClient.h"
#include "StarSystem.h"
#include "StarMEM.h"
#include "StarMath.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#ifdef STAR_WINDOWS_VERSION

#else
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif


//typedef void (*FuncPtr)( const char * );
//FuncPtr DebugLog;
//
//extern "C"
//{
//    void SetDebugFunction( FuncPtr fp )
//    {
//        DebugLog = fp;
//    }
//}

/*---------------------------------------------------------------------------*
 Error String definitions
 *---------------------------------------------------------------------------*/
char SC_STATUS_STRING[SC_STATUS_MAX][16] = {
    "READY",
    "ERROR",
    "BUSY",
    "PARENT",
    "CHILD",
    "SCAN",
    "CONNECTION",
    "CONNECTED",
    "NONE",
};

char SC_ERROR_STRING[18][128] = {
    "ERROR 101 \nติด Proxy Server",
    "ERROR 102 \nเชื่อมต่อ Server ไม่ได้",//ต่อ Server ไม่ได้",
    "ERROR 103 \nเล่นในประเทศนี้ไม่ได้ ",
    "ERROR 104 \nเวอร์ชั่นผิด โหลดตัวใหม่นะ",
    "ERROR 105 \nID นี้โดนแบน",
    "ERROR 106 \nไม่มี ID นี้ในระบบ",
    "ERROR 107 \nPassword ไม่ถูกต้อง",
    "ERROR 108 \nServer ปิดปรับปรุง",
    "ERROR 109 \nServer ตัดคุณออกจากระบบ",
    "ERROR 110 \nlogin ซ้อน",
    "ERROR 111 \nขาดการเชื่อมต่อนานเกินไป",
    "ERROR 112 \nเชื่อมต่อเกมServerไม่ได้",//อินเตอร์เนตห่วยมาก",
    "ERROR 113 \nไม่ได้ต่ออินเตอร์เนต",
    "ERROR 301 \nServer Full",
    "ERROR 114 \nWating Data",
    "ERROR 115 \nEngine เล่นไม่ได้กับ Server",
    "ERROR 116 \nมีการผิดผลาดในการส่งข้อมูล",
    "ERROR 117 \nเกมเวอร์ชั้นนี้ไม่สามารถเล่นได้"
};

typedef struct {
    s16   type;
    s16   fromID;
    s32   packNo;
    //s32   toID; // destination host ID
    //s32   fromID; // source hots ID
}STSOCKMSG_GENERIC; // 8 byte

// Use Only UDP
typedef struct {
    s32 conf;
}STSOCKMSG_CONF;

//0897940100 Data size for big data size
typedef struct {
    s16 type;
    s16 fromID;
    s32 packNo;
    s32 size;   // if size negative it mean continue data
    // if size Positive it mean init size;
}STSOCKMSGDATA_DATASIZE;

// for packet send data size
typedef struct {
    s16 type;
    s16 fromID;
    s32 packNo;
    s32 sizeAll; // All size   // Maxsize 64K //
    
    s16 crc;
    s16 numData; // HowManyData
}STSOCKMSGDATA_PACKDATASIZE; // 16 byte


/*---------------------------------------------------------------------------*
 Variable definitions
 *---------------------------------------------------------------------------*/

#ifndef STAR_WINDOWS_VERSION
typedef int   SOCKET;
#else
#define MSG_DONTWAIT 0
typedef int socklen_t;
#endif
#define INVALID_SOCKET  -1

typedef struct {
    SOCKET        socket;
    sockaddr_in   inetSockAddr;
    socklen_t     addrLen;
    bool          online;
    bool          thread;
    bool          canSendData;
    bool          processRecieve;
    u32           packRef;
    
    int           sendCount; // Number Packet ที่ส่งต่อ 1 frame
    int           recvCount; // Number Packet
    int           currentRecvIdx;
    int           maxRecvBuffer;
    
    int           curPacketNo; // Packet ปัจจะบันเบอร์ที่เท่าไหร่
    u8            sendPacketNo[STAR_CLIENT_DEFAULT_MAX_BLOCK];
    int           recvDestroyCount; // will auto delete if not process in 3 frame
    int           sendDataSize;
    bool          bGotConfirm;
    u32           streamSize;
    u32           tickCount;
    u32           waitingCount;
    
    u32           serverIP;
    u16           serverPort;
    bool          CreateConnectThread;
#ifdef STAR_WINDOWS_VERSION
    HANDLE		  thread_ID;
    DWORD		  thread_CreateID;
#else
    pthread_t     thread_ID;
#endif
    char* sendBuffer;
    char* recvBuffer; // รับมาจาก Server
    char* tempRecvBuffer[10];  // รับได้มากสุด 10 อัน 640K
    char* streamDataBuffer;
    // ใช้ตัวส่ง 2 ตัวสลับกันในกรณีที่ข้อมูลส่งกลับมาช้า หรือ อาจจะต้องมีการขอซ้ำ
    char* sendBufferMSG[STAR_CLIENT_DEFAULT_MAX_BLOCK];
    char* recvBufferMSG[STAR_CLIENT_DEFAULT_MAX_BLOCK];
    int   sendBufferMSGSize[STAR_CLIENT_DEFAULT_MAX_BLOCK];
    int   recvBufferMSGSize[STAR_CLIENT_DEFAULT_MAX_BLOCK];
    
    u8*        bigDataBuffer;
    u32        bigDataBufferSize;
    u32        bigDataSize;      // ตัวโปรแกรมจะจองหน่วยความจำและจะไม่มีการ Free หน่วยความจำ
    u32        bigDataSendSize;
    bool       bigDataSending;   // กำลังส่งข้อมูลไปที่ server
    
    //  File Send ใช้ตัวแปรเดียวกับ big data
    char       sendFileName[32];
    bool       fileDataSending;
    
    u8*        bigDataRecvBuffer;
    u32        bigDataRecvBufferSize;
    u32        bigDataRecvSize;
    u32        bigDataRecvSizeDone;
    char       recvFileName[32];
    
    bool       bigDataReady;
}RemoteHost_t;

extern StarVersion  version;
extern char * STNET_NTOIP(u32 ipValue);
// Global Variable;
static u32        s_nTimeOut; // will convert to micro second
static u32        s_nReTrasmit;
static u32        s_nFrameTime;
static u32        s_nConnectType;
static u32        s_nResponTime;
static volatile SCStatus   s_scStatus = SC_STATUS_NONE;
static volatile u32        s_nError;
u_long            s_bBlocking;
static RemoteHost_t    s_RemoteHost; // Server Data
static s32        s_nLocalID;

static u32        s_nDataSendByte;
static u32        s_nDataRecvByte;
static u32        s_nTimeStart;    // Milli Sec
static u32        s_nConnectionType;

static bool       s_bSendHeader; // swith between send header and Data
static s32        s_nSendSize;   // ขนาดที่ส่งอยู่
static s32        s_nSendingSize; // ขนาดที่ส่งเสร็จแล้ว
static StarVersion  s_ClientVersion;
#ifdef STAR_WINDOWS_VERSION
HANDLE  s_MutexBusy;
//    HANDLE  s_MutexBusyRead;
//    HANDLE  s_MutexWaitGetData;
#else
static pthread_mutex_t   s_MutexBusy = PTHREAD_MUTEX_INITIALIZER;
//    static pthread_mutex_t   s_MutexBusyRead = PTHREAD_MUTEX_INITIALIZER;
//    static pthread_mutex_t   s_MutexWaitGetData = PTHREAD_MUTEX_INITIALIZER;
#endif

#ifdef STAR_WINDOWS_VERSION
DWORD WINAPI STSOCK_IOThreadFunc(LPVOID paramPtr);
DWORD WINAPI STSOCK_ConnectThreadFunc(LPVOID ptr);
#else
static void *STSOCK_IOThreadFunc(void *paramPtr);
static void *STSOCK_ConnectThreadFunc(void *ptr);
#endif
int Read(char *buf,int count);

static int ReadFrom(SOCKET sock,char* buf,int count,struct sockaddr * sockAddr,socklen_t* sockLen);
static int WriteTo(SOCKET sock,char* buf,int count,struct sockaddr *sockAddr,socklen_t sockLen);

static void SendReTrasmit();
static void SendPingPacket();


/*---------------------------------------------------------------------------*
 Internal Function Implementation
 *---------------------------------------------------------------------------*/


static int processMessageBuffer(RemoteHost_t *host);

static void processCheckSendData(RemoteHost_t *host);

static void SendPingPacket() {
    size_t  byte_send;
    STSOCKMSGDATA_DATASIZE dataSize;
    dataSize.type = STSOCKMSG_PING;
    
    byte_send = WriteTo(s_RemoteHost.socket, (char*)&dataSize, sizeof(STSOCKMSGDATA_DATASIZE), (struct sockaddr *)&s_RemoteHost.inetSockAddr, s_RemoteHost.addrLen);
}

static void SendReTrasmit() {
    if(s_nConnectType == STAR_CLIENT_TCP_CONNECT) {
        return;  // TCP IP not retrasmit
    }
    
    size_t  byte_send;
    byte_send = WriteTo(s_RemoteHost.socket, s_RemoteHost.sendBuffer, s_RemoteHost.sendDataSize, (struct sockaddr *)&s_RemoteHost.inetSockAddr, s_RemoteHost.addrLen);
}

static int ReadFrom(SOCKET sock, char* buf, int count, struct sockaddr * sockAddr, socklen_t* sockLen){
    u32 startTime = STOS_GetTickCount();
    
    size_t byte_read = 0;
    int this_read = -1;
    
    if (s_bBlocking == 1) {
        while (byte_read < count) {
            while (this_read < 0) {
                this_read = (int)recvfrom(sock, buf, count - byte_read, 0, sockAddr, sockLen);
            }
            byte_read += this_read;
            buf       += this_read;
            this_read = -1; // Bug Fix
        }
    }
    else {
        while (byte_read < count) {   //STOS_GetTickCount() - startTime < s_nReTrasmit) {
            if(s_nConnectType == STAR_CLIENT_TCP_CONNECT) {
                this_read = (int)recv(sock, buf, count - byte_read, 0);
            }
            else {
                this_read = (int)recvfrom(sock, buf, count - byte_read, MSG_DONTWAIT, sockAddr, sockLen);
            }
            
            if(this_read > 0) {
                byte_read += this_read;
                buf       += this_read;
                s_nDataRecvByte += this_read;
            }
            if (byte_read == count) {
                return count;
            }
            
            if (this_read == 0) {
                return 0;  // disconnect
            }
            
            // ถ้าหมดเวลาแล้วยังรับไม่หมด และยังมีข้อมูลวิ่งมาอยู่ต้องทำการต่อเวลาไปที่ server เพื่อให้รู้ว่ายัง ออนไลย์อยู่ ไม่งั้นจะโดนตัดการเชื่อมต่อ
            if(STOS_GetTickCount() - startTime > s_nFrameTime) { // Fix from < to > 2.59.2
                SendPingPacket();
                startTime = STOS_GetTickCount(); // calculate bandwish here
            }
        }
        
        if(byte_read != count) {
            // FIX THIS BUG for byte_read = -1 or 0 or not same data
            return -1;
        }
    }
    return count;
}

static int WriteTo(SOCKET sock, char* buf, int count, struct sockaddr *sockAddr, socklen_t sockLen){
    int byte_send =  0;
    int this_write = -1;
    
    if(s_nConnectType == STAR_CLIENT_TCP_CONNECT) {
        while (byte_send < count) {
            this_write = (int)send(sock, buf, count-byte_send, 0);
            if(this_write == 0) {
                s_nError    = STAR_CLIENT_ERROR_CONNECT_FAIL;
                s_scStatus  = SC_STATUS_ERROR;
                return this_write;
            }
            else if(this_write > 0) {
                byte_send += this_write;
                buf       += this_write;
                s_nDataSendByte += this_write;
            }
        }
    }
    else if(s_nConnectType == STAR_CLIENT_UDP_CONNECT) {
        while (byte_send < count) {
            // send data
            this_write = (int)sendto(sock, buf, count - byte_send, 0, sockAddr, sockLen);
            if(this_write < 0) {
                return this_write;
            }
            byte_send += this_write;
            buf       += this_write;
            s_nDataSendByte += this_write;
        }
    }
    return byte_send;
}

/*---------------------------------------------------------------------------*
 Star socket Protocal Function Implementation
 *---------------------------------------------------------------------------*/
// packet ใหญ่เท่าไหร่ก็ได้แต่ไม่เกิน 60K
bool STSC_SendData(const void *buffer, u32 size) { // limite max STAR_CLIENT_DEFAULT_BUFFER
    if(s_bSendHeader == true) // ยังส่งไม่เสร็จ
        return false;
    
    if(s_scStatus == SC_STATUS_NONE || s_scStatus == SC_STATUS_ERROR || s_scStatus == SC_STATUS_READY)
        return false;
    
    int bufferSize = ((STAR_CLIENT_DEFAULT_MAX_BLOCK-2) - s_RemoteHost.sendCount) * STAR_CLIENT_DEFAULT_BUFFER;
    char* bufferSend = (char*)buffer;
    
    if(size > bufferSize) {
        return false;
    }
    
    if(s_RemoteHost.sendCount < STAR_CLIENT_DEFAULT_MAX_BLOCK - 2) {
        u32 fillSize = size;
        while (fillSize) {
            if(fillSize <= STAR_CLIENT_DEFAULT_BUFFER )
            {
                memcpy(s_RemoteHost.sendBufferMSG[s_RemoteHost.sendCount],bufferSend,fillSize);
                s_RemoteHost.sendBufferMSGSize[s_RemoteHost.sendCount] = fillSize;
                s_RemoteHost.sendPacketNo[s_RemoteHost.sendCount] = s_RemoteHost.curPacketNo;
                s_RemoteHost.sendCount++;
                
                fillSize -= fillSize;
                bufferSend += fillSize;
            }
            else {
                memcpy(s_RemoteHost.sendBufferMSG[s_RemoteHost.sendCount],bufferSend,STAR_CLIENT_DEFAULT_BUFFER);
                s_RemoteHost.sendBufferMSGSize[s_RemoteHost.sendCount] = STAR_CLIENT_DEFAULT_BUFFER;
                s_RemoteHost.sendPacketNo[s_RemoteHost.sendCount] = s_RemoteHost.curPacketNo;
                s_RemoteHost.sendCount++;
                
                fillSize -= STAR_CLIENT_DEFAULT_BUFFER;
                bufferSend += STAR_CLIENT_DEFAULT_BUFFER;
            }
        }
        s_RemoteHost.curPacketNo++;
    }
    else {
        return false;
    }
    return true;
}

bool STSC_CanSendFileOrData() {
    return (s_RemoteHost.canSendData == true && s_RemoteHost.bigDataSending == false && s_RemoteHost.fileDataSending == false);
}

void STSC_SendFileBuffer(const char *fileName, const void *buffer, u32 size) {
    if(s_scStatus == SC_STATUS_NONE) {
        return;
    }
    
    if(s_RemoteHost.bigDataSending == true || s_RemoteHost.fileDataSending == true)
        return;
    
    if (s_RemoteHost.bigDataBuffer == NULL) {
        s_RemoteHost.bigDataBuffer = (u8*)STOS_Alloc(size);
        s_RemoteHost.bigDataSize  = size;
        s_RemoteHost.bigDataBufferSize = size;
    }
    
    if(s_RemoteHost.bigDataBufferSize < size) {
        if (s_RemoteHost.bigDataBuffer) {
            STOS_Free(s_RemoteHost.bigDataBuffer);
            s_RemoteHost.bigDataBuffer = (u8*)STOS_Alloc(size);
            s_RemoteHost.bigDataSize  = size;
            s_RemoteHost.bigDataBufferSize = size;
        }
    }
    
    if(s_RemoteHost.bigDataBuffer == NULL)
    {
        return;
    }
    
    // copy data to big data send buffer
    memcpy(s_RemoteHost.bigDataBuffer,buffer,size);
    s_RemoteHost.fileDataSending  = true;
    s_RemoteHost.bigDataSendSize = 0;
    sprintf(s_RemoteHost.sendFileName,"%s",fileName);
    
}// ส่ง buffer และ บันทึกเป็นชื่อไฟล์ ห้ามเกิน 32 ตัวอักษร

void STSC_SendBigData(const void *buffer, u32 size) {
    
    if(s_RemoteHost.bigDataSending == true || s_RemoteHost.fileDataSending == true)
        return;
    
    if (s_RemoteHost.bigDataBuffer == NULL) {
        s_RemoteHost.bigDataBuffer = (u8*)STOS_Alloc(size);
        s_RemoteHost.bigDataSize  = size;
        s_RemoteHost.bigDataBufferSize = size;
    }
    
    if(s_RemoteHost.bigDataBufferSize < size) {
        // re-allocate data
        if (s_RemoteHost.bigDataBuffer) {
            STOS_Free(s_RemoteHost.bigDataBuffer);
            s_RemoteHost.bigDataBuffer = (u8*)STOS_Alloc(size);
            s_RemoteHost.bigDataSize  = size;
            s_RemoteHost.bigDataBufferSize = size;
        }
    }
    
    if(s_RemoteHost.bigDataBuffer == NULL) {
        return;
    }
    
    // copy data to big data send buffer
    memcpy(s_RemoteHost.bigDataBuffer,buffer,size);
    s_RemoteHost.bigDataSending  = true;
    s_RemoteHost.bigDataSendSize = 0;
}

bool STSC_HaveBigDataSending() {
    return (s_RemoteHost.bigDataSending == true || s_RemoteHost.fileDataSending == true);
}

int  STSC_BigDataSendingSize() {
    return s_RemoteHost.bigDataSendSize;
}

int  STSC_BigDataSendSize() {
    return s_RemoteHost.bigDataSize;
}

bool STSC_HaveBigDataRecving() {
    return (s_RemoteHost.bigDataReady == false && s_RemoteHost.bigDataRecvSizeDone < s_RemoteHost.bigDataRecvSize);
}

int  STSC_BigDataRecvingSize() {
    return s_RemoteHost.bigDataRecvSizeDone;
}

bool STSC_HaveBigData() {
    return s_RemoteHost.bigDataReady;
}

u8*  STSC_GetBigDataBuffer() {
    return s_RemoteHost.bigDataRecvBuffer;
}

int  STSC_GetBigDataSize() {
    return s_RemoteHost.bigDataRecvSize;
}

char*STSC_GetBigDataFileName() { // NULL if only data
    return s_RemoteHost.recvFileName;
}

void STSC_GetBigDataDone() {
    s_RemoteHost.bigDataReady =false;
}

bool STSC_HaveData() { // chekc this first before call RecieveData
    if(s_RemoteHost.currentRecvIdx != s_RemoteHost.maxRecvBuffer
       && s_RemoteHost.processRecieve == false )
        return true;
    else
        return false;
}

void STSC_GetData(int idx,void *buffer,u32 *dataSize) {
    if(s_RemoteHost.processRecieve == true) {
        return;
    }
    
    memcpy(buffer, s_RemoteHost.recvBufferMSG[idx], s_RemoteHost.recvBufferMSGSize[idx]);
    *dataSize = s_RemoteHost.recvBufferMSGSize[idx];
}

int  STSC_GetDataCount() {
    return s_RemoteHost.recvCount;
}

int  STSC_GetDataType(int idx) {
    return -1;
}

u32 STSC_GetDataSize(int idx) {
    return s_RemoteHost.recvBufferMSGSize[idx];
}

SCLinkLevel STSC_GetLinkLevel() {
    
    int signelLevel = s_nFrameTime / 4;
    
    if(s_nResponTime <= signelLevel) {
        return SC_LINK_MAX;
    }
    else if(s_nResponTime <= (signelLevel << 1))
    {
        return SC_LINK_BEST;
    }
    else if(s_nResponTime <= (signelLevel << 1) + signelLevel)
    {
        return SC_LINK_GOOD;
    }
    else if(s_nResponTime <= (signelLevel << 2))
    {
        return SC_LINK_NORMAL;
    }
    else if(s_nResponTime < (s_nFrameTime << 1))
    {
        return SC_LINK_POOL;
    }
    else
        return SC_LINK_BAD;
}

static int _processMessagePacket(RemoteHost_t *client) {
    
    STSOCKMSGDATA_PACKDATASIZE  *PackData;
    u16                         *dataSizeTable;
    char                        *startDataAddr;
    s16                         curPacket = 0;
    u16                         continuePack = 0;
    
    
    PackData = (STSOCKMSGDATA_PACKDATASIZE *)client->tempRecvBuffer[client->currentRecvIdx];
    
    if (PackData->numData == 0) {
        client->currentRecvIdx++;
        if(client->currentRecvIdx >= 10)
            client->currentRecvIdx = 0;
        return 0;
    }
    // Now Data Correct let fill in MSG Buffer
    
    dataSizeTable = (u16*)&client->tempRecvBuffer[client->currentRecvIdx][sizeof(STSOCKMSGDATA_PACKDATASIZE)];
    startDataAddr = (char*)&client->tempRecvBuffer[client->currentRecvIdx][sizeof(STSOCKMSGDATA_PACKDATASIZE) + (2 * PackData->numData)];
    
    continuePack = 1;
    curPacket  = -1;
    
    for(int i=0; i<PackData -> numData; i++) {
        if(continuePack != dataSizeTable[i] >> 11) {
            continuePack = 1-continuePack;
            curPacket++;
            client->recvBufferMSG[curPacket] = startDataAddr;
            client->recvBufferMSGSize[curPacket] = 0;
        }
        
        client->recvBufferMSGSize[curPacket] += dataSizeTable[i] & 0x7ff;
        continuePack = dataSizeTable[i] >> 11;
        startDataAddr += (dataSizeTable[i] & 0x7ff);
    }
    
    client->recvCount = curPacket + 1; //PackData->numData;
    client->currentRecvIdx++;
    if(client->currentRecvIdx >= 10)
        client->currentRecvIdx = 0;
    return 0;
}

void STSC_StartGetData() {
    _processMessagePacket(&s_RemoteHost);
}

void STSC_EndGetData() {
    s_RemoteHost.recvCount = 0;
}

static int _ProessMessageFileData(RemoteHost_t *client) {
    
    STSOCKMSGDATA_PACKDATASIZE  *PackData;
    u16                         packetDataSize = 0;
    size_t                      bigDataPackSize;
    u16                         *dataSizeTable;
    char                        *startDataAddr;
    
    s32                         *headerBigData;
    
    PackData = (STSOCKMSGDATA_PACKDATASIZE*)client->recvBuffer;
    dataSizeTable = (u16 *)&client->recvBuffer[sizeof(STSOCKMSGDATA_PACKDATASIZE)];
    for(int i=0;i<PackData->numData;i++) {
        packetDataSize += dataSizeTable[i] & 0x7ff;
    }
    packetDataSize += sizeof(STSOCKMSGDATA_PACKDATASIZE) + (2*PackData->numData);
    startDataAddr = (char*)&client->recvBuffer[packetDataSize];
    headerBigData = (s32*)startDataAddr;
    startDataAddr += 8;
    bigDataPackSize = (PackData->sizeAll -  packetDataSize) + sizeof(STSOCKMSGDATA_DATASIZE) - 8; // delete header 8 byte
    
    if(headerBigData[0] & STSOCKMSG_BIGDATA_HEADER) {
        // write to file first for temp and get back when want to process for save memory
        client->bigDataReady = false;
        client->bigDataRecvSize  = headerBigData[1];
        client->bigDataRecvSizeDone = 0;
        
        memcpy(client->recvFileName, (char*)&headerBigData[2], 32);
        
        if (client->bigDataRecvBuffer == NULL) {
            client->bigDataRecvBuffer =(u8*) STOS_Alloc(client->bigDataRecvSize);
            client->bigDataRecvBufferSize = client->bigDataRecvSize;
        }
        else if(client->bigDataRecvBufferSize < client->bigDataRecvSize) {
            STOS_Free(client->bigDataRecvBuffer);
            client->bigDataRecvBuffer =(u8*) STOS_Alloc(client->bigDataRecvSize);
            client->bigDataRecvBufferSize = client->bigDataRecvSize;
        }
        bigDataPackSize -=32;
        startDataAddr += 32;
    }
    else if( (headerBigData[0] & STSOCKMSG_BIGDATA_CONTINUE) || ( headerBigData[0] & STSOCKMSG_BIGDATA_END)) {
        
    }
    
    memcpy(&client->bigDataRecvBuffer[client->bigDataRecvSizeDone], startDataAddr, bigDataPackSize);
    
    client->bigDataRecvSizeDone += bigDataPackSize;
    if(headerBigData[0] & STSOCKMSG_BIGDATA_END) {
        client->bigDataReady = true;
    }
    
    return 0;
}

static void _PackDataWithPacket() {
    
    int i;
    u32 sumSize = 0;
    u16 dataCount = s_RemoteHost.sendCount;
    
    // Start Packing Data
    STSOCKMSGDATA_PACKDATASIZE    *packetHeader;
    u16                           *dataSizeTable;
    char                          *startDataAddr;
    u16                           continuePack = 0;
    u8                            curPacket = 0;
    packetHeader            = (STSOCKMSGDATA_PACKDATASIZE  *)s_RemoteHost.sendBuffer;  // 16 byte
    dataSizeTable = (u16*)&s_RemoteHost.sendBuffer[sizeof(STSOCKMSGDATA_PACKDATASIZE)]; //
    startDataAddr = (char*)&s_RemoteHost.sendBuffer[sizeof(STSOCKMSGDATA_PACKDATASIZE) + (2*dataCount)];
    
    packetHeader->type      = STSOCKMSG_PACKDATASIZE;
    packetHeader->packNo    = s_RemoteHost.packRef;
    packetHeader->fromID    = s_nLocalID;
    
    for (i=0; i<dataCount; i++) {
        sumSize += s_RemoteHost.sendBufferMSGSize[i];
    }
    
    sumSize += (dataCount * 2); // Add Data Size Table
    sumSize += 4; // Add 4 byte extend
    
    packetHeader->sizeAll  = sumSize;
    packetHeader->numData  = dataCount;
    
    //Start Packing Data // will have 2 byter for eachdata
    for(i=0; i<dataCount; i++) {
        // นับจากด้านขวามาซ้าย
        //  11 bit แรก เป็นขนาดของ Packet มากสุด 1024
        //  11  bit ถัดมาเป็น การบอกการต่อเนื่องของ Packet เนื่องด้วยมีแค่ 6 bit และ สามารถให้ได้มากสุดแค่ 32 เลยใช่ 1 กับ 0 สลับกันไปเริ่มที่ 0 และสลับไปเป็น  1
        if(curPacket != s_RemoteHost.sendPacketNo[i]) {
            continuePack = 1 - continuePack; // สลับ 0 กับ 1
        }
        dataSizeTable[i]  = s_RemoteHost.sendBufferMSGSize[i] | (continuePack << 11);
        curPacket =  s_RemoteHost.sendPacketNo[i];
    }
    
    for(i=0; i<dataCount; i++) {
        memcpy(startDataAddr,s_RemoteHost.sendBufferMSG[i],s_RemoteHost.sendBufferMSGSize[i]);
        startDataAddr+=s_RemoteHost.sendBufferMSGSize[i];
    }
    
    
}

static void _PackDataWithBigData() {
    STSOCKMSGDATA_PACKDATASIZE    *packetHeader;
    char                          *startDataAddr;
    int                            sizeDataCanSend = 0;
    s32                           *headerAddr;
    packetHeader            = (STSOCKMSGDATA_PACKDATASIZE*)s_RemoteHost.sendBuffer;  // 16 byte
    
    if(s_RemoteHost.bigDataSending == true)
        packetHeader->type      = packetHeader->type | STSOCKMSG_BIGDATA;
    else if(s_RemoteHost.fileDataSending == true)
        packetHeader->type      = packetHeader->type | STSOCKMSG_FILEDATA;
    
    startDataAddr           = (char *)&s_RemoteHost.sendBuffer[packetHeader->sizeAll + sizeof(STSOCKMSGDATA_DATASIZE)];
    headerAddr              = (s32*)startDataAddr;
    startDataAddr +=8;
    
    sizeDataCanSend = (60 << 10) - packetHeader->sizeAll;
    packetHeader->sizeAll += 8;
    
    // have 8 byte header for flag
    if(s_RemoteHost.bigDataSendSize == 0) {
        // first packet
        headerAddr[0]  = STSOCKMSG_BIGDATA_HEADER;
        headerAddr[1]  = s_RemoteHost.bigDataSize;
        if(s_RemoteHost.fileDataSending == true) {
            memcpy((char*)&headerAddr[2],s_RemoteHost.sendFileName,32);
            startDataAddr += 32;
            packetHeader->sizeAll += 32;
        }
    }
    else {
        headerAddr[0]  = STSOCKMSG_BIGDATA_CONTINUE;
        headerAddr[1]  = s_RemoteHost.bigDataSize;
    }
    
    if(sizeDataCanSend > s_RemoteHost.bigDataSize - s_RemoteHost.bigDataSendSize) {
        
        memcpy(startDataAddr, &s_RemoteHost.bigDataBuffer[s_RemoteHost.bigDataSendSize],s_RemoteHost.bigDataSize - s_RemoteHost.bigDataSendSize);
        packetHeader->sizeAll += (s_RemoteHost.bigDataSize - s_RemoteHost.bigDataSendSize);
        s_RemoteHost.bigDataSendSize += s_RemoteHost.bigDataSize - s_RemoteHost.bigDataSendSize;
        
        // printf("Big Data Pack End = %d\n",s_RemoteHost.bigDataSize - s_RemoteHost.bigDataSendSize);
    }
    else {
        memcpy(startDataAddr, &s_RemoteHost.bigDataBuffer[s_RemoteHost.bigDataSendSize],sizeDataCanSend);
        s_RemoteHost.bigDataSendSize += sizeDataCanSend;
        packetHeader->sizeAll += sizeDataCanSend;
        // printf("Big Data Pack = %d\n %d / %d",sizeDataCanSend,s_RemoteHost.bigDataSendSize,s_RemoteHost.bigDataSize);
    }
    
    if(s_RemoteHost.bigDataSendSize == s_RemoteHost.bigDataSize) {
        s_RemoteHost.bigDataSending = false;
        s_RemoteHost.fileDataSending = false;
        headerAddr[0]  |= STSOCKMSG_BIGDATA_END;
        headerAddr[1]  = s_RemoteHost.bigDataSize;
    }
}

static void _PackDataWithCRC16() {
    STSOCKMSGDATA_PACKDATASIZE    *packetHeader;
    packetHeader            = (STSOCKMSGDATA_PACKDATASIZE  *)s_RemoteHost.sendBuffer;  // 16 byte
    u16  crc16 = STOS_CRC16(&s_RemoteHost.sendBuffer[sizeof(STSOCKMSGDATA_PACKDATASIZE)],packetHeader->sizeAll-4);
    packetHeader->crc = crc16;
}

static void STSC_CreateProcessThread(){
    if(s_RemoteHost.thread == 1)
    {
#ifdef STAR_WINDOWS_VERSION
        CloseHandle(s_RemoteHost.thread_ID);
#else
        int rex = pthread_join(s_RemoteHost.thread_ID, NULL);
#endif
    }
    
    s_RemoteHost.thread = 0;
    
#ifdef STAR_WINDOWS_VERSION
    s_RemoteHost.thread_ID = CreateThread(NULL,0,STSOCK_IOThreadFunc,&s_RemoteHost,0,&s_RemoteHost.thread_CreateID);
#else
    int rex = pthread_create(&s_RemoteHost.thread_ID,NULL,STSOCK_IOThreadFunc,(void *)&s_RemoteHost);
#endif
    
    
    s_RemoteHost.tickCount = STOS_GetTickCount();
    
    s_RemoteHost.packRef = 1;
    s_RemoteHost.canSendData = true;
    s_RemoteHost.bGotConfirm = true;
    s_RemoteHost.sendDataSize = 0;
    
    s_scStatus   = SC_STATUS_CHILD;
    s_nTimeStart = STOS_GetTickCount();
}

void STSC_NextFrame() {  // call when send every packet done engine will start send from here
    
    if(s_nConnectionType == STAR_CLIENT_SECTION &&
       (s_RemoteHost.sendCount > 0 || s_RemoteHost.bigDataSending == true || s_RemoteHost.fileDataSending == true)) {
        if(s_scStatus == SC_STATUS_ERROR) {
            STSC_ResetStatus();
            STSC_Connect(s_RemoteHost.serverIP, s_RemoteHost.serverPort);
            return;
        }
    }
    
    if(s_scStatus == SC_STATUS_ERROR || s_scStatus == SC_STATUS_NONE || s_scStatus == SC_STATUS_CONNECTING || s_scStatus == SC_STATUS_READY) {
        return;
    }
    
    if(s_scStatus == SC_STATUS_CONNECTED) {
        STSC_CreateProcessThread();
    }
    // นับจำนวนเฟรมที่ได้รับข้อมูลมา ต้องรีบ เคลียก่อนที่จะหาย ไม่งั้นจะลบมันทิ้งทั้งหมด
    if(s_RemoteHost.recvCount > 0) {
#ifndef STAR_WINDOWS_VERSION
        s_RemoteHost.recvDestroyCount++;
        if(s_RemoteHost.recvDestroyCount > 2)
            STSC_EndGetData();
#endif
    }
    
    if(s_RemoteHost.canSendData == true) {
        // กันไม่ให้มีการส่ง พี่มากเกินไป ถ้าเกินเวลาก็จะส่ง packetเปล่า ๆ ไปแทน
        // Optimize wait for next frame and
        // Wait for next frame
        //fix 2.59.2
        if(STOS_GetTickCount() - s_RemoteHost.tickCount <  s_nFrameTime) {
            return;
        }
    }
    //    ถ้าถึงรอบส่งแล้วยัง busy อยู่แปลว่า lag มาก ๆ แล้ว ให้ออกทันทีไม่ต้องรอ ไม่งั้นจะ loop ค้าง
    //    while (s_scStatus == SC_STATUS_BUSY); // fix 2.59.2
    if (s_scStatus == SC_STATUS_BUSY) {
        return;
    }
    
    if(s_RemoteHost.canSendData == false) {
        if(s_bSendHeader == true) {// ส่งส่วนที่เหลือถ้ายังส่งไม่เสร็จ
#ifdef STAR_WINDOWS_VERSION
            WaitForSingleObject(s_MutexBusy,INFINITE);
#else
            pthread_mutex_lock(&s_MutexBusy);
#endif
            s_scStatus = SC_STATUS_BUSY;
            
            int byte_send;
            if(s_nSendSize - s_nSendingSize >= SIZE_SEND_PER_FRAME)
                byte_send = WriteTo(s_RemoteHost.socket, &s_RemoteHost.sendBuffer[s_nSendingSize], SIZE_SEND_PER_FRAME , (struct sockaddr *)&s_RemoteHost.inetSockAddr, s_RemoteHost.addrLen);
            else
                byte_send = WriteTo(s_RemoteHost.socket, &s_RemoteHost.sendBuffer[s_nSendingSize], s_nSendSize - s_nSendingSize , (struct sockaddr *)&s_RemoteHost.inetSockAddr, s_RemoteHost.addrLen);
            
            s_nSendingSize += byte_send;
            
            if(s_nSendingSize == s_nSendSize)
                s_bSendHeader = false;
            
            s_scStatus = SC_STATUS_CHILD;
#ifdef STAR_WINDOWS_VERSION
            ReleaseMutex(s_MutexBusy);
#else
            pthread_mutex_unlock(&s_MutexBusy);
#endif
        }
        return;
    }
    
    s_scStatus  = SC_STATUS_BUSY;
    //     ========================
    //     เริ่ม pack และส่งข้อมูล
    //     ========================
    
    if(s_bSendHeader == false)
    {
#ifdef STAR_WINDOWS_VERSION
        WaitForSingleObject(s_MutexBusy,INFINITE);
#else
        pthread_mutex_lock(&s_MutexBusy);
#endif
        STSOCKMSGDATA_PACKDATASIZE    *packetHeader;
        packetHeader  = (STSOCKMSGDATA_PACKDATASIZE  *)s_RemoteHost.sendBuffer;  // 20 byte
        
        _PackDataWithPacket();
        if(s_RemoteHost.bigDataSending == true || s_RemoteHost.fileDataSending == true) {
            _PackDataWithBigData();
        }
        _PackDataWithCRC16();
        
        s32  sendsize = packetHeader->sizeAll + sizeof(STSOCKMSGDATA_DATASIZE); // Add 16 byte here
        s_RemoteHost.sendDataSize = sendsize;
        
        s_nSendSize    = sendsize;
        s_nSendingSize = 0;
        s_bSendHeader  = true;
        
        int byte_send;
        //byte_send = WriteTo(s_RemoteHost.socket, s_RemoteHost.sendBuffer, sendsize, (struct sockaddr *)&s_RemoteHost.inetSockAddr, s_RemoteHost.addrLen);
        if(sendsize >= SIZE_SEND_PER_FRAME)
            byte_send = WriteTo(s_RemoteHost.socket, s_RemoteHost.sendBuffer, SIZE_SEND_PER_FRAME , (struct sockaddr *)&s_RemoteHost.inetSockAddr, s_RemoteHost.addrLen);
        else
            byte_send = WriteTo(s_RemoteHost.socket, s_RemoteHost.sendBuffer, sendsize , (struct sockaddr *)&s_RemoteHost.inetSockAddr, s_RemoteHost.addrLen);
        s_nSendingSize = byte_send;
        
        if(byte_send != 0) {
            s_RemoteHost.tickCount = STOS_GetTickCount();
            s_RemoteHost.canSendData = false; // block จนกว่าจะได้ confirm ถึงจะส่งต่อได้
            s_RemoteHost.bGotConfirm = false;
            // s_RemoteHost.recvCount   = 0;
            s_RemoteHost.sendCount   = 0;
            s_RemoteHost.waitingCount= 0;
            
            s_RemoteHost.curPacketNo = 0;
            
            if(s_nSendingSize == s_nSendSize)
                s_bSendHeader = false;
            
            s_scStatus  = SC_STATUS_CHILD;
        }
        else if(byte_send == 0) {
            s_nError = STAR_CLIENT_ERROR_DISCONNECT;
            s_scStatus = SC_STATUS_ERROR;
        }
#ifdef STAR_WINDOWS_VERSION
        ReleaseMutex(s_MutexBusy);
#else
        pthread_mutex_unlock(&s_MutexBusy);
#endif
    }
}



#ifdef STAR_WINDOWS_VERSION
DWORD WINAPI STSOCK_ConnectThreadFunc(LPVOID ptr)
#else
static void *STSOCK_ConnectThreadFunc(void *ptr)
#endif
{
    RemoteHost_t  *host = (RemoteHost_t*)ptr;
    SOCKET  sock;
    int type;
    if(s_nConnectType == STAR_CLIENT_TCP_CONNECT)
        type = SOCK_STREAM;
    else if(s_nConnectType == STAR_CLIENT_UDP_CONNECT)
        type = SOCK_DGRAM;
    /*--------------------------------------------------
     Connecting
     -------------------------------------------------*/
    sock = socket(AF_INET,type,0);
    if(sock == INVALID_SOCKET) {
        s_nError    = STAR_CLIENT_ERROR_CONNECT_FAIL;
        s_scStatus  = SC_STATUS_ERROR;
        return (NULL);
    }
    
    struct  sockaddr_in     inetServAddr;
    memset((char*)&inetServAddr,0,sizeof(inetServAddr));
    
    inetServAddr.sin_family       = AF_INET;
    inetServAddr.sin_port         = htons((u_short)host->serverPort);
    inetServAddr.sin_addr.s_addr  = (u_int)host->serverIP;
    
#ifdef STAR_WINDOWS_VERSION
    u_long set = 0;
    ioctlsocket(sock,FIONBIO, &set); // set none blocking mode
#else
    u_long set = 1;
    ioctl(sock,FIONBIO, &set); // set none blocking mode
#endif
    if(s_nConnectType == STAR_CLIENT_TCP_CONNECT)
    {
        int error = connect(sock, (struct sockaddr*)&inetServAddr, sizeof(inetServAddr));
#ifdef STAR_WINDOWS_VERSION
        int  len = sizeof(error);
        if(error == SOCKET_ERROR)
            error = WSAGetLastError();
#else
        socklen_t  len = sizeof(error);
#endif
        if(error != 0) {
#ifdef STAR_WINDOWS_VERSION
            if(errno == WSAENETUNREACH)
#else
                if(errno == ENETUNREACH)
#endif
                {
                    s_nError    = STAR_CLIENT_ERROR_NO_INTERNET;
                    s_scStatus  = SC_STATUS_ERROR;
                    return NULL;
                }
#ifdef STAR_WINDOWS_VERSION
                else if(errno == WSAEINPROGRESS)
#else
                    else if(errno == EINPROGRESS)
#endif
                    {
                        fd_set fds;
                        FD_ZERO(&fds);
                        FD_SET(sock,&fds);
                        
                        struct timeval tv;
                        tv.tv_sec = STAR_CLIENT_DEFAULT_CON_TIMEOUT;
                        tv.tv_usec = 0;
                        
                        int ret = select(sock+1,NULL,&fds,NULL,&tv);
                        if(ret  <= 0) {
                            s_nError    = STAR_CLIENT_ERROR_SERVER_FAIL;
                            s_scStatus  = SC_STATUS_ERROR;
                            return NULL;
                        }
                        else {
                            //==============================================
                            // Fix on 2.59.2
                            //==============================================
                            if(getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len) < 0) {
                                s_nError    = STAR_CLIENT_ERROR_CONNECT_FAIL;
                                s_scStatus  = SC_STATUS_ERROR;
                                return (NULL);
                            }
                            if (error > 0) {
                                s_nError    = STAR_CLIENT_ERROR_SERVER_FAIL;
                                s_scStatus  = SC_STATUS_ERROR;
                                return NULL;
                            }
                        }
                    }
                    else {
                        s_nError    = STAR_CLIENT_ERROR_CONNECT_FAIL;
                        s_scStatus  = SC_STATUS_ERROR;
                        return (NULL);
                    }
        }
    }
    
    host->socket       = sock;
    host->inetSockAddr = inetServAddr;
    host->addrLen      = (socklen_t) sizeof(inetServAddr);
    host->online       = 1;
    host->canSendData  = false;
    
    if(s_nConnectType == STAR_CLIENT_UDP_CONNECT) {
        STSOCKMSG_GENERIC   knockMsg;
        
        // Fill the message data
        knockMsg.type       =   STSOCKMSG_KNOCK;
        knockMsg.packNo     =   s_RemoteHost.packRef;
        knockMsg.fromID     =   STSOCKID_UNKNOWS;
        
        int byte = WriteTo(s_RemoteHost.socket, (char*)&knockMsg, sizeof(STSOCKMSG_GENERIC), (struct sockaddr *)&s_RemoteHost.inetSockAddr, sizeof(s_RemoteHost.inetSockAddr));
        
        if(!byte) {
            s_nError    = STAR_CLIENT_ERROR_CONNECT_FAIL;
            s_scStatus  = SC_STATUS_ERROR;
            return NULL;
        }
    }
    
    /*--------------------------------------------------
     Recieve Response
     -------------------------------------------------*/
    
    // Recieve Message From Server Make sure got Data
    // Set Non-Blocking
    set = s_bBlocking;
#ifdef STAR_WINDOWS_VERSION
    ioctlsocket(host->socket,FIONBIO, &set);
#else
    ioctl(host->socket,FIONBIO, &set);
#endif
    
    //===============================================================
    
    // will connect 5 Second
    STSOCKMSG_GENERIC  *Msg;
    u32 starttime = STOS_GetTickCount();
    int recvSize = 0;
    bool checkok = false;
    
    while (s_nLocalID < 0 || checkok == false) {
        if(STOS_GetTickCount() - starttime > STAR_CLIENT_DEFAULT_CON_TIMEOUT * 1000) {
            s_nError    = STAR_CLIENT_ERROR_SERVER_FAIL;
            s_scStatus  = SC_STATUS_ERROR;
            return (NULL);
        }
        
        struct sockaddr_in newAddr;
        socklen_t  newLen = sizeof(newAddr);
        
        memset((char *)host->recvBuffer, 0, sizeof(char));
        
        recvSize = ReadFrom(host->socket,(char*)host->recvBuffer, sizeof(STSOCKMSGDATA_DATASIZE), (struct sockaddr *)&newAddr, &newLen);
        if( recvSize > 0)
        {
            Msg = (STSOCKMSG_GENERIC*)host->recvBuffer;
            
            if(s_nConnectType == STAR_CLIENT_UDP_CONNECT) {
                host->inetSockAddr = newAddr;
                host->addrLen      = newLen;
            }
            
            switch (Msg->type) {
                case STSOCKMSG_GIVEID:
                    STSOCKMSG_CONF    confMsg;
                    STSOCKMSGDATA_DATASIZE *idMSG;
                    idMSG = (STSOCKMSGDATA_DATASIZE*) Msg;
                    
                    s_nLocalID = idMSG->packNo; // use to store local id on first time
                    
                    confMsg.conf = s_nLocalID;
                    
                    STSOCKMSGDATA_DATASIZE  *sendRegis;
                    StarVersion             *sendVersion;
                    StarVersion             *clientVersion;
                    char regisBuffer[sizeof(STSOCKMSGDATA_DATASIZE) + (sizeof(StarVersion) * 2)];
                    
                    sendRegis   = (STSOCKMSGDATA_DATASIZE*)regisBuffer;
                    sendVersion = (StarVersion*)&regisBuffer[sizeof(STSOCKMSGDATA_DATASIZE)];
                    clientVersion = (StarVersion*)&regisBuffer[sizeof(STSOCKMSGDATA_DATASIZE) + sizeof(StarVersion)];
                    
                    sendRegis->type = STSOCKMSG_REGISTER;
                    sendRegis->size = sizeof(StarVersion) * 2;
                    
                    *sendVersion = version;
                    *clientVersion = s_ClientVersion;
                    WriteTo(host->socket, (char*)&regisBuffer, sizeof(STSOCKMSGDATA_DATASIZE) + (sizeof(StarVersion) * 2), (struct sockaddr *)&host->inetSockAddr, sizeof(host->inetSockAddr));
                    break;
                case STSOCKMSG_REGISTER:
                    
                    if(Msg->fromID == STSOCKMSG_REGIS_OK_ALWAY) {
                        s_nConnectionType = STAR_CLIENT_ALWAY_ON;
                        checkok = true;
                    }
                    else if(Msg->fromID == STSOCKMSG_REGIS_OK_SECTION) {
                        s_nConnectionType = STAR_CLIENT_SECTION;
                        checkok = true;
                    }
                    else if(Msg->fromID == STSOCKMSG_OLD_VERSION) {
                        s_nError    = STAR_CLIENT_ERROR_OLD_VERSION;
                        s_scStatus  = SC_STATUS_ERROR;
                        return (NULL);
                    }
                    else if(Msg->fromID == STSOCKMSG_SERVER_FULL) {
                        s_nError    = STAR_CLIENT_ERROR_SERVERFULL;
                        s_scStatus  = SC_STATUS_ERROR;
                        return (NULL);
                    }
                    else if(Msg->fromID == STSOCKMSG_CLIENT_WRONG) {
                        s_nError    = STAR_CLIENT_ERROR_NOT_SUPPORT;
                        s_scStatus  = SC_STATUS_ERROR;
                        return (NULL);
                    }
                    break;
                default:
                    s_nError    = STAR_CLIENT_ERROR_OLD_VERSION;
                    s_scStatus  = SC_STATUS_ERROR;
                    return (NULL);
                    break;
            }
        }
        else if(recvSize <= 0) {
            s_nError    = STAR_CLIENT_ERROR_OLD_VERSION;
            s_scStatus  = SC_STATUS_ERROR;
            return (NULL);
        }
    }
    
    s_scStatus = SC_STATUS_CONNECTED;
    return (NULL);
}
#pragma mark -
#pragma mark Recieve Thread
#pragma mark -
/*---------------------------------------------------------------------------*
 Recieve Data Thread
 *---------------------------------------------------------------------------*/
void processCheckSendData(RemoteHost_t *host) {
    if(host->canSendData == false && host->bGotConfirm == false) {
        if(STOS_GetTickCount() - host->tickCount > s_nReTrasmit) {
            // Adjust Restarsmit Data Here to match speed
            // FIX 2.59.2
            s_nReTrasmit = STOS_GetTickCount() - host->tickCount;
            if(s_nReTrasmit > s_nFrameTime - STAR_CLIENT_DEFAULT_RETRASMIT)
                s_nReTrasmit = s_nFrameTime - STAR_CLIENT_DEFAULT_RETRASMIT;
            //==================================================
#ifdef STAR_WINDOWS_VERSION
            WaitForSingleObject(s_MutexBusy,INFINITE);
#else
            pthread_mutex_lock(&s_MutexBusy);
#endif
            s_scStatus  = SC_STATUS_BUSY;
            WriteTo(host->socket, host->sendBuffer, host->sendDataSize, (struct sockaddr *)&host->inetSockAddr, host->addrLen);
            host->tickCount = STOS_GetTickCount();
            host->waitingCount++;
            if(host->waitingCount *s_nReTrasmit >  STAR_CLIENT_DEFAULT_WATING_TIME)
                s_nError = STAR_CLIENT_ERROR_WAITING_DATA;
            
            s_scStatus  = SC_STATUS_CHILD;
#ifdef STAR_WINDOWS_VERSION
            ReleaseMutex(s_MutexBusy);
#else
            pthread_mutex_unlock(&s_MutexBusy);
#endif
        }
    }
}

static void processMessagePackage(RemoteHost_t *host,int msgType){
    
    STSOCKMSGDATA_DATASIZE      *DataMsg;
    STSOCKMSGDATA_PACKDATASIZE  *PackData;
    int                         readSize;
    u16                         crc16,dataCRC16;
    
    DataMsg = (STSOCKMSGDATA_DATASIZE *)host->recvBuffer;
    if (DataMsg->size > 0 && DataMsg->size < 63488) {
        readSize =ReadFrom(host->socket, (char*) &host->recvBuffer[sizeof(STSOCKMSGDATA_DATASIZE)], DataMsg->size, (struct sockaddr*)&host->inetSockAddr, &host->addrLen);
    }
    else {
        SendReTrasmit();
        return;
    }
    
    
    //printf("Got Data Packet %d/%d\n",readSize,DataMsg->size);
    
    if(readSize != DataMsg->size)// Lost Data Retrasmit
    {
        SendReTrasmit();
        return;
    }
    
    PackData = (STSOCKMSGDATA_PACKDATASIZE *)host->recvBuffer;
    crc16 = STOS_CRC16(&host->recvBuffer[sizeof(STSOCKMSGDATA_PACKDATASIZE)],PackData->sizeAll - 4);
    dataCRC16 = *(u16*)&host->recvBuffer[sizeof(STSOCKMSGDATA_DATASIZE)];
    
    if(PackData->packNo == host->packRef) {
        host->packRef++;
        host->canSendData = true;
        host->bGotConfirm = true;
        s_nError = STAR_CLIENT_ERROR_NONE;
        s_nResponTime = STOS_GetTickCount() - host->tickCount;
    }
    
    if(crc16 != dataCRC16) {
        SendReTrasmit();
        return;
    }
    
    host->processRecieve = true;
    host->recvDestroyCount = 0;
    
    if(msgType  & STSOCKMSG_PACKDATASIZE) {
        memcpy(host->tempRecvBuffer[host->maxRecvBuffer], host->recvBuffer, STAR_CLIENT_DEFAULT_BUFFER * STAR_CLIENT_DEFAULT_MAX_BLOCK);
        host->maxRecvBuffer++;
        if(host->maxRecvBuffer == 10)
            host->maxRecvBuffer = 0;
    }
    
    if(msgType & STSOCKMSG_FILEDATA) {
        _ProessMessageFileData(host);
    }
    
    host->processRecieve = false;
}

//return 1 mean have message 0 mean not have
static int processMessageBuffer(RemoteHost_t *host){
    
    STSOCKMSGDATA_DATASIZE      *DataMsg;
    STSOCKMSG_GENERIC           *Msg;
    int                         readSize;
    
    Msg = (STSOCKMSG_GENERIC*)host->recvBuffer;
    readSize = ReadFrom(host->socket, (char*) host->recvBuffer, sizeof(STSOCKMSGDATA_DATASIZE), (struct sockaddr*)&host->inetSockAddr, &host->addrLen);
    
    if(readSize <= 0) {
        s_nError = STAR_CLIENT_ERROR_DISCONNECT;
        s_scStatus = SC_STATUS_ERROR;
        
        return 0;
    }
    else if(readSize != sizeof(STSOCKMSGDATA_DATASIZE)) {
        SendReTrasmit();
        return 0;
    }
    
    switch (Msg->type) {
        case STSOCKMSG_SERVER_FULL:
            s_nError = STAR_CLIENT_ERROR_SERVERFULL;
            s_scStatus = SC_STATUS_ERROR;
            return 0;
            break;
        case STSOCKMSG_DUPLICATE:
            s_nError = STAR_CLIENT_ERROR_DUPLICATE;
            s_scStatus = SC_STATUS_ERROR;
            return 0;
            break;
            // packet โดนบวกเพิ่มที่ตรงนี้
        case STSOCKMSG_CONFIRM:
            DataMsg = (STSOCKMSGDATA_DATASIZE *) host->recvBuffer;
            if(DataMsg->packNo == host->packRef) {
                host->packRef++;
                host->canSendData = true;
                host->bGotConfirm = true;
                s_nError = STAR_CLIENT_ERROR_NONE;
                s_nResponTime = STOS_GetTickCount() - host->tickCount;
            }
            return 0;
            break;
        case STSOCKMSG_STREAMDATA: // Patch speed read data
            break;
        case STSOCKMSG_CLOSE_CONNECTION:
            s_nError = STAR_CLIENT_ERROR_DISCONNECT;
            s_scStatus = SC_STATUS_ERROR;
            return 0;
            // Disconnect From Server
            break;
        default:
            if((Msg->type <= STSOCKMSG_ALLDATA)) {
                return Msg->type;
            }
            else {
                // SendReTrasmit();
                return 0;
            }
            
            break;
    }
    return 0;
}

#ifdef STAR_WINDOWS_VERSION
DWORD WINAPI STSOCK_IOThreadFunc(LPVOID paramPtr)
#else
static void *STSOCK_IOThreadFunc(void *paramPtr)
#endif
{
    RemoteHost_t   *host = (RemoteHost_t*)paramPtr;
    u32    timeOut;
    int ret;
    struct timeval tv;
    
    int maxfd = host->socket;
    tv.tv_sec = STAR_CLIENT_DEFAULT_TIMEOUT;
    tv.tv_usec = 0;
    
    fd_set allset;
    FD_ZERO(&allset);
    FD_SET(maxfd, &allset);
    
    timeOut = STOS_GetTickCount();
    
    host->thread = 1;
    
    while (host->online) {
        
        tv.tv_sec =  0;//STAR_CLIENT_DEFAULT_TIMEOUT;
        tv.tv_usec = s_nReTrasmit * 1000;
        fd_set reading = allset;
        int nready = select(maxfd+1, &reading, NULL,NULL, &tv);
        
        if(nready>0)
        {
#ifdef STAR_WINDOWS_VERSION
            WaitForSingleObject(s_MutexBusy,INFINITE);
#else
            pthread_mutex_lock(&s_MutexBusy);
#endif
            ret = processMessageBuffer(host);
            if(ret > 0) {
                processMessagePackage(host,ret);
            }
            
            if(s_scStatus == SC_STATUS_ERROR) {
#ifdef STAR_WINDOWS_VERSION
                ReleaseMutex(s_MutexBusy);
#else
                pthread_mutex_unlock(&s_MutexBusy);
#endif
                break;
            }
            s_scStatus  = SC_STATUS_CHILD;
            
#ifdef STAR_WINDOWS_VERSION
            ReleaseMutex(s_MutexBusy);
#else
            pthread_mutex_unlock(&s_MutexBusy);
#endif
            timeOut = STOS_GetTickCount(); // reset timeout
        }
        else if(nready == 0) {
            // Time Out
            if(STOS_GetTickCount() - timeOut > STAR_CLIENT_DEFAULT_TIMEOUT * 1000) {
                
                s_scStatus = SC_STATUS_ERROR;
                s_nError   = STAR_CLIENT_ERROR_TIMEOUT;
                break;
            }
            else {
                if (s_nConnectionType == STAR_CLIENT_ALWAY_ON) {
                    // processCheckSendData(host); // resend only UDP
                }
            }
        }
        else {
            // Disconnect
            s_scStatus = SC_STATUS_ERROR;
            s_nError   = STAR_CLIENT_ERROR_DISCONNECT;
            break;
        }
    }
    host->online = false;
    return (NULL);
}

#pragma mark -
#pragma mark Stream Data
#pragma mark -
bool STSC_HaveStreamData(){
    return (s_RemoteHost.streamSize > 0 && s_RemoteHost.processRecieve == false);
}

char*STSC_GetStreamDataPointer() {
    return s_RemoteHost.streamDataBuffer;
}

u32  STSC_GetStreamDataSize() {
    return s_RemoteHost.streamSize;
}

void STSC_ReleaseStreamData() { // Will autoRelease if have new stream data
    if (s_RemoteHost.streamDataBuffer != NULL) {
        STOS_Free(s_RemoteHost.streamDataBuffer);
        s_RemoteHost.streamDataBuffer = NULL;
        s_RemoteHost.streamSize = 0;
    }
}

void STSC_Init(u16 connectType) {
    u32 buffer = STAR_CLIENT_DEFAULT_BUFFER;
    if(s_scStatus != SC_STATUS_NONE) return;
    
    s_nTimeOut      = STAR_CLIENT_DEFAULT_TIMEOUT;
    s_nReTrasmit    = STAR_CLIENT_DEFAULT_RETRASMIT;
    s_nFrameTime    = (1000/STAR_CLIENT_DEFAULT_FRAME) - 32 ; //  30 frame per sec
    s_nConnectType  = connectType;
    s_scStatus      = SC_STATUS_READY;
    s_nError        = STAR_CLIENT_ERROR_NONE;
    s_bBlocking     = 0; // None Blocking Mode   1 = none blocking mode ก็ตือ เปิดไม่บล๊อก ถ้า 0 คือ none blocking mode ปิด คือ block นั้นเอง;
    s_nConnectionType = 0;
    s_nDataRecvByte = 0;
    s_nDataSendByte = 0;
    s_bSendHeader   = false;
    
    memset((char *)&s_RemoteHost, 0, sizeof(s_RemoteHost));
    
    for (int i=0; i<STAR_CLIENT_DEFAULT_MAX_BLOCK; i++) {
        s_RemoteHost.sendBufferMSG[i]      = (char *)STOS_Alloc(buffer);
        
    }
    
    s_RemoteHost.sendBuffer =(char*) STOS_Alloc(buffer * STAR_CLIENT_DEFAULT_MAX_BLOCK);
    s_RemoteHost.recvBuffer =(char*) STOS_Alloc(buffer * STAR_CLIENT_DEFAULT_MAX_BLOCK);
    
    for (int i=0; i<10; i++) {
        s_RemoteHost.tempRecvBuffer[i] = (char*) STOS_Alloc(buffer * STAR_CLIENT_DEFAULT_MAX_BLOCK);
    }
    
    s_nLocalID           = -1;
    s_RemoteHost.packRef = 0;
    
#ifdef STAR_WINDOWS_VERSION
    s_MutexBusy = CreateMutex(NULL,FALSE,NULL);
    //  s_MutexBusyRead = CreateMutex(NULL,FALSE,NULL);
    //  s_MutexWaitGetData = CreateMutex(NULL,FALSE,NULL);
#endif
    s_ClientVersion.major = 0;
    s_ClientVersion.minor = 0;
    s_ClientVersion.revision = 0;
}

void STSC_SetClientVersion(s32 major,s32 miner,s32 revision) {
    s_ClientVersion.major = major;
    s_ClientVersion.minor = miner;
    s_ClientVersion.revision = revision;
}

void STSC_ResetStatus() {
    
    s_scStatus = SC_STATUS_READY;
    s_nError   = STAR_CLIENT_ERROR_NONE;
    s_RemoteHost.canSendData = false;
    s_RemoteHost.online = 0;
    s_RemoteHost.CreateConnectThread = false; // only For flash
    
    s_nDataRecvByte = 0;
    s_nDataSendByte = 0;
    s_RemoteHost.bGotConfirm  = false;
    s_RemoteHost.sendDataSize = 0;
    s_nLocalID     = -1;
    s_RemoteHost.packRef = 0;
    s_bSendHeader = false;
    
    s_RemoteHost.recvCount   = 0;
    s_RemoteHost.sendCount   = 0;
    s_RemoteHost.currentRecvIdx = 0;
    s_RemoteHost.maxRecvBuffer = 0;
    s_RemoteHost.waitingCount= 0;
    s_RemoteHost.curPacketNo = 0;
    s_RemoteHost.bigDataSending = false;
    s_RemoteHost.fileDataSending = false;
    
    if(s_RemoteHost.socket > 0) {
        struct linger Ling;
        
        Ling.l_onoff  = 1;
        Ling.l_linger = 0;
        
        setsockopt(s_RemoteHost.socket, SOL_SOCKET, SO_LINGER, (const char*)&Ling, sizeof(struct linger));
#ifdef STAR_WINDOWS_VERSION
        shutdown(s_RemoteHost.socket, 2);
        closesocket(s_RemoteHost.socket);
#else
        shutdown(s_RemoteHost.socket, SHUT_RDWR);
        int ret = -1;
        while (ret == -1) {
            ret = close(s_RemoteHost.socket);
        }
#endif
    }
    
    if(s_RemoteHost.thread == 1) {
#ifdef STAR_WINDOWS_VERSION
        CloseHandle(s_RemoteHost.thread_ID);
        //CloseHandle(s_MutexBusy);
        //  ReleaseMutex(s_MutexWaitGetData);
#else
        pthread_join(s_RemoteHost.thread_ID, NULL);
        //pthread_mutex_destroy(&s_MutexBusy);
        //    pthread_mutex_unlock(&s_MutexWaitGetData);
#endif
    }
    
    s_RemoteHost.socket = 0;
    s_RemoteHost.thread = 0;
    
    // memset((char *)&s_RemoteHost, 0, sizeof(s_RemoteHost));
}

void STSC_Connect(u32 ipValue,u16 port) {
    
    // Reconnect 2.67
    if(s_scStatus == SC_STATUS_ERROR || s_scStatus == SC_STATUS_CHILD) {
        if(s_scStatus == SC_STATUS_CHILD) {
            STSC_Disconnect();
        }
        
        STSC_ResetStatus();
    }
    
    // Reset s_nReTrasmit Time Again
    // FIX 2.59.2
    s_nReTrasmit    = STAR_CLIENT_DEFAULT_RETRASMIT;
    s_nError        = STAR_CLIENT_ERROR_NONE;
    
    if (s_scStatus == SC_STATUS_CONNECTING ) {
        return;
    }
    
    //#ifdef STAR_DEBUG
    if(s_nConnectType != STAR_CLIENT_TCP_CONNECT && s_nConnectType != STAR_CLIENT_UDP_CONNECT) {
        s_nError    = STAR_CLIENT_ERROR_CONNECT_FAIL;
        s_scStatus  = SC_STATUS_ERROR;
        return;
    }
    //#endif
    
    if(s_scStatus != SC_STATUS_READY)
    {
        s_nError    = STAR_CLIENT_ERROR_CONNECT_FAIL;
        s_scStatus  = SC_STATUS_ERROR;
        return;
    }
    s_scStatus = SC_STATUS_CONNECTING;
    s_RemoteHost.serverIP = ipValue;
    s_RemoteHost.serverPort = port;
    
#if STAR_WINDOWS_VERSION
    s_RemoteHost.thread_ID = CreateThread(NULL,0,STSOCK_ConnectThreadFunc,&s_RemoteHost,0,&s_RemoteHost.thread_CreateID);
#else
    int rex = pthread_create(&s_RemoteHost.thread_ID,NULL,STSOCK_ConnectThreadFunc,(void *)&s_RemoteHost);
#endif
}

void STSC_Disconnect() {
    if(s_scStatus != SC_STATUS_CHILD)
        return;
    
    STSOCKMSG_GENERIC           Msg;
    STSOCKMSGDATA_DATASIZE      DataMsg;
    memset(&DataMsg, 0, sizeof(STSOCKMSGDATA_DATASIZE));
    
    DataMsg.type  = STSOCKMSG_DATASIZE;
    DataMsg.size  = sizeof(STSOCKMSG_GENERIC);
    
    
    Msg.type = STSOCKMSG_CLOSE_CONNECTION;
    Msg.fromID  = s_nLocalID;
    Msg.packNo    = s_RemoteHost.packRef;
    
    WriteTo(s_RemoteHost.socket,(char *)&DataMsg,sizeof(STSOCKMSGDATA_DATASIZE),(struct sockaddr*)&s_RemoteHost.inetSockAddr,s_RemoteHost.addrLen);
    WriteTo(s_RemoteHost.socket,(char *)&Msg, sizeof(STSOCKMSG_GENERIC),(struct sockaddr*)&s_RemoteHost.inetSockAddr,s_RemoteHost.addrLen);
    
    struct linger Ling;
    
    Ling.l_onoff  = 1;
    Ling.l_linger = 0;
    
    setsockopt(s_RemoteHost.socket, SOL_SOCKET, SO_LINGER, (const char*)&Ling, sizeof(struct linger));
    
#ifdef STAR_WINDOWS_VERSION
    shutdown(s_RemoteHost.socket, 2);
    closesocket(s_RemoteHost.socket);
#else
    shutdown(s_RemoteHost.socket, SHUT_RDWR);
    
    int ret = -1;
    while (ret == -1) {
        ret = close(s_RemoteHost.socket);
    }
#endif
    s_RemoteHost.socket = 0;
    s_RemoteHost.online = 0;
#ifdef STAR_WINDOWS_VERSION
    CloseHandle(s_RemoteHost.thread_ID);
    //CloseHandle(s_MutexBusy);
#else
    pthread_join(s_RemoteHost.thread_ID, NULL);
    //pthread_mutex_destroy(&s_MutexBusy);
#endif
}

void STSC_Release() {
    for (int i=0; i<STAR_CLIENT_DEFAULT_MAX_BLOCK; i++) {
        STOS_Free(s_RemoteHost.sendBufferMSG[i]);
    }
    
    STOS_Free(s_RemoteHost.sendBuffer);
    STOS_Free(s_RemoteHost.recvBuffer);
    
    if(s_RemoteHost.streamDataBuffer) {
        STOS_Free(s_RemoteHost.streamDataBuffer);
        s_RemoteHost.streamDataBuffer = NULL;
        s_RemoteHost.streamSize  = 0;
    }
    
    if(s_RemoteHost.bigDataBuffer) {
        STOS_Free(s_RemoteHost.bigDataBuffer);
        s_RemoteHost.bigDataSize = 0;
    }
    
    s_scStatus = SC_STATUS_NONE;
}

bool STSC_HaveError(void) {   // please check every frame
    return (s_scStatus == SC_STATUS_ERROR);
};

u32  STSC_GetConnectionType() {
    return s_nConnectionType;
}

char* STSC_GetStatusString() {
    return SC_STATUS_STRING[s_scStatus];
}

char* STSC_GetErrorString() {
    switch (s_nError) {
        case STAR_CLIENT_ERROR_PROXY_FAIL:
            return SC_ERROR_STRING[0];
            break;
        case STAR_CLIENT_ERROR_SERVER_FAIL:
            return SC_ERROR_STRING[1];
            break;
        case STAR_CLIENT_ERROR_COUNTYR_FAIL:
            return SC_ERROR_STRING[2];
            break;
        case STAR_CLIENT_ERROR_VERSION_FAIL :
            return SC_ERROR_STRING[3];
            break;
        case STAR_CLIENT_ERROR_BAN:
            return SC_ERROR_STRING[4];
            break;
        case STAR_CLIENT_ERROR_ACCOUNT:
            return SC_ERROR_STRING[5];
            break;
        case STAR_CLIENT_ERROR_PASS:
            return SC_ERROR_STRING[6];
            break;
        case STAR_CLIENT_ERROR_MAINTAIN:
            return SC_ERROR_STRING[7];
            break;
        case STAR_CLIENT_ERROR_DISCONNECT:
            return SC_ERROR_STRING[8];
            break;
        case STAR_CLIENT_ERROR_DUPLICATE:
            return SC_ERROR_STRING[9];
            break;
        case STAR_CLIENT_ERROR_TIMEOUT:
            return SC_ERROR_STRING[10];
            break;
        case STAR_CLIENT_ERROR_CONNECT_FAIL:
            return SC_ERROR_STRING[11];
            break;
        case STAR_CLIENT_ERROR_NO_INTERNET:
            return SC_ERROR_STRING[12];
            break;
        case STAR_CLIENT_ERROR_SERVERFULL:
            return SC_ERROR_STRING[13];
            break;
        case STAR_CLIENT_ERROR_WAITING_DATA:
            return SC_ERROR_STRING[14];
            break;
        case STAR_CLIENT_ERROR_OLD_VERSION:
            return SC_ERROR_STRING[15];
            break;
        case STAR_CLIENT_ERROR_REF_NO:
            return SC_ERROR_STRING[16];
            break;
        case STAR_CLIENT_ERROR_NOT_SUPPORT:
            return SC_ERROR_STRING[17];
            break;
            
    }
    
    return NULL;
}

u32 STSC_GetResponseTime() {
    return s_nResponTime;
}

u32 STSC_GetCurFrame() {
    return s_RemoteHost.packRef;
}

u32 STSC_GetAllSendData() {
    return s_nDataSendByte;
}

u32 STSC_GetAllRecvData() {
    return s_nDataRecvByte;
}

u32 STSC_GetRunTimeMilli() {
    return STOS_GetTickCount() - s_nTimeStart;
}

f32 STSC_GetBandwidth() { // kbit/sec
    f32 bitData = (f32)((s_nDataSendByte + s_nDataRecvByte) * 8) / 1024.0f;
    f32 bandwidth = bitData /  ( (f32)STSC_GetRunTimeMilli() / 1000.0f);
    return bandwidth;
}

SCStatus STSC_GetStatus() {
    return s_scStatus;
}

u32  STSC_GetError() {
    return s_nError;
}

void STSC_SetBlockingData(bool block){
    if(s_scStatus != SC_STATUS_NONE) {
        return;
    }
    
    s_bBlocking  = block;
}

void STSC_SetTimeout(u32 second) {
    s_nTimeOut = second * 1000;
}

void STSC_SetReTrasmit(u32 microSec) {
    s_nReTrasmit = microSec;
}

void STSC_SetFrame(u32 frame) {
    s_nFrameTime = 1000/frame;
}