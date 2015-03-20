//
//  StarNet.m
//  StarEngine
//
//  Created by SAMRET WAJANASATHIAN on 5/6/2556.
//
//



#include "StarNet.h"
#include "string.h"
#include "StarMEM.h"
#include "errno.h"
#include "stdio.h"
#include <sys/types.h>

#ifdef STAR_WINDOWS_VERSION
// #include <WS2def.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>
#endif
static char *httpBuffer;
static char httpErrorString[64];
volatile int  httpErrorCode;
//static char httpVersion[8];
static bool s_bHttpInit = false;
volatile bool s_bHaveResponse;
#ifdef STAR_WINDOWS_VERSION
HANDLE		httpThreadID;
DWORD		httpThreadCreateID;
#else
pthread_t     httpThreadID;
#endif
struct POST_STRUCT{
    int sockfd;
    char host[64];
    char page[64];
    char postSTR[1024];
    bool thread; // for join
    char *buffer;
    int  buffSize;
    bool sendImage;
    const char *imageBuffer;
    int  imageSize;
    char imageDataName[16];
    char imageDataFileName[16];
    
};

struct POST_STRUCT postDes;
#ifdef STAR_WINDOWS_VERSION
DWORD WINAPI PostThreadFunc(LPVOID paramPtr);
#else
void *PostThreadFunc(void *paramPtr);
#endif

