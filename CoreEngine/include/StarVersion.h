 //
//  StarVersion.h
//  StarEngine
//
//  Created by SAMRET WAJANASATHIAN on 9/6/2554.
//  Copyright 2554 SAMRET WAJANASATHIAN All rights reserved.
//
// 0.xx1 mean Debuging
// 0.xx2 mean Complete
// Summary for OpenGL 1.x
//  White can use with vertex
//  Black Color can't use with vertex if have Black Color will ignore vertex setting color. Alpha can use only

 #define  STAR_VERSION 2.87
// version 2.87 FIX
//   STNET_NTOIP   ipString not correect;
//   Add AS3 network (socket,chat,http)
//   Add AS3 Sound Engine
//   ADD Download file from Server
//      void STOS_SetDownloadServer(int ipValue,int port); // use for ios and android flash not have
//      void STOS_StartDownloadData();
//      void STOS_AddDownloadFile(const char* fileName);
//      void STOS_EndDownLoadData();

//      bool STOS_Downloading();  // กำลัง ดาวโหลดอยู่หรือเปล่า
//      bool STOS_CheckHaveFile(const char* fileName,size_t fileSize);

// version 2.86 FIX CHAT
// version 2.85  14/12/2557
//  FIX Chat and optimize ถ้าส่งไม่ได้ จะเก็บไว้ส่งทีหลัง
//   แก้ไขรูปแบบการบันทึกด้วย
// version 2.84  16/11/2557
//  Add STRE_ReadFilePos();
//  Add STRE_AppendFile();
//      STRE_UpdateFile();

// version 2.83  05/11/2557
//   Change Recieve Protocal
//   Add Client Version to check error
// version 2.82  03/11/2557
//   Add iPhone 6 and 6 plus
//   แก้ไขการทำงานตอนรับข้อมูลให้มี buffer ที่รับข้อมูลมา ถ้าเกิดการับแบบหลาย ๆ package มา
//   แก้ไขโดยมีการรวม confirm package มาด้วนใน data package
// version 2.81  29/10/2557
//   Fix WindowVersion Post from Blocking to Non blocking Mode
//
// version 2.80  25/10/2557
//   Optimize use bynary serch for pack file
//   Fix bug for Network Reset
// version 2.79  16/10/2557
//   Add TextView Class
//   Fix can use alpha with animation
// version 2.78  13/10/2557
//   ADD StarWindows Version
//   Fix bug ไม่สามารถเล่น effect ได้พร้อม ๆ กันหลาย channel บน ios และ mac app
//   Fix bug ไม่ Error ถ้าหาไฟล์เสียงไม่พบใน Debug Mode
// version 2.77
//   Star2dSprite เริ่มต้น จองหน่วยความจำให้ 1 ตัว
// version 2.76
//  เพิ่มกดปุ่ม Enter ใน keyboard
//  เพิ่มแสดงผล resource ที่โหลดทั้งหมด เพื่อคำนวนง่าย
//  เพิ่มเติมการกดปุ่ม Tap ใน Keyboard
//  bool STIN_haveTAPTrick();
//   optimize speed loadasync 4X for texture
// version 2.76
//  Fix Particle Lock Resource Not Work
//  Add assert ถ้าวาด แล้วไม่ได้ set sprite ไว้
// version 2.74
//   Fix Bug StarClient Can't reconnect
// Version 2.73
//  Fix BUG Android
//  Load From PackFile Android Error STRE_LoadTextureFrom3ndTx,STRE_LoadLayoutFrom3ndTx
//  Modify Flash Version for Unicode Text box
//  fix Memory
//   - Use pthread_mutex remove while loop
//  - fix free algorthym not auto fragment memory for speed free memory
//  - Add STOS_FragmentMemory();
// Add
//    Star2DSprite::SetTextBoxUNICODE
//    Star
// version 2.72
//  Fix iOS Key auto display Done;
//  Increate MEMORY Table from 64K to 128K
// version 2.71
//  Fix   Network bug
//  Fix Particle Bug Postion ที่กลอกเข้ามาโดน reset 
//  Add void STSC_StartGetData(); to lock resource
//  Add STRE_CheckFile(const char *fileName);
//  modify Particle สามารถ unlock texture ได้ ปกติ จะ lock ไว้ในหน่วยความจำ
// version 2.70
//  Fix Bug Release Lock Texture
//  Add CGPoint* STIN_getScrollValue();
//  Fix externalPath some are 256 change it to 512
//  Optimize ยุบรวมฟังชั่น LoadASYNC และ Load ปกติเข้าด้วยกัน
//  ปรับระบบ การ release ให้รองรับกับ Load แบบ ASYNC
//  เพิ่มฟังชั้น bool    STRE_RemoveResourceIdx(s32 idx);
//  return false ถ้า setspritewithpanename ไม่ได้
//  เพิ่ม
//   void GetUseTextureName(u32 index,char **outTexture,u32  *useCount);  mac app เท่านั้น
// version 2.69
//  Fix SARA in Thai Language
//  Fix HTTP POST Connect in non blocking mode and set back to blocking mode
//  Remove all system auto call to extern function
//  Change StartEngineInit have on off physic
// version 2.68
// Add Star2DSprite
//      void    SetTextFiledWithKeyboard(u32 index,bool scoreDisplay = false); // ADD for auto for keyboard
//      char*   GetTextFiled(u32 index);
//   // Remove While loop wait to pthread_mutex
//  Add Keyobard Function on STIN Section Only iOS and MAC
//  Add STIN_Init() call at engine start
//  Add Keyboard Notification on iOS EAGLView
//  FIX iOS Support Lanscalpe Left (old only right)
// version 2.67
//   FIX BUG SetText can't copy morethen MAX_TEXT_BOX * 3 size
//
// version 2.66   03/06/2557
//   FIX Mac Version Touch Start Point Error
// version 2.65   23/05/2557
//  FIX BUG
//   Star2DSprite not release sprite Control
//   STSC Send more then 512 Error
// version 2.64   22/05/2557
//  Fix Effect Angle not correct

