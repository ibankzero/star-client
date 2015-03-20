//
//  StarProtocol.h
//  StarMacOS
//
//  Created by SAMRET WAJANASATHIAN on 18/6/2557.
//
//

#ifndef StarMacOS_StarProtocol_h
#define StarMacOS_StarProtocol_h


/*---------------------------------------------------------------------------*
 StarSock Protocal definitions
 *---------------------------------------------------------------------------*/
#define MAX_CHAR_NAME     32
// Register Response
#define STSOCKMSG_REGIS_OK_ALWAY    1
#define STSOCKMSG_REGIS_OK_SECTION  2
#define STSOCKMSG_OLD_VERSION       3

#define STSOCKMSG_SERVER_FULL       4 // ใช้ได้เฉพาะตอน Connect เท่านั้น ไปใช้ที่อื่นจะซ้ำกันใช้ไม่ได้
#define STSOCKMSG_CLIENT_WRONG      5

// All System message must be negative

#define STSOCKID_ALLPARTY           -6
#define STSOCKID_ALLFRIEND          -5
#define STSOCKID_ALLROOM            -4  // only in same room
#define STSOCKID_UNKNOWS            -3
#define STSOCKID_ALL                -2
#define STSOCKID_SERVER             -1


// system packet จะเป็นค่าติดลบ ทั้งหมด
#define STSOCKMSG_DATASIZE          -1
#define STSOCKMSG_GIVEID            -2
#define STSOCKMSG_KNOCK             -3
#define STSOCKMSG_CLOSE_CONNECTION  -4
#define STSOCKMSG_REGISTER          -5   // first time check
#define STSOCKMSG_RETRASMIT         -6
#define STSOCKMSG_STREAMDATA        -7
#define STSOCKMSG_PING              -9
#define STSOCKMSG_CONFIRM           -10
#define STSOCKMSG_DUPLICATE         -11
  
#define STSOCKMSG_BIGDATA_HEADER    0x0001
#define STSOCKMSG_BIGDATA_CONTINUE  0x0002
#define STSOCKMSG_BIGDATA_END       0x0004

// สามารถส่งพร้อมกันได้หมาบ ๆ ตัว
#define STSOCKMSG_PACKDATASIZE      0x0001
#define STSOCKMSG_CHATDATA          0x0002
#define STSOCKMSG_FILEDATA          0x0004 // for patch or other file use only TCP
#define STSOCKMSG_BIGDATA           0x0008

#define STSOCKMSG_ALLDATA           0x000f

#define STSOCKMSG_STREAMBLOCKSIZE  (8*1024)


#define SIZE_SEND_PER_FRAME        512


/*---------------------------------------------------------------------------*
 constant definitions
 *---------------------------------------------------------------------------*/
#define STAR_CLIENT_DEFAULT_FRAME           8   // network will work 8 frame per second it want to done recieve in 125 milli second
#define STAR_CLIENT_DEFAULT_BUFFER          1024 // send and recieve buffer per block
#define STAR_CLIENT_DEFAULT_MAX_BLOCK       64   // Can't adjust it effect with server
#define STAR_CLIENT_DEFAULT_PORT            9000 // default port number
#define STAR_CLIENT_DEFAULT_TIMEOUT         30   // second
#define STAR_CLIENT_DEFAULT_CON_TIMEOUT      5   // connection time out
#define STAR_CLIENT_DEFAULT_WATING_TIME     3000 // wating popup milli sec
// retrans default 32 milli second ใน1 เพรมจะส่ง 4 ครั้งเพื่อให้ได้ข้อมูลให้ได้
#define STAR_CLIENT_DEFAULT_RETRASMIT       ((1000 / STAR_CLIENT_DEFAULT_FRAME) / 4)



/*---------------------------------------------------------------------------*
 Packet Type
 *---------------------------------------------------------------------------*/
#define STAR_CLIENT_DATA_TYPE_PACKET        0x01
#define STAR_CLIENT_DATA_TYPE_BIG_DATA      0x02
#define STAR_CLIENT_DATA_TYPE_FILE          0x03

/*---------------------------------------------------------------------------*
 constant definitions
 *---------------------------------------------------------------------------*/