#ifdef STAR_WINDOWS_VERSION
DWORD WINAPI PostThreadFunc(LPVOID paramPtr){
#else
    void *PostThreadFunc(void *paramPtr){
#endif
        struct POST_STRUCT *postData = (struct POST_STRUCT*)paramPtr;
        char *sendLine;
        
        if(postData->sendImage == false) {
            sendLine = (char*)STOS_Alloc(1024+1);
#ifdef STAR_WINDOWS_VERSION
            sprintf(sendLine,
                    "POST %s HTTP/1.0\r\n"
                    "Host: %s\r\n"
                    "Content-type: application/x-www-form-urlencoded\r\n"
                    "Content-length: %d\r\n\r\n"
                    "%s\r\n", postData->page,postData->host, strlen(postData->postSTR), postData->postSTR);
            
            send(postData->sockfd, sendLine, strlen(sendLine),0);
#else
            snprintf(sendLine, 1024,
                     "POST %s HTTP/1.1\r\n"
                     "Host: %s\r\n"
                     "Content-type: application/x-www-form-urlencoded\r\n"
                     "Content-length: %d\r\n\r\n"
                     "%s\r\n", postData->page,postData->host, (int)strlen(postData->postSTR), postData->postSTR);
            
            write(postData->sockfd, sendLine, strlen(sendLine));
#endif
        }
        else {
            int sendSize;
            int headerSize;
            int endSize;
            sendLine = (char*)STOS_Alloc(1024+1+postData->imageSize);
            char  postURL[128];
            sprintf(postURL, "%s?%s",postData->page,postData->postSTR);
#ifdef STAR_WINDOWS_VERSION
            headerSize = sprintf(sendLine,
                                 "POST %s HTTP/1.1\r\n"
                                 "Host: %s\r\n"
                                 "Content-type: application/x-www-form-urlencoded\r\n"
                                 "Content-length: %d\r\n\r\n"
                                 "Content-Type: multipart/form-data; boundary=---------------------------14737809831466499882746641449\r\n"
                                 "-----------------------------14737809831466499882746641449\r\n"
                                 "Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n"
                                 "Content-Type: application/octet-stream\r\n\r\n"
                                 
                                 , postURL,postData->host, postData->imageSize, postData->imageDataName,postData->imageDataFileName);
#else
            headerSize = snprintf(sendLine, 512,
                                  "POST %s HTTP/1.1\r\n"
                                  "Host: %s\r\n"
                                  "Content-type: application/x-www-form-urlencoded\r\n"
                                  "Content-length: %d\r\n\r\n"
                                  "Content-Type: multipart/form-data; boundary=---------------------------14737809831466499882746641449\r\n"
                                  "-----------------------------14737809831466499882746641449\r\n"
                                  "Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n"
                                  "Content-Type: application/octet-stream\r\n\r\n"
                                  
                                  , postURL,postData->host, postData->imageSize, postData->imageDataName,postData->imageDataFileName);
#endif
            // append Data here
            memcpy(&sendLine[headerSize], postData->imageBuffer, postData->imageSize);
            headerSize += postData->imageSize;
            endSize = sprintf(&sendLine[headerSize],
                              "\r\n-----------------------------14737809831466499882746641449--\r\n");
            
            sendSize = headerSize + endSize;
#ifdef STAR_WINDOWS_VERSION
            int sendDone =(int) send(postData->sockfd, sendLine, sendSize,0);
#else
            int sendDone =(int) write(postData->sockfd, sendLine, sendSize);
#endif
            SLOG("Send Image size %d Done %d\n",sendSize,sendDone);
        }
        
        STOS_Free(sendLine);
        
        
        
        int readSize;
        int sizeRead = 0;
        //SLOG"\nProcess Start Data\n");
        //readSize = 1;
        //  while( (readSize = read(postData->sockfd, &postData->buffer[sizeRead], DEFAULT_BUFFER_SIZE)) > 0 ) {
#ifdef STAR_WINDOWS_VERSION
        bool haveData = false;
        //Windows in non blocking mode
        do {
            readSize = (int)recv(postData->sockfd, &postData->buffer[sizeRead], DEFAULT_BUFFER_SIZE,0);
            if(readSize > 0)
            {
                sizeRead += readSize;
                haveData = true;
            }
            //SLOG"ReadSize = %d/%d\n",readSize,sizeRead);
        }
        while (readSize > 0 || haveData == false);
        
        if(readSize > 0) {
            postData->buffer[sizeRead] = '\0';
            postData->buffSize = (u32)sizeRead;
        }
#else
        readSize = (int)read(postData->sockfd, &postData->buffer[sizeRead], DEFAULT_BUFFER_SIZE);
        if(readSize > 0)
        {
            postData->buffer[readSize] = '\0';
            postData->buffSize = (u32)readSize;
            sizeRead += readSize;
            //SLOGpostData->buffer);
            
        }
#endif
        
        // }
        
        // Process Data and Error
        // SLOG"\nProcess Data\n");
        char *strCutData;
        // SLOG"\nCutString\n");
        strCutData = strstr(postData->buffer, " ");
        if(strCutData != NULL)
        {
            char errorCode[4];
            strCutData++;
            errorCode[0] = *strCutData++;
            errorCode[1] = *strCutData++;
            errorCode[2] = *strCutData++;
            errorCode[3] = '\0';
            httpErrorCode = atoi(errorCode);
        }
        strCutData++;
        char *firstIdx = strCutData;
        strCutData = strstr(strCutData, "\n");
        strncpy(httpErrorString, firstIdx, strCutData-firstIdx);
        
        
        //    strCutData = strstr(postData->buffer, " ");
        //    if(strCutData != NULL)
        // SLOGpostData->buffer);
        
        s_bHaveResponse = true;
        // SLOG"Got Message %d\n",s_bHaveResponse);
        return NULL;
    }
    
    u32  STNET_IPTON(const char* ipAddr){
        return inet_addr(ipAddr);
    }
    char * STNET_NTOIP(u32 ipValue){
        static char ipAddrString[32];
        
        struct sockaddr_in addrIn;
        addrIn.sin_addr.s_addr = ipValue;//htonl(ipValue); // FIX BUG 2.87
        strcpy(ipAddrString, inet_ntoa(addrIn.sin_addr));
        
        return ipAddrString;
    }
    s32 STNET_InitHTTP(){ // Call this function first if you want to use HTTP Connect
        httpBuffer = (char*)STOS_Alloc(DEFAULT_BUFFER_SIZE);
        if(httpBuffer == NULL)
        {
            return -1;
            
        }
        SLOG("STAR HTTP Init\n");
        
        s_bHttpInit = true;
        s_bHaveResponse = false;
        memset(&postDes, 0, sizeof(POST_STRUCT));
        httpErrorCode = 0;
        httpErrorString[0] = '\0';
        
        return 0;
        
        
        
    }
    
    HTTP_STATUS STNET_HTTP_Get(const char *host,const char *page) // return success;
    {
        //#ifdef STAR_DEBUG
        if(s_bHttpInit == false){
            SLOG("STNET_HTTP_Get ERROR:Call STNET_InitHTTP First\n");
            return HTTP_ERROR;
        }
        //#endif
        
        return HTTP_SUCCESS;
    }
    
    HTTP_STATUS STNET_HTTP_PostImage(const char *host,const char *page,const char *post,const char* imageName,const char* fileName,const char*buffer,int bufferLength){
        //#ifdef STAR_DEBUG
        if(s_bHttpInit == false){
            SLOG("STNET_HTTP_Post ERROR:Call STNET_InitHTTP First\n");
            return HTTP_ERROR;
        }
        //#endif
        
        HTTP_STATUS ret;
        
        
        postDes.imageBuffer = buffer;
        strcpy(postDes.imageDataName,imageName);
        strcpy(postDes.imageDataFileName,fileName);
        postDes.imageSize = bufferLength;
        postDes.sendImage = true;
        
        ret = STNET_HTTP_Post(host,page,post);
        
        
        return ret;
    }
    
    HTTP_STATUS STNET_HTTP_Post(const char *host,const char *page,const char *post){
        //#ifdef STAR_DEBUG
        if(s_bHttpInit == false){
            SLOG("STNET_HTTP_Post ERROR:Call STNET_InitHTTP First\n");
            return HTTP_ERROR;
        }
        //#endif
        char  str[64];
        char **pptr;
        struct hostent *hptr;
        
        int sockfd;
        struct sockaddr_in servaddr;
        
        if( (hptr = gethostbyname(host)) == NULL)
        {
            SLOG("Not found HOST %s\n",host);
            return HTTP_ERROR_CONNECT;
        }
        
        
        
        if (hptr->h_addrtype == AF_INET
            && (pptr = hptr->h_addr_list) != NULL) {
#ifndef STAR_WINDOWS_VERSION
#ifndef STAR_DEBUG
            inet_ntop(hptr->h_addrtype, *pptr, str,
                      sizeof(str));
#else
            SLOG("address: %s\n",
                 inet_ntop(hptr->h_addrtype, *pptr, str,
                           sizeof(str)));
#endif
            
#else // Windows version
#ifndef STAR_DEBUG
            // InetNtop(hptr->h_addrtype, *pptr, str,
            //          sizeof(str));
#else
            //	SLOG"address: %s\n",
            //	       InetNtop(hptr->h_addrtype, *pptr, str,
            //                     sizeof(str)));
#endif
#endif
            
        } else {
            fprintf(stderr, "Error call inet_ntop \n");
        }
        
#ifdef STAR_WINDOWS_VERSION
        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sockfd == INVALID_SOCKET) {
            WSACleanup();
            return HTTP_ERROR_CONNECT;
        }
#else
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
#endif
        memset(&servaddr, 0,sizeof(servaddr));
        //bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(80);
        
#ifdef STAR_WINDOWS_VERSION
        //InetPton(AF_INET,str,&servaddr.sin_addr);
        servaddr.sin_addr.s_addr = *((unsigned long*)hptr->h_addr);
#else
        
        inet_pton(AF_INET, str, &servaddr.sin_addr);
#endif
        
        // Fix to non blocking mode
#ifdef STAR_WINDOWS_VERSION
        u_long set = 0;
        ioctlsocket(sockfd,FIONBIO, &set);
#else
        u_long set = 1;
        ioctl(sockfd,FIONBIO, &set); // set none blocking mode
#endif
        
        int error = connect(sockfd, (struct sockaddr *) & servaddr, sizeof(servaddr));
        socklen_t  len = sizeof(error);
#ifdef STAR_WINDOWS_VERSION
        if(error == SOCKET_ERROR)
            error = WSAGetLastError();
#endif
        
        if (error != 0) {
#ifdef STAR_WINDOWS_VERSION
            if(errno == WSAENETUNREACH)
#else
                if(errno == ENETUNREACH)
#endif
                {
                    SLOG("Connect Error\n");
                    return HTTP_ERROR_CONNECT;
                }
#ifdef STAR_WINDOWS_VERSION
                else if(errno == WSAEINPROGRESS)
#else
                    else if(errno == EINPROGRESS)
#endif
                    {
                        fd_set fds;
                        FD_ZERO(&fds);
                        FD_SET(sockfd,&fds);
                        
                        struct timeval tv;
                        tv.tv_sec = 3;
                        tv.tv_usec = 0;
                        
                        int ret = select(sockfd+1,NULL,&fds,NULL,&tv);
                        if(ret  <=0)
                        {
                            SLOG("Select Error\n");
                            return HTTP_ERROR_CONNECT;
                        }
                        else
                        {
                            //==============================================
                            // Fix on 2.59.2
                            //==============================================
                            if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&error, &len) <0)
                            {
                                SLOG("HTTP Connect connect Error %d\n",errno);
                                return HTTP_ERROR_CONNECT;
                            }
                            if (error > 0) {
                                SLOG("HTTP Connect Not Found Server %d\n",errno);
                                return HTTP_ERROR_CONNECT;
                            }
                        }
                        
                    }
                    else
                    {
                        SLOG("Connect Unknow Error %d\n",error);
                        return HTTP_ERROR_CONNECT;
                    }
        }
        
        // Fix to non blocking mode