// version 2.63   20/05/2557
//  Change Default to 30 FPS
//  Fix Bug
//    StarClient ตอน Connection ต้องไม่แสดงสถานะอื่น ๆ
//  Add
//   เพื่อ http post เพื่อส่ง php ใน StarNet.h StarNet.cpp
//  s32 STNET_InitHTTP(); // Call this function first if you want to use HTTP Connect
//  HTTP_STATUS STNET_HTTP_Get(const char *host,const char *page); // return success;
//  HTTP_STATUS STNET_HTTP_Post(const char *host,const char *page,const char *post);
//  bool    STNET_HTTP_HaveResponse();
//  int     STNET_HTTP_GetErrorCode();
//  char*   STNET_HTTP_GetErrorString();
//  HTTP_STATUS STNET_HTTPGetSendingStatus();
//  HTTP_STATUS STNET_HTTPGetRecieveStatus();
//  char* STNET_HTTP_GetRespondBuffer();
//  HTTP_STATUS STNET_HTTP_PostImage(const char *host,const char *page,const char *post,const char* imageName,const char* fileName,const char*buffer);

// version 2.62   09/05/2557
//   แก้ไขให้ การอ่าน layou เป็น Dynamic โดยไม่ขึ้นกับ ขนาด เพื่อ optimiz memory
//   optimize childIdx ที่ nLayout.h จาก fix 128 เป็นตามขนาด childCount
//   Optimize Star2DSprite ให้เริ่มต้นที่ 16 และ Auto incress ทุก 2 เท่าโดยอัตโนมัติ
//      - void increateSpriteControl();
//      - เพื่อ assert ทุก ฟังชั่น
//      - เมื่อ release จะลบ texture และ font ออกด้วย ถ้าไม่ได้ lock ไว้
//   Fix bug nLayout
//      - Set null ตอน init เอาออก เพราะจะทำให้โดน null ถ้ามีการเรียกอีกที และไม่เอาหน่วยความจำออก
//      - ปรับ childIdx จาก u32 เป็น u16
//   Fix Memory
//      - ถ้าจองหน่วยความจำไม่ได้ ให้หยุดทำงานทันที
// version 2.61.1 24/04/2557
//   เพิ่ม  -(GLuint) LoadTextureFromNSImage:(NSImage *) aNSImage :(bool) lock; เฉพาะ MAC version
// version 2.61 22/04/2557
//   แก้ไขให้รองรับการเชื่อมต่อที่ต่ำโครต ๆ
//   แก้ไขให้ส่งค่าย้อนกลับมาว่าส่งได้หรือไม่ได้ ด้วย เพราะถ้าของเก่ายังส่งไม่เสร็จ ก็จะส่งต่อไม่ได้
// version 2.60.0 ยังไม่ได้เอาไปรวมกับ 2.59.3
//  Fix bug
//     STSC_SendData ถ้าส่งมากกว่า 1024 แล้วไม่ได้ขยับ pointer
//     Fix Connection Error And Crash bug
//  Fix SetText ให้สามารถแสดงผลกับ เครื่องหมายคำพูดเปิดปิดภาษาไทยได้ 
//  Add
//     void Star2DSprite::SetTexturePos(u32 idx,f32 x1,f32 y1,f32 width,f32 height);
//     void nLayout::SetTexturePos(u32 idx,f32 x1,f32 y1,f32 width,f32 height);

//     STSC can detect 2 mode alway  or one time and keep section. It depend on server type
//     u32   STSC_GetConnectionType();
// version 2.59.3
//  Add
//   void SetAlpha(u32 index,f32 fAlpha); on Star2DSprite.h
// version 2.59.2
//   Fix
//     STSC_Connect CRASH when not have server
// version 2.59.1
//   Add
//     bool STSC_HaveBigDataRecving();
//     int  STSC_BigDataRecvingSize();
//   Fix bug
//     STRE_LoadTextureFromSaveData(const char *fileName) พอโหลดได้แล้วมันเด้งออก เนื่องจากงาง  return ผิดจุด
//     STSC_GetBigDataSize() Error return ค่าออกมาผิด ไม่ได้ ส่งค่า file ออกมาแต่ส่งค่าขนาด Buffer ออกมาแทน
// version 2.59
//  Adjust Network to 5 frame per sec for normal but if have big data will ignore it will send ASAP
//   Fix Load Effect
//    แก้ไข FileName จาก 128 เป็น 1024 buffer ไม่พอที่จะใส่ชื่อไฟล์
// version 2.58 20/02/2014  15:02
//    Add GLuint  STRE_LoadTextureFromSaveData(const char *fileName); ใช้เพื่อโหลดภาพจากเซพที่มีอยู่
//  Adjust StarClient and modify to support big packet data
//    void STSC_SendData(const void *buffer,u32 size);
//    void STSC_SendPacket(const void *buffer,u32 size); // limite max STAR_CLIENT_DEFAULT_BUFFER
//    void STSC_SendFile(const void *buffer,u32 size);
//    int  STSC_GetDataType(int idx);
//
// version 2.57.2 30/01/2014
//  - Fix bug DrawPane(int idx) ตำแหน่งไม่เปลี่ยนถ้าเรียนกคำสั่ง setposition
//  - Add
//       void SetDisableClickID(s32 playListID,bool disable); at Star2DProject
// version 2.57.1 04/01/2014
//  - Add STRE_GetTextureName(u32 idx);
//  Add Star2DProject
//        f32  GetLayoutX(u32 playListID);
//        f32  GetLayoutY(u32 playListID);
//  - Fix IsActiveBug on Star2DPhysic
// version 2.57 24/12/2013
//   - Add IOS version initWithGLKView
// version 2.56 19/12/2013
//   - Fix All return input will 0,0 at center
//   - Fix all Check in bound and all drawpane to 0,0 at center
//   - All Draw Pane will convert -posY to posY
//   - Fix Chack in Bound when scale and position

// version 2.55 18/12/2013
//   - Fix bug WindowPane Texture not correct and center not correct

// version 2.54 17/12/2013
//   - Fix ios Can't play Stream Music
//   - add STRE_SetSavePath(path);
// version 2.52 09/12/2013
//   
//   - เพิ่มฟังชั่นในการช่วย link ไป Particle ที่ Star2DSprite
//   -  แก้ไข nLayout เพื่อเอาค่า Matrix ส่งมาให้  ที่ particle
//       s32     GetPaneId(const char * paneName);
//       SetLinkParticleWithPaneID(u32 index,StarParticleEffect* pt,u32 paneID)

//   - Fix Texture Error Flip on Y Position
//   - Add reset() at StarParticleEffect
//   - Fix Particle Not display if not loop particle
// version 2.50.1  04/12/2013
//   - FIX BUG STRE_ReadFile
//   - FIX BUG STRE_WriteFile

// version 2.50.0  27/11/2013
//    - Add nLayout
//       void SetBlendFunc(s32 source,s32 dest);
//   - Add Star2DSprite
//       void RemovePane(u32 index);
//       void SetBlendFunc(u32 index,s32 source,s32 dest);
//   - Add Star2DPorject.cpp
//   - Fix support 64bit data
//   - Fix Star2DPhysic IsActive will return false if animation stop too
//  Add
//   void STOS_ShowDebugInfo(bool show);
// version 2.49.0  24/11/2013
//   - Make Compotible with iOS Android Mac and Flash
//   - Change effect free channel from 16 to 32
//   - change color from f32 to u8
// version 2.48.0  13/11/2013
//   - Change version to 13 Digit
//   - version variable will in StarSystem.cpp
//   - add StarClient.h,StarClient.cpp to make new StarSocket Protocal for TCP/UDP protocal
//   - This version want to use with StarServer 2.0.0 UP
// version 2.47 24/10/2013
//   - Add StarNet for Network Function
//   - fix DrawBound (nativeLayout) to support Global Scale
//   - Add  CRC define on StarMath.h
//   - Add  CRC8,16,32 Function
//      u8      STOS_CRC8(const void    *data,u32 dataLength);
//      u16     STOS_CRC16(const void   *data,u16 dataLength);
//      u32     STOS_CRC32(const void   *data,u32 dataLength);