#define STAR_CLIENT_DEFAULT_FRAME           8   // network will work 8 frame per second it want to done recieve in 125 milli second
#define STAR_CLIENT_DEFAULT_BUFFER          1024 // send and recieve buffer per block
#define STAR_CLIENT_DEFAULT_MAX_BLOCK       64   // Can't adjust it effect with server
#define STAR_CLIENT_DEFAULT_PORT            9000 // default port number
#define STAR_CLIENT_DEFAULT_TIMEOUT         30   // second
#define STAR_CLIENT_DEFAULT_CON_TIMEOUT      5   // connection time out
#define STAR_CLIENT_DEFAULT_WATING_TIME     3000 // wating popup milli sec
// retrans default 32 milli second ใน1 เพรมจะส่ง 4 ครั้งเพื่อให้ได้ข้อมูลให้ได้
#define STAR_CLIENT_DEFAULT_RETRASMIT       ((1000 / STAR_CLIENT_DEFAULT_FRAME) / 4)
/*---------------------------------------------------------------------------*
 connection definitions
 *---------------------------------------------------------------------------*/
/* Local Network */
#define STAR_CLIENT_BLUETOOTH_CONNECT       0x01 // Local network only
#define STAR_CLIENT_WIRELESS_CONNECT        0x02 // Local network only
#define STAR_CLIENT_LAN_CONNECT             0x03 // LAN USE UDP Protocal only
/* Internet network */
#define STAR_CLIENT_UDP_CONNECT             0x04 // Internet network
#define STAR_CLIENT_TCP_CONNECT             0x05 // Internet network
/* Connection Type */
#define STAR_CLIENT_ALWAY_ON                0x06
#define STAR_CLIENT_SECTION                 0x07

/*---------------------------------------------------------------------------*
 Error definitions
 *---------------------------------------------------------------------------*/
/* Error Code 0x00XX */
#define STAR_CLIENT_ERROR_NONE              0x0000
/* Error Code 0x01XX Client */
#define STAR_CLIENT_ERROR_PROXY_FAIL        0x0101 // connect to proxy server fail
#define STAR_CLIENT_ERROR_SERVER_FAIL       0x0102 // connect to server fail
#define STAR_CLIENT_ERROR_COUNTYR_FAIL      0x0103 // Block Conutry code
#define STAR_CLIENT_ERROR_VERSION_FAIL      0x0104 // when client verion not correct
#define STAR_CLIENT_ERROR_BAN               0x0105 // Gernerat when account are block or ban
#define STAR_CLIENT_ERROR_ACCOUNT           0x0106 //  ไม่มี Account ID นี้
#define STAR_CLIENT_ERROR_PASS              0x0107 //  Password ไม่ถูกต้อง
#define STAR_CLIENT_ERROR_MAINTAIN          0x0108 // Server Maintain
#define STAR_CLIENT_ERROR_DISCONNECT        0x0109 //  ตัดการเชื่อมต่อจาก Server
#define STAR_CLIENT_ERROR_DUPLICATE         0x010a // มีการ login ซ้อนเข้ามา
#define STAR_CLIENT_ERROR_TIMEOUT           0x010b // Time out ไม่สามารถเชื่อมต่อ server ได้
#define STAR_CLIENT_ERROR_CONNECT_FAIL      0x010c // Create Connection Fail/Server หลุด
#define STAR_CLIENT_ERROR_WAITING_DATA      0x010d // wating data but still connect
#define STAR_CLIENT_ERROR_OLD_VERSION       0x010e // server version not match
#define STAR_CLIENT_ERROR_REF_NO            0x010f // packNo Not correct
#define STAR_CLIENT_ERROR_NOT_SUPPORT       0x0110 // Client Old Version
/* Error Code 0x02xx Hardware */
#define STAR_CLIENT_ERROR_NO_INTERNET       0x0201 // Not connnect to internet

/* Error Code 0x03xx Server */
#define STAR_CLIENT_ERROR_SERVERFULL        0x0301  // Server Full
#define STAR_CLIENT_ERROR_NOT_SUPPORT_ALWAY 0x0302  // Server Not Support Allway on
/*---------------------------------------------------------------------------*
 macro definitions
 *---------------------------------------------------------------------------*/
// For 32-byte alignment
#define     STAR_ROUNDUP_32B( x ) ( ( (x) + 31 ) & 0xffffffe0 )


/*---------------------------------------------------------------------------*
 Chat System definitions
 *---------------------------------------------------------------------------*/
