//
//  StarClient.h
//  StarMacOS
//
//  Created by SAMRET WAJANASATHIAN on 6/11/2556.
//
//

#ifndef __StarMacOS__StarClient__
#define __StarMacOS__StarClient__

#include "StarType.h"
#include "StarProtocol.h"

extern "C" {
	/*---------------------------------------------------------------------------*
	function definitions
	*---------------------------------------------------------------------------*/
	WIN_DLL void STSC_Init(u16 connectType);//,u32 buffer); // Use Network
	WIN_DLL void STSC_SetClientVersion(s32 major,s32 miner,s32 revision);
	WIN_DLL void STSC_Release();

	WIN_DLL bool STSC_HaveError(void);   // please check every frame
	WIN_DLL u32  STSC_GetError();

	WIN_DLL void STSC_Connect(u32 ipValue,u16 port);  // only tcp/udp

	WIN_DLL void STSC_Disconnect();

	WIN_DLL u32         STSC_GetConnectionType();
	WIN_DLL char*       STSC_GetStatusString();
	WIN_DLL char*       STSC_GetErrorString();
	WIN_DLL u32         STSC_GetResponseTime(); //Milli Second
	WIN_DLL u32         STSC_GetCurFrame();
	WIN_DLL SCStatus    STSC_GetStatus();
	WIN_DLL SCLinkLevel STSC_GetLinkLevel();  // depen on Wifi or Ping
	WIN_DLL u32         STSC_GetAllSendData();
	WIN_DLL u32         STSC_GetAllRecvData();
	WIN_DLL u32         STSC_GetRunTimeMilli();
	WIN_DLL f32         STSC_GetBandwidth(); // kbit/sec

	WIN_DLL bool STSC_SendData(const void *buffer,u32 size); // สามารถส่งได้ไม่เกิน 60K ถ้ามากกว่าจะซอยให้เองเท่าที่เหลืออยู่
	WIN_DLL void STSC_SendBigData(const void *buffer,u32 size); // ใช้สำหรับส่งdata ที่มากกว่า 60K เท่านั้น ถ้าไม่มากกว่าให้ส่งด้วย SendData
	WIN_DLL void STSC_SendFileBuffer(const char *fileName,const void *buffer,u32 size); // ส่ง buffer และ บันทึกเป็นชื่อไฟล์ ห้ามเกิน 32 ตัวอักษร
	//bool STSC_SendFile(const char *fileName); // อ่านไฟล์ และ ส่งไปให้

	// if false can't send File or Data Now;
	WIN_DLL bool STSC_CanSendFileOrData();

	WIN_DLL bool STSC_HaveData(); // chekc this first before call RecieveData
	WIN_DLL void STSC_GetData(int idx,void *buffer,u32 *dataSize); // copy to buffer (it will copy to buffer)
	WIN_DLL int  STSC_GetDataType(int idx);
	WIN_DLL int  STSC_GetDataCount();
	WIN_DLL u32  STSC_GetDataSize(int idx);

	WIN_DLL void STSC_StartGetData(); // Please Call berfore get data to block new comming data from thread
	WIN_DLL void STSC_EndGetData(); // please call after done get data;

	WIN_DLL bool STSC_HaveBigData();
	WIN_DLL u8*  STSC_GetBigDataBuffer();
	WIN_DLL int  STSC_GetBigDataSize();
	WIN_DLL char*STSC_GetBigDataFileName(); // NULL if only data
	WIN_DLL void STSC_GetBigDataDone();

	// Check Big Data Sending
	WIN_DLL bool STSC_HaveBigDataSending();
	WIN_DLL int  STSC_BigDataSendingSize();
	WIN_DLL int  STSC_BigDataSendSize();

	WIN_DLL bool STSC_HaveBigDataRecving();
	WIN_DLL int  STSC_BigDataRecvingSize();
	/*---------------------------------------------------------------------------*
	stream data (PATCH or Other big Data) function definitions
	*---------------------------------------------------------------------------*/
	WIN_DLL bool STSC_HaveStreamData();
	WIN_DLL char*STSC_GetStreamDataPointer();
	WIN_DLL u32  STSC_GetStreamDataSize();
	WIN_DLL void STSC_ReleaseStreamData(); // Will autoRelease if have new stream data

	WIN_DLL void STSC_SetBlockingData(bool block); // default None Block

	/*---------------------------------------------------------------------------*
	function config network
	*---------------------------------------------------------------------------*/
	WIN_DLL void STSC_SetTimeout(u32 second);
	WIN_DLL void STSC_SetReTrasmit(u32 microSec);
	WIN_DLL void STSC_SetFrame(u32 frame);
	WIN_DLL void STSC_ResetStatus(); // Internal Call only


	WIN_DLL void STSC_NextFrame(); // call when send every packet done engine will start send from here
}
#endif /* defined(__StarMacOS__StarClient__) */