// version 2.46 26/09/2013
//   = chnage STOS_GetPhysicUseMem() from char* to u32
// version 2.45 5/09/13
//   - Fix bug nLayout WindowPane
//   - Fix StarThread Load RGB Error
//   - Add StarInput Function
//   Add
//     Star2DSprite::DrawBound(u32 index)
//     nativeLayout::DrawBound(u32 paneID,f32 x,f32 y,f32 z,f32 rot);
//   - Remove AllInput from StarEAGView on iOS
//   - Fix STOS_GetTickCount();
//   - Fix issue debug string not show
//   - Fix issue load font error

// version 2.44 BIG CHANGE
//   - Big Change to 3 platform with C++ and Hide Structure in source code
//   - Optimize to use Own Memory management 
//  StarMEM.cpp -- STOS_Alloc to alignment 8
//  Change Object to Class
//   - iOSType.h    -> StarType.h
//   - iOSLayout.m  -> nLayout.cpp
//   - iOSAnimation.m -> nAnimation.cpp
//   - iOSResource   -> nResource.
//  Modify StarEAGLView.m to .mm
//
//
//
//
//
//
//  Add nResourec
//    void    SetResourcePath(const char *path);
//    void    SetTexturePath(const char *path);
//
// version 2.43
// Add StarParticleEffect
// version 2.42
//19/08/2013
// add
// Star2DSprint
//-(void) DrawPane:(u32) index :(f32) posX :(f32) posY :(f32) posZ scaleX2:(f32) sX scaleY2:(f32) sY;
// iOSLayout
//-(void) DrawNative:(u32) paneIdx x:(f32) x y:(f32)y z:(f32)z scaleX2:(f32) scaleX scaleY2:(f32) scaleY (ย้าย glScalef มาหลัง glTranslatef)
//-(void) DrawPaneID:(u32)paneID :(f32) x :(f32) y :(f32) z scaleX2:(f32) sX scaleY2:(f32) sY;
//10/06/2013
//   Add Support RGBA8888 (pv8)
// version 2.41 08/06/2013
//  Add Cache Loading to star Packet. Default will use cache if possible
// version 2.40 08/06/2013
//  Change
//    void*    LoadTextureFrom3ndTx last parameter from bool to u32 for compress type
//   Add
//    u32  GetTextureFormat(const char *fileName); at iOSResource
//   Add Support RGBA4444
//   Remove IsPNGFile
//
// version 2.39 05/06/2013
//  Fix bug not close file when open pv3 file
//  Fix not Scale in bound Add Scale in Sprite Struct
// version 2.38 29/06/2013
//  Modify
//      - LoadResourceFileName:(NSString *)fileName :(BOOL) lock
//      - SetupOpenGLTexture
//      - Thread from 256 to 1024
//      - LoadTextureFrom3ndTx(const char *fileName,size_t* fileSize,u16 *widht,u16 *height,bool *compress)
//      - bool CompareTexture(const char *first,const char *second)
//  Add
//      - void    SetUSEGL_ONE_ONLY(bool use); // default false;
//      - bool    GetUSEGL_ONE_ONLY();
//  Fix at StarThread
//   SetupOpenGLTexture,LoadResourceFileName
//  Fix at iosResource
//   modify LoadTextureFrom3ndTx to return imgWidht imgHeight and Compress
//   Support Texture and open ly pack version 2 with compress texture
//   Add support PVTR Texture and output GLTexture Error if have error

// version 2.37 28/06/2013
//   Fix Bug -(void) DrawPicturePane:(u32) paneIdx
//       add will return if textureIdx == -1
// version 2.36 27/06/2013
//   Fix can't use layout on Lanscape

// Version 2.35 21/06/2013
//   Ignore not found texture file will have _ at index 0 and index 1 only load from 3ndly file
//  iOSLayout
//    LoadTextureData;
//    LoadLayout
// version 2.34 17/06/2013
//  Add StarThread
//    +(void) Load3ndLyFile:(NSString *) fileName;
//  Add iOSResource
//   bool    Have3ndLyFile(); // Return true if have
//   void    Load3ndLyFile(const char *fileName);
//   void*   LoadLayoutFrom3ndTx(const char *fileName,size_t* fileSize); // return buff and fileSize;


// version 2.33 14/06/2013
//  Fix StarThread.m
//    SetupOpenGLTexture() Binding texture

//  Add 3ndTextureFormat use with TextTools Program
//  bool    Have3ndTXFile(); // Return true if have
//  void    Load3ndTxFile(const char *fileName);
//  void    LoadTextureFrom3ndTx(const char *fileName,void* outBuff,size_t* fileSize); // return buff and fileSize;
//  void    LoadTextureFromIdx(int idx,void* outBuff,size_t* fileSize);
//  void    Release3ndTxFile(const char *fileName);
//  void    ReleaesAll3ndTxFile();
//  Add StarThread
//    +(void) Load3ndTxFile:(NSString *) fileName;
// version 2.32 06/06/2013
//  Add StarMEM.h
//   Fix bug  LooadTextureData Faster
//   StarThread
//    +(u32)  GetLoadPercent;
//  Show Percent when Loading

