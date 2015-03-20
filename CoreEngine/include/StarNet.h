//
//  StarNet.h
//  StarNDKEngine
//
//  Created by SAMRET WAJANASATHIAN on 18/4/2556.
//  Copyright (c) พ.ศ. 2556 SAMRET WAJANASATHIAN. All rights reserved.
//

#ifndef __StarNDKEngine__StarNet__
#define __StarNDKEngine__StarNet__
#include "starType.h"
#include "stdio.h"
#include "stdlib.h"

#define STAR_StreamOpenCompleted        0x0001
#define STAR_StreamHasByteAvailable     0x0002
#define STAR_StreamErrorOccurred        0x0003
#define STAR_StreamEventEndEncountered  0x0004
#define STAR_StreamEventUnknow          0x0005
#define STAR_StreamSendComplete         0x0006
#define STAR_StreamSendError            0x0007
#define STAR_StreamSendSomeData         0x0008
#define STAR_StreamConnectionTimeout    0x0009
#define STAR_StreamNotConnectToInternet 0x0010 //ENETUNREACH
#define STAR_StreamDisconnectFromServer 0x0011

#define DEFAULT_BUFFER_SIZE             (1024*1024)   // 1 MB
#ifdef STAR_WINDOWS_VERSION
	typedef int socklen_t;
#else
typedef int  SOCKET;
#endif
//#ifndef __FLASHPLAYER__
enum HTTP_STATUS
{
	HTTP_SUCCESS    = 0,
	HTTP_SENDING,
	HTTP_RECIEVEING,
	HTTP_ERROR_CONNECT,
	HTTP_ERROR_IP,
	HTTP_ERROR_SEND,
	HTTP_ERROR
};

enum TCPIP_STATUS
{
	TCPIP_SUCCESS = 0,
	TCPIP_SENDING,
	TCPIP_RECIEVE,
	TCPIP_ERROR_CONNECT,
	TCPIP_ERROR_IP,
	TCPIP_ERROR_SEND,
	TCPIP_ERROR
};

//#endif

EXTERN_C_BEGIN
    WIN_DLL u32  STNET_IPTON(const char* ipAddr);
    WIN_DLL char* STNET_NTOIP(u32 ipValue);
EXTERN_C_END

//#ifndef __FLASHPLAYER__

void STNET_SetBlockingMode(SOCKET socket,bool block);
void STNET_SetTimeOut(int second); // defult 30 second

s32 STNET_InitHTTP(); // Call this function first if you want to use HTTP Connect
HTTP_STATUS STNET_HTTP_Get(const char *host,const char *page); // return success;
HTTP_STATUS STNET_HTTP_Post(const char *host,const char *page,const char *post);
HTTP_STATUS STNET_HTTP_PostImage(const char *host,const char *page,const char *post,const char* imageName,const char* fileName,const char*buffer,int bufferLength);

bool    STNET_HTTP_HaveResponse();
int     STNET_HTTP_GetErrorCode();
char*   STNET_HTTP_GetErrorString();



HTTP_STATUS STNET_HTTPGetSendingStatus();
HTTP_STATUS STNET_HTTPGetRecieveStatus();
char* STNET_HTTP_GetRespondBuffer();

//============================================
// TCP_IP

s32 STNET_InitTCP_IP();
s32 STNET_TCPConnect(const char *host,int port); // defalut Non Blocking
void STNET_TCPCallback(void(*callback)(int code));

bool STNET_HaveThreadProcess();
bool STNET_HaveConnected();
void STNET_ThreadProcess();
void STNET_TCPSetBlockingMode(bool block); // default are True
s32 STNET_TCPDisconnect();
s32 STNET_SetBufferSize(size_t send,size_t recieve);

s32 STNET_TCPSend(void* buff,int size);
s32 STNET_TCPRecieve(void* buff,int size);

TCPIP_STATUS STNET_TCPStatus();

#endif /* defined(__StarNDKEngine__StarNet__) */