#define CHAT_MAX_FRIEND    99
#define CHAT_MAX_PARTY     5
#define CHAT_MAX_GUILD     99
#define CHAT_MAX_ALLIAN    10
#define CHAT_MAX_ID        9000
#define CHAT_MAX_MESSAGE_POOL  5000
#define CHAT_MAX_PER_MSG   128

#define CHAT_POLLING_TIME      2000 // millisec

#define CHAT_TO_BOSS_ID       0xff1f
#define CHAT_TO_GM_ID         0xff2f

#define CHAT_OFFLINE_COUNT     3  //1 tick = 5 second if not sending it will time out

#pragma -
#pragma mark Login Package
#pragma -

#define  CHAT_LOGIN_REQ                 0x0001
#define  CHAT_LOGIN_RES                 0x0002  // SEND chat detail to client
#define  CHAT_FRIEND_NAME_REQ           0x0003
#define  CHAT_FRIEND_NAME_RES           0x0004

#define  CHAT_LOGIN_FAIL_RES            0xf001

#pragma mark - CHAT_PROTOCAL

#define  CHAT_ONLINE_RES                0x0005  // Status online  // send online id
#define  CHAT_OFFLINE_RES               0x0006  // Status offline // send offline id

#define  CHAT_READED_REQ                0x0007  // return readable
#define  CHAT_READED_RES                0x0008  // return when recieve message

#define  CHAT_CHAT_REQ                  0x0009  // Chat detail return CHAT_RECIEVE_RES
#define  CHAT_CHAT_RES                  0x000a  // return when recieve message

#define  CHAT_ADD_FRIEND_REQ            0x000b
#define  CHAT_ADD_FRIEND_RES            0x000c

#define  CHAT_DEL_FRIEND_REQ            0x000d
#define  CHAT_DEL_FRIEND_RES            0x000e

#define  CHAT_PARTY_UPDATE_RES          0x000f
//#define  CHAT_ADD_PARTY_REQ             0x000f
//#define  CHAT_ADD_PARTY_RES             0x0010
//
//#define  CHAT_DEL_PARTY_REQ             0x0011
//#define  CHAT_DEL_PARTY_RES             0x0012

#define  CHAT_ADD_GUILD_REQ             0x0013
#define  CHAT_ADD_GUILD_RES             0x0014

#define  CHAT_DEL_GUILD_REQ             0x0015
#define  CHAT_DEL_GUILD_RES             0x0016

#define  CHAT_ADD_ALLIANCE_REQ          0x0017
#define  CHAT_ADD_ALLIANCE_RES          0x0018

#define  CHAT_DEL_ALLIANCE_REQ          0x0019
#define  CHAT_DEL_ALLIANCE_RES          0x001a

#define  CHAT_POLL_REQ                  0x001b
#define  CHAT_POLL_RES                  0x001f
#define  CHAT_MESSAGE_RES               0x001c

#define  CHAT_FRIEND_UPDATE_RES         0x0020


#define CHAT_VERSION        1
/*---------------------------------------------------------------------------*
 structure definitions
 *---------------------------------------------------------------------------*/
//  ตัวอย่างสถานะการเชื่อมต่อแบบ Internet
//  NONE -> READY(ต่อ server ได้) -> CHILD (login ได้) -> READY (หลุดจากเกม Server แต่ยังต่อได้) -> NONE (ต่อไม่ได้แล้ว)
typedef enum {
    SC_STATUS_READY     =   0,  //ค่าเริ่มต้นสถานะ
    SC_STATUS_ERROR,            //ไม่สามารถระบุได้ว่าอะไรเสีย
    SC_STATUS_BUSY,             //กำลังส่งข้อมูลอยู่
    SC_STATUS_PARENT,           //มีสถานะเป็น Host ใช้ในกรณีที่ไม่ได้เล่นผ่านเนต
    SC_STATUS_CHILD,            //เชื่อมต่อได้ และมีสถานะเป็น ลูก ใช้ได้ทั้งInternet และ Local
    SC_STATUS_SCAN,             // ใช้เพื่อหา Host ใช้ได้เฉพาะ Local
    SC_STATUS_CONNECTING,
    SC_STATUS_CONNECTED,
    SC_STATUS_NONE,             // ไม่ได้เชื่อมต่อ การเชื่อต่อหลุด จะกลับมาที่สถานะนี้
    SC_STATUS_MAX
}SCStatus;

typedef enum {
    SC_LINK_BAD       =    0,
    SC_LINK_POOL,
    SC_LINK_NORMAL,
    SC_LINK_GOOD,
    SC_LINK_BEST,
    SC_LINK_MAX      // 5
}SCLinkLevel;


/*---------------------------------------------------------------------------*
 structure definitions
 *---------------------------------------------------------------------------*/
/*  Packet header maximum 32K  byte per packet */

enum MSG_TYPE{
    MSG_TYPE_MESSAGE = 1,
    MSG_TYPE_EMOTION = 2,
    MSG_TYPE_ITEM = 3,
    MSG_TYPE_WEAPON = 4,
    MSG_TYPE_MONEY = 5,
    MSG_TYPE_GOLD = 6,
    MSG_TYPE_JADE = 7,
};

enum SEND_TYPE{
    TYPE_WORLD    = 0,
    TYPE_ALLIANCE = 1,
    TYPE_GUILD    = 2,
    TYPE_PARTY    = 3,
    TYPE_FRIEND   = 4,
    TYPE_MAX,
};


//enum SEND_TYPE{
//    TYPE_FRIEND     = 0,
//    TYPE_PARTY      = 1,
//    TYPE_GUILD      = 2,
//    TYPE_ALLIANCE   = 3,
//    TYPE_GM         = 4,
//    TYPE_WORLD      = 5
//};

enum MSG_STATUS{
    MSG_NOT_SEND    =0,
    MSG_SENDING,
    MSG_RECIEVE,
    MSG_READED,
    MSG_DELIVERY
};

typedef struct SCHeader_
{
    u32      packetNo;
    u16      packetType;  // if send error will not have and auto will appear
    u16      packetSize;
}SCHeader;  //8 byte


struct MSG_FILE_HEADER{
    u32      version;
    u32      msgCount;
    u32      lastNo;
    u16      notif;
    u16      reserved;
};

struct Chat_Header{
    u16        packSize;
    u16        type;
};

struct Chat_Polling{
    u32        zenID;
    u32        lastID;
    u32        mode;
    u32        reserved;
};

struct Chat_Time{
    u16    year;
    u8     month;
    u8     day;
    u8     house;
    u8     min;
    u16    reserved;
}; // 8byte

struct Chat_Message{
    Chat_Time  dateTime;  //8
    u32     charID;
    char    fromName[MAX_CHAR_NAME];
    u32     ID;
    u8      status;
    u8      sendType;
    u8      type; // 1Message 2 Emotion,3 ส่งของ 4 ส่งเงิน
    u16     fromID;
    u16     toID;
    char    message[128];
    u32     sendItemID; // or emotion or money depenon type
    
}; // 152

struct Chat_MessageSend{
    u8      sendType;
    u8      type; // 1Message 2 Emotion,3 ส่งของ 4 ส่งเงิน
    u16     fromID;
    u16     toID;
    char    message[128];
    u32     sendItemID; // or emotion or money depenon type
};

struct ChatDataDetail{
    u32  UID;  //4:4
    u32  zenID; //4:8
    char name[MAX_CHAR_NAME];
    
    u8   maxFriend;//1:
    u8   maxParty; // 1:
    u8   maxGuild; //1:
    u8   maxAlliance; //1:
    
    u16  friendID[CHAT_MAX_FRIEND];//200:212 // other PlayerID
    u16  partyID[CHAT_MAX_PARTY]; //20:232 Friend in Party // got zen id to map partIdx
    u16  guildID[CHAT_MAX_GUILD]; //200:532 Guild ID ทุกคนในกิล
    u16  allianceID[CHAT_MAX_ALLIAN]; //20:552 Guild อื่น ๆ
    
    
    
};

struct ChatUpdateFriendStatus{
    u16  friendID[CHAT_MAX_FRIEND];
    u8   onlineStatus[CHAT_MAX_FRIEND];
};

struct ChatUpdatePartyStatus{
    u32   partyZenID[CHAT_MAX_PARTY];
};

struct FriendDetail{
    u16   zenID;
    u32   charID;
    char  name[MAX_CHAR_NAME];
};

//struct Chat_Message{
//    u8      toType; // friend,guild party GM
//    u16     emotion; // xx xx set/emo  0 mean not have emo
//    u32     msgID;  // Individual combile with fromID
//    u16     toID;
//    u16     fromID;
//    char    message[CHAT_MAX_PER_MSG];
//};

#endif