// version 2.31 05/06/2013
// Add StarNet
//  +(u32)  STNET_IPTON:(const char*) ipAddr;
//  +(char *) STNET_NTOIP:(u32) ipValue;
// version 2.30 23/05/2013
// Optimize
//   DrawChild  -> iOSLayout
//
// Remove
//   -(void) DrawDebugString:(u32) x :(u32) y :(const char *)text;
//
// Add iOSAnimation
//  -(u32) GetTranSCount:(s32) idx;
//  -(u32) GetTranTCount:(s32) idx;
//  -(u32) GetTextPatternCount(s32) idx;
//
// Add StarEAGLView
//   -(void) LoadSystemFont;
//   -(void) DrawDebugString:(u32) x :(u32) y :(const char *)text;
//   -(void) DrawDebugChar:(f32) posX :(f32)posY :(struct FontTxTs *) pFontTx
// Fix bug iOSLayout
//  AnimateTextPattern;
//  AnimateTextSRT
// ReleaseAllResource Bug remove lock Font

// version 2.29 22/05/2013
// เพิ่ม
//  
//  +(void) LoadFacebookImageASYNC:(NSString **) fbID :(GLuint*) txIdArray :(BOOL*) loadFinishArray :(u32) friendSize;
//  +(bool) IsFacebookLoading;
//  +(void) ProcessFacebook; // ทำงานที่ StarEAGL swapBuffer
//  +(void) LoadTextureFromUIImage:(UIImage *) aUIImage;

// version 2.28 21/05/2013
// ย้าย จาก iOSLayout
//  -(id) initLayoutFileNameASYNC:(NSString *) fileName :(bool) lock;
//  -(BOOL) LoadResourceFileName:(NSString *)fileName;
//  -(u32) LoadFontFileName:(NSString *) fontFile;
//  -(void) LoadSystemFont;
//
// ไปที่ StarThread และเปลี่ยนเป็น + ให้หมด
//  +(BOOL) LoadResourceFileName:(NSString *)fileName;
//  +(u32) LoadFontFileName:(NSString *) fontFile;
//  +(void) LoadSystemFont;
//  เพิ่ม
// iOSLayout.h
//  -(id) initLayoutFileNameASYNC:(NSString *) fileName :(bool) lock;
//  -(void) EncodeLayout;
//  -(void) LoadTextureData;

// StarLayout.h
//  -(void) SetupLayout;
//  -(void) LoadLayoutASYNC:(const char *)fileName:(BOOL) lock;
//  -(BOOL) LoadTextureASYNC:(BOOL) lock;
//  -(void) EncodeLayoutASYNC;
//  -(void) LoadAnimationASYNC:(const char*)fileName;
// StarThread.h StarThread.m
//  +(void) StartLoadFileASYNC;
//  +(void) EndLoadFileASYNC;
//  +(void) ProcessThread;
//  +(bool) IsLoading;
// Don't Call
//  +(bool) IsASYNCMode;
//  +(void) AddLoadLayout:(StarLayout*) layout:(const char*) fileName:(bool) lock;
//  StarEAGLView
//  loadingSprite
//  swapBuffers
//  แก้ไข
//  StarLayout
//  -(void) LoadLayout:(const char *)fileName :(bool)lock // Load ASYNC

// version 2.27 20/05/2013
// iOSLayout.m
// แก้ไข
// -(u32) LoadFontFileName:(NSString *) fontFile
// -(void) LoadSystemFont
// -(BOOL) LoadResourceFileName:(NSString *)fileName
// -(void) EncodeSTData2iOSData
// -(void) DrawChar:(u32) paneIdx  startX:(f32) posX startY:(f32) posY FontTx:(struct FontTxTs *) pFontTx FontHeight:(u16) realHeight
// -(void) DrawString:(f32) posX:(f32) posY:(u32) paneIdx string:(u16 *) text
// -(void) DrawDebugChar:(f32) posX :(f32)posY :(struct FontTxTs *) pFontTx
// -(void) DrawDebugString:(u32) x :(u32) y:(const char *)text
// -(void) DrawBound:(u32)idx:(f32) x1:(f32) y1:(f32) x2:(f32) y2:(f32) red:(f32) green:(f32) blue
// -(void) DrawChild:(u32) paneIdx
// iOSLayout.h
// struct iOSLayoutStruct
// เพิ่ม
// GLint             m_textureFormat;

// iOSResource.cpp
// static GLint           s_glTextureFormat[MAX_IOSRESOURCE_FILE];
// int     LoadResource(const char* fileName,GLuint glID,UInt32 imgX,UInt32 imgY,UInt32 txX,UInt32 txY,u32 size,GLint txFormat, bool lock)
// GLint   GetTextureFormat(int idx)


