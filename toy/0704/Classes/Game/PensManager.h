//
//  PensManager.h
//  TOYBM0701
//
//  Created by huangwen on 15/11/10.
//
//

#ifndef PensManager_hpp
#define PensManager_hpp

#include "cocos2d.h"
#include "cocosgui.h"
USING_NS_CC;

#define xPens   PensManager::getInstance()

typedef  std::function<void(Ref*,ui::Widget::TouchEventType)> callFunc;

class PensManager
{
public:
    struct pensInfo{
        int lockID;
        std::string parchaseInfo;
    };
    
    
    static PensManager* getInstance();
    
    PensManager();
    
    //from memory get is lock
    bool pensLockSign(int group);
    //unlock
    void pensUnLock(int index, bool lock);
    
    bool createBoxFrontground(int index);
    bool createBoxBackground(int index);
    
    bool isHaveLock(int index);
    
    bool getLockState(int index);
    
    int getGroup(int index);
    
    void setPensParchaseInfo(const std::string& path);
    
    std::string getParchaseInfoFromIndex(int index);
    
    std::vector<int>  getPackInfoID();
    
private:
    static PensManager* msg;
    
    std::vector<pensInfo*> _info;
    
    ValueVector _value;
    
    int _sign = 0;
    int _signFront = 0;
    
    int _acc = 0;
    
    UserDefault* _user = UserDefault::getInstance();
    
    callFunc _callback;
    
    bool    _lockArr[6];
};

#endif /* PensManager_hpp */
