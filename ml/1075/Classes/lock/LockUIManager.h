//
//  LockUIManager.h
//  BHUG1062
//
//  Created by tanshoumei on 5/12/16.
//
//

#ifndef LockUIManager_hpp
#define LockUIManager_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "RewardManager.h"
#include <map>
using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

#define     LOCK_FILE           "content/common/lock.png"
#define     LOCK_FILE_DISABLE   "ui/ui_png/dec/lock_disable.png"
#define     REWARD_LOCK_FILE    "content/common/video.png"

#define     TAG_REWARD_LOCK     0XAAEEA1
#define     TAG_REWARD_LOADING  0XAAEEA2

#define     TAG_IAP_LOCK        TAG_REWARD_LOCK
#define     LOCK_NAME_IAP       "LOCK_NAME_IAP"
#define     LOCK_NAME_REWARD    "LOCK_NAME_REWARD"



class LockUIManager: public Ref
{
    LockUIManager();
    
public:
    static LockUIManager* getInstance();
    
    //reward lock
    bool registerLock(string typeName, const int inx, Node* lockParent);
    
    void distroyRegister(){
        _mapRewardInfos.clear();
    }
    std::function<void()> onRewardSuccess;
    //iap lock
    //    bool registerIAPLock(string iapId, string iapModuelName, const int inx, Node* lockParent);
    
    void setLockPos(Vec2 pos);
    void setLockOriginPos(Vec2 pos){
        _lockPos = pos;
    }
    //reward lock
    void updateLockStatus();
    void rewardsSuccessShowClosed();
    
    void updateIAPLockStatus();
    
    void showNoRewardAdDialog();
    
    void showRewardLoading();
    
    void removeRewardLoading();
    
    //广告逻辑，在ios上如果reward广告请求失败，则将reward的item标记为iap，要求用户购买
    void changeRewardToIAP();
    
private:
    
    void registerRewardLock(string moduleName, string keyInModule, const int inx, Node* lockParent);
    
    //reward lock.
    void addLock(Node* parent);
    
    void addIAPLock(Node* parent);
private:
    map<string, Node*> _mapLockParents;
    map<string, RewardInfoItem> _mapRewardInfos;
    
    map<Node*, string> _mapIAPLockParentNodes;
    
    bool _networkAvailable = true;
    Vec2 _lockPos;
};

#endif /* LockUIManager_hpp */