#ifdef STAR_WINDOWS_VERSION
        set = 1; // NON block Mode
        ioctlsocket(sockfd,FIONBIO, &set); // set none blocking mode
#else
        set = 0; // Blocking Mode
        ioctl(sockfd,FIONBIO, &set); // set none blocking mode
#endif
        
        
        postDes.sockfd = sockfd;
        strcpy(postDes.host,host);
        strcpy(postDes.page,page);
        strcpy(postDes.postSTR, post);
        postDes.buffer = httpBuffer;
        postDes.buffSize = 0;
        
        if(postDes.thread == true)
        {
#ifdef STAR_WINDOWS_VERSION
            CloseHandle(httpThreadID);
#else
            pthread_join(httpThreadID,NULL);
#endif
            postDes.thread = false;
        }
        
        int ret;
        s_bHaveResponse = false;
#ifdef STAR_WINDOWS_VERSION
        httpThreadID = CreateThread(NULL,0,PostThreadFunc,&postDes,0,&httpThreadCreateID);
#else
        ret = pthread_create(&httpThreadID, NULL, PostThreadFunc, (void*)&postDes);
        if(ret != 0)
        {
            SLOG("Create POST Thread Fail\n");
            return HTTP_ERROR;
        }
#endif
        
        postDes.thread = true;
        //close(sockfd);
        
        return HTTP_SUCCESS;
    }
    
    
    HTTP_STATUS STNET_HTTPGetSendingStatus(){
        //#ifdef STAR_DEBUG
        if(s_bHttpInit == false){
            SLOG("STNET_HTTPGetSendingStatus ERROR:Call STNET_InitHTTP First\n");
            return HTTP_ERROR;
        }
        //#endif
        return HTTP_SUCCESS;
    }
    
    HTTP_STATUS STNET_HTTPGetRecieveStatus(){
        //#ifdef STAR_DEBUG
        if(s_bHttpInit == false){
            SLOG("STNET_HTTPGetRecieveStatus ERROR:Call STNET_InitHTTP First\n");
            return HTTP_ERROR;
        }
        //#endif
        
        return HTTP_SUCCESS;
    }
    bool STNET_HTTP_HaveResponse(){
        return s_bHaveResponse;
    }
    
    int     STNET_HTTP_GetErrorCode(){
        return httpErrorCode;
    }
    char*   STNET_HTTP_GetErrorString(){
        return httpErrorString;
    }
    
    char* STNET_HTTP_GetRespondBuffer(){
        //#ifdef STAR_DEBUG
        if(s_bHttpInit == false){
            SLOG("STNET_HTTP_GetRespondBuffer ERROR:Call STNET_InitHTTP First\n");
            return NULL;
        }
        //#endif
        if(s_bHaveResponse == false)
            return NULL;
        
        s_bHaveResponse = false;
        
        postDes.sendImage = false;
        
        // convert to data only
        if(httpErrorCode != 200)
            return httpBuffer;
        else {
            char *cutData = strstr(httpBuffer, "\r\n\r\n");
            //SLOGstrCutData);
            cutData+=4;
            return cutData;
        }
        
    }
    
    //    #endif