//
//  RuntimePermissions.h
//  AdsDemo
//
//  Created by tangjinlong on 16/6/7.
//
//

#ifndef RuntimePermissions_h
#define RuntimePermissions_h
/**
 * android 6.0 runtime permissions
 **/
//#define READ_CALENDAR "android.permission.READ_CALENDAR"
//#define WRITE_CALENDAR "android.permission.WRITE_CALENDAR"
//#define CAMERA  "android.permission.CAMERA"
//
//#define READ_CONTACTS  "android.permission.READ_CONTACTS"
//#define WRITE_CONTACTS  "android.permission.WRITE_CONTACTS"
//#define GET_ACCOUNTS  "android.permission.GET_ACCOUNTS"
//
//#define ACCESS_FINE_LOCATION  "android.permission.ACCESS_FINE_LOCATION"
//#define ACCESS_COARSE_LOCATION  "android.permission.ACCESS_COARSE_LOCATION"
//#define RECORD_AUDIO  "android.permission.RECORD_AUDIO"
//
//#define READ_PHONE_STATE  "android.permission.READ_PHONE_STATE"
//#define CALL_PHONE  "android.permission.CALL_PHONE"
//#define READ_CALL_LOG  "android.permission.READ_CALL_LOG"
//#define WRITE_CALL_LOG  "android.permission.WRITE_CALL_LOG"
//#define ADD_VOICEMAIL  "com.android.voicemail.permission.ADD_VOICEMAIL"
//#define USE_SIP "android.permission.USE_SIP"
//#define PROCESS_OUTGOING_CALLS  "android.permission.PROCESS_OUTGOING_CALLS"
//
//#define BODY_SENSORS  "android.permission.BODY_SENSORS"
//
//#define SEND_SMS  "android.permission.SEND_SMS"
//#define READ_SMS  "android.permission.READ_SMS"
//#define RECEIVE_SMS  "android.permission.RECEIVE_SMS"
//#define RECEIVE_WAP_PUSH  "android.permission.RECEIVE_WAP_PUSH"
//#define RECEIVE_MMS  "android.permission.RECEIVE_MMS"
////#define READ_CELL_BROADCASTS  "Read Cell Broadcasts"
//
//#define READ_EXTERNAL_STORAGE  "android.permission.READ_EXTERNAL_STORAGE"
//#define WRITE_EXTERNAL_STORAGE  "android.permission.WRITE_EXTERNAL_STORAGE"
typedef enum{
kReadCalendar = 0x000001 ,
kWriteCalendar = 0x000001 <<1,
kCamera  = 0x000001 <<2,
    
kReadContacts = 0x000001 <<3,
kWriteContacts= 0x000001 <<4,
kGetAccounts = 0x000001 <<5,
    
kAccessFineLocation= 0x000001 <<6,
kAccessCoraseLocation= 0x000001 <<7,
    
kRecordAudio= 0x000001 <<8,
    
kReadPhoneState= 0x000001 <<9,
kCallPhone= 0x000001 <<10,
kReadCallLog= 0x000001 <<11,
kWriteCallLog= 0x000001 <<12,
kAddVoicemail= 0x000001 <<13,
kUseSIP= 0x000001 <<14 ,
 kProcessOutgoingCalls= 0x000001 <<15 ,
    
kBodySensors= 0x000001 <<16,
    
kSendSMS= 0x000001 <<17 ,
kReadSMS= 0x000001 <<18,
kReceiveSMS= 0x000001 <<19 ,
kReceiveWapPush= 0x000001 <<20 ,
kReceiveMMS= 0x000001 <<21,
    
kReadExternalStorage= 0x000001 <<22 ,
kWriteExternalStorage= 0x000001 <<23
}PERMISSION;
class RuntimePermission{
protected:
    /**
     *@brief constructor
     **/
    RuntimePermission()
    {
    };
    ~RuntimePermission();
public:
    static RuntimePermission* getInstance();
    void requestRuntimePermissions(int requestCode,long permission);
protected:
    static RuntimePermission* m_pInstance;

};

#endif /* RuntimePermissions_h */   
