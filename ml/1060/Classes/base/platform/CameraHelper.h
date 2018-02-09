//
//  CameraHelper.h
//  KidsFrameWorkTest
//
//  Created by zhangguangzong1 on 5/4/14.
//
//


#ifndef KidsFrameWorkTest_CameraHelper_h
#define KidsFrameWorkTest_CameraHelper_h

#include "cocos2d.h"

USING_NS_CC;

class CaptureCallback;

class CameraHelper {
    
public:
    
    static CameraHelper *getInstance();
    
    virtual void open();
    virtual void close();
    
    //前后置摄像头切换
    virtual bool swapFrontAndBackCameras();
    
    virtual void capture();
    
    virtual void captureEnd();
    
    virtual void saveToAlbum(Image* img);
    
    virtual void saveCall(bool isSuccess);
    
    virtual bool isCameraEnable();
protected:
    virtual Texture2D* getCaptureTex();
private:
    CaptureCallback* m_Callback;
public:
    inline void setCaptureCall(CaptureCallback* call)
    { m_Callback = call;};
    inline CaptureCallback* getCaptureCall()
    { return m_Callback;};
};

class CaptureCallback{
public:
    virtual void completeCapture(Texture2D* tex) = 0;
    virtual void saveToAlbum(bool isSuccess) = 0;
};

#endif