// version 2.26 15/05/2013
// iOSLayout.m
// เพิ่ม
// -(void) CalculateChildMatrix:(u32) paneIdx
// -(void) CalculateMatrix:(u32) paneIdx:(f32) posX:(f32) posY:(f32) posZ;


// version 2.25 02/05/2013
//  Fix Check Bound Error from convert int to f32 on initOpenGLES StarEGAL

// Shoot Fix 2013_05_06
// iOSLayout.m
// เพิ่ม
// วาด สามารถ Scale ได้
// -(void) DrawNative:(u32) paneIdx x:(f32) x y:(f32)y z:(f32)z scaleX:(f32) scaleX scaleY:(f32) scaleY
// -(void) DrawPaneID:(u32)paneID :(f32) x:(f32) y:(f32) z :(f32) sX :(f32) sY

// iOSResource.cpp
// เพิ่ม
// void    SetRealLayoutSize(Float32 maxX,Float32 maxY);
// Float32 getMaxLayoutX();
// Float32 getMaxLayoutY();
// Float32 getRealLayoutX();
// Float32 getRealLayoutY();

// StarEAGLView.m
// เพิ่ม
// - (void) setRealLayoutSize:(Float32) sizeX :(Float32) sizeY
// แก้
// - (void) swapBuffers // Fix Fade
// - (void) drawLineStart:(CGPoint)start End:(CGPoint) end
// - (void) drawRectStart:(CGPoint)start End:(CGPoint) end
// - (void) drawTriangleP1:(CGPoint)p1 P2:(CGPoint) p2 P3:(CGPoint) p3
// - (void) drawCircle:(CGPoint) center:(Float32) radius
// - (void) fillTriangleP1:(CGPoint)p1 P2:(CGPoint) p2 P3:(CGPoint) p3
// - (void) fillRectStart:(CGPoint)start End:(CGPoint) end
// - (void) fillRectLeftStart:(CGPoint)start End:(CGPoint) end

// StarOGL2D.m
// แก้
// -(void) setEffect:(NSUInteger) aEffect
// -(void) setPosition:(CGPoint)point Shift:(BOOL) shift
// -(void) setFrame:(NSInteger) frame
// -(void) setScalX:(GLfloat) aScx ScalY:(GLfloat) aScy
// -(float) GetFrameWidth  // เอา if retina ออก
// -(float) GetFrameHeight // เอา if retina ออก

// Star2DSprite.m
// แก้
// -(f32)	GetDisplayX:(u32) index
// -(f32)	GetDisplayY:(u32) index

// version 2.24 29/04/2013
//  Change
//    Layout and glOthoganal from 0 to 1024 to -512 to 512 it will effect for old position but will fix only layout first
//    Change -(void) initLayout on initOpenGLESRender,CreateSurface;
//           -DrawNative X:Y:Z on iOSLayout
//           -DrawChild for invert Y Pos
//         All rotage from -rotage to possitive sign
//           - LoadResourceFileName convert Premultiply alpha to Alpha
//  Fix CheckInBound at iOSLayout only it will effect GetInBound
//       LoadFontFileName for width ratio
//       DrawString
//       DrawChar

// version 2.23 29/04/2013
// Fix Touch Function on StarEAGLView
// Fix -(void) EncodeSTData2iOSData on iOSLayout 
// Fix -(GLuint) LoadTextureFromUIImage:(UIImage *) aUIImage:(bool) lock on StarEAGLView
// - เอา [aUIImage release]; ออก

// version 2.22 27/04/2013
//  Fix Bug
//   -initAnimationFileName:(NSString *)fileName on iOSAnimation
//   -LoadLayout on StarLayout
//   -All DrawRoot and DrawPane on StarLayout
//   - TextPan can use white balance with TOP and BOTTOM (black not have effect now)
//   - Change Pixel format from 565 to RGB8 at init2D on StarEAGLView
//  Modify
//   - EncodeSTData2iOSData on iOSLayout
//
//  Add
//  - DrawBound at iOSLayout
//  - RemoveResourceGUID(GLuint glID); on iOSResource


// version 2.21 19/04/2013
//  Fix
//    Deallocate BUG on iOSAnimation at ClearAnimation
//    Add
//       -(GLuint) LoadTextureFromUIImage:(UIimage *) aUIImage:(bool) lock
// version 2.20 12/04/2013
//  Add
//      -(id) init2DLayout:(UInt32) width:(UInt32) height; for Layout Size
//     -(void) SetViewport:(int) x:(int) y:(int) widht:(int) height
//  Fix 
//      DrawTextBox
//      Workwith export 15 Font Version 3
//  Add 
//    Loading individule texture to match with layout ตั้งชื่อ texture 4 ตัวแรกว่า TEMP ตัวใหญ่หมด
//    - (GLuint) LoadTexture:(NSString *) fileName; (StarEAGView)
//   -(void) SetTextureID:(GLuint) textID; StarLayout.h,iOSLayout.h
//  Add
//   Locking Resource
//   iOSResource.h
//      - s_bTextureLock; for locking resource
//      - s_nResourceSize[MAX_IOSRESOURCE_FILE];
//      - Memory Texture Use
//      - LoadResource(const char* fileName,GLuint glID,UInt32 imgX,UInt32 imgY,UInt32 txX,UInt32 txY,u32 size); 
//      - GetMaxMemory();
//
//   StarLayout.h
//      - (void) LoadLayout:(const char *) fileName :(bool) lock;
//   iOSLayout.h
//      - (id)  initLayoutFileName:(NSString *) fileName :(bool) lock;
//
//

// verison 2.19 11/04/2013
//  Add
//    Star2DSprite
//      -(char*)    GetComment:(u32) index;
//      -(void)     SetTextBox:(u32) index:(char *) text;
//    iOSLayout
//      -(char *) GetComment:(u32) panID;
//      -(void) SetText:(char *) text :(u32) paneID;

// version 2.18 07/04/2013
//  Add
//    -(void) pauseGame;
//    -(void) resumeGame;

// version 2.17 02/04/2013
//  Add
//      f32     GetZoomValue();
//      void    SetZoomValue(f32 zoom);

// Version 2.16 01/04/2013
//  Fix
//     White And Black Balance not /255.0f

// version 2.15 04/03/2013 - 22/03/2013 13:22
//  Add
//      StarFont* GetResourceFont(u32 textureId);
//      StarFont* GetFreeResourceFont(u32 *idx);
//      bool RemoveResourceFont(u32 idx);
//    iOSLayout
//      -(void) LoadSystemFont;
//      -(void) DrawDebugString:(u32) x :(u32) y:(const char *)text;

//      Textbox Render
//      Font Resource
//      -(void) DrawString:(u32) paneIdx string:(u16 *) text
//      -(void) DrawChild:(u32) paneIdx
//      -(u32) LoadFontFileName:(NSString *) fontFile
//  Modify
//      -(void) DrawChild  for textbox
//      -EncodeSTData2iOSData  for textbox
//      -LoadLayout  LoadFont
//      STTextBoxList  from char to u16

// version 2.14 12/02/2013 16:21
// Use with export file version 13
//  Optimize Texture file reader

// version 2.13 28/01/2013
//  Fix
//       Alpha value Make crash bug
// version 2.12 23/01/2013
//  Add
//      - Float32 GetTextureSizeX(int idx);
//      - Float32 GetTextureSizeY(int idx);

//   Fix Bug
//      - LoadAnimation Crash Bug at Load BlackB_Count
//      - DrawRoot Error
// version 2.11 18/1/2013
//   Fix Bug
//      - ClearAnimationData Not Free vertex and Material color
//      - AnimateMaterialColor

// version 2.10 4/1/2013 - 5/1/2013
//   Use with Export 10
//   Add 
//      +Text Box Pane
//   Change
//      + MAX Child from 32 to 64
//   Fix
//      + Have Bounding Pane before Picture position wrong


// version 2.09 10/11/2012  - 3/01/2012
//  Remove -(void) Star2DSprite::DrawPane:(u32) index order:(u32) order;
//  change iOSLayoutStruct posX,posY,posZ to layoutAdjX,layoutAdjY,layoutAdjZ it good to understand variable
//  Fix Rot from Parent Error
//  Change algorythm for calculate metrix. Now use Push and Pop from open gl
//  Add More NSAssert


// version 2.08 07/10/2012 20:33
//  Fix 
//      Black and White color error not tranparent
//      Support Export Version 0.9
// version 2.07 01/10/2012 09:40
//  Add White and Black Metarial  Can use only Black or white can't use together at this time will modify later
//  Supprot Version 8

// Version 2.06 29/09/2012 20:52
//  This version support export layout vrsion 7
//  Add VertextRGB animation Animation
// version 2.05 27/09/2012 20:43
//   Fix EXEC BAD ASSECC Make Game Crash come from memory allocate not collect use sizeX but load sizeY
//      
// version 2.04 02/09/2012 21:10
//   Fix Alpha Problem with glBlendFunc at DrawChild

// version 2.03 16/08/2012 11:25
//  Change DrawChild from
//      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); to
//      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
// version 2.02 05/07/2012 18:01
//  Add 
//        to support export version 0.6  TexturePattern
//  Change
//         Max TEXTURE from 32 to 128
//  version 2.01 30/01/2012
//  Fix rotate
// Skip to 2.00  29/01/2012
//  add method
//
//   GetRotFromParentZ: // internal at iOSLayout.m can get only 1 level
//


// Revision 1.17
// Fix Touch Move Touch Start

// Revision 1.16
// Fix initWithImage

// Revision 1.15 
// Fix Touch

// Revision 1.14 
// Add Type STAR_TYPE_RETINATOIPAD

// Revision 1.13
// Add GetBoundInBoundSprite in Star2DSprite 

// Revision 1.12
// mini FixBug Star2DSprite

// Revision 1.11
// Add STAR_TYPE_IPADWii_LAYOUT Convert wii(640x320) to iPad.

// Revision 1.10
// Fix Layout View in   480x320

// Revision 1.09
// Add STAR_TYPE_UNIWii_LAYOUT

// Revision 1.08
// Change support Layout and Animation to version 5
// - Support Start position Left,Right,Up,Down
// - Support Animation Texture with Smooth
// - Scale from Parent
// revision 1.07
// Add -(BOOL) GetBoundInBound:(u32) index1:(u32) index2

// revision 1.06
//- Star2DSprite
// Add DrawPane:(u32) index :(f32) posX :(f32) posY :(f32) posZ rot:(f32) rot.
//-iOSLayout
// Add DrawPaneID:(UInt32) paneID :(f32) x:(f32) y:(f32) z :(f32) rot.

// revision 1.05
// Fix StarLayout Invert draw Y Position.

// revision 1.04
// Fix Button In Ad Pos

// revision 1.03
// Add STAR_TYPE_UNIVERSAL_LAYOUT
// Fix tranparen animation not work from export
// Change export version to 4
// Add VertexAnimation for iOSAnimation.h
// Fix Get In Bound

// revision 1.01
// Fix iOSLayout Animation

// revision 1.00
// Complete with first version with Animation Software.
// Worl with convertsion version 0.03 and file version 3

// Add      - iOSAnimation.h
// Add      - StarLayout.h
// Add      - iOSLayout.h
// Add      - iOSResource.h
// Add      -(BOOL) LoadResourceFileName:(NSString *)fileName;
// Add      -(BOOL) ReleaseResourceFileName:(NSString*) fileName;

// revision 0.59
// Fix      Ad For iPad.

// revision 0.58
// Add      - (void) gotoURL:(NSString *) url;
// Ad       Ad FuncTion
 
// revision 0.57
// Fix      - Add STAR_TYPE_IPADC โหมดย่อ iPad เป็น iPod

// revision 0.56
// Fix      - Fix Retina

// revision 0.55
// Fix      - Touch in iPadS Type.
// Fix      - Draw Left in iPadS Type.

// revision 0.54
// Add      - STAR_TYPE_IPADS โหมด เอา Retina มาวาดตรงกลาง
// Add      - (void) drawAtLeftPoint:(CGPoint) point WithFrame:(NSInteger) frame; วาดโดยคิดคำณวนจากซ้ายบน
// Add      - (void) fillRectLeftStart:(CGPoint)start End:(CGPoint) end;

// revision 0.53
// Add      - x2 Position if Retina.
// Add      - (NSString *)getDeviceSystem;
// Add      - (BOOL) getRetinaDevice;

// revision 0.52
// Fix      -(void) ChangeDisplayLandscape
// Add      - (void) reloadTextureImageString:(NSString *)name1 Path:(NSString *)name2 ...

// revision 0.51
// Fix      - BUG STAR_TYPE_IPHONE_RETINA and STAR_TYPE_IPHONE 
// Add      - style on setGradientFillColor
// Add      - STAR_GRADIANT_LEFTRIGHT
//          - STAR_GRADIANT_UPDOWN 
// revision 0.41
// Add      -(BOOL) haveTouchMove;
// Add      -(NSUInteger) touchStartCount .... for return touch start count ส่งจำนวนของปุ่มที่เริ่มสำผัสสมมุติกดอยู่ แล้ว1ปุ่ม กดเพิ่มเข้ามาอีก3ปุ่ม returnค่า 3 กลับมา
// Add -(CGPoint) touchAllPoint:(NSUInteger) index; .... to get all touch point [non-move/move] //ส่งค่าตำแหน่งปุ่ม ไม่ว่าจะขยับหรือไม่

// revision 0.031
// Add      -(BOOL) haveTouchEnd; ถ้ามีการยกปุ่มกดออก
// Add      -(CGPoint) touchEndPoint:(NSUInteger) index; ส่งตำแหน่งที่ยกปุ่มออก
// Add      -(NSUInteger) touchEndCount; ส่งค่าจำนวนปุ่มที่ยกออก

// Fix      -CreateSurface() not finish on this version
// Change   -(id) init2DView -> -(id) init2DViewDeviceType:(UInt32) aDeviceType; 
// Remove   -(void) ChangeDisplayLandscape;
// Remove   -(void) ChangeDisplayPortrait;

// revision 0.021
// change 
//   Runable to CADisplayLink 
// Add
//   Retina Display Support

// Version 0.01 
// Start Star Engine version

