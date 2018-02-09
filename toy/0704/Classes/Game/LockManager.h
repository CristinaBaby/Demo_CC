//
//  LockManager.h
//  ColorBook
//
//  Created by luoxp on 4/24/15.
//
//

#ifndef __ColorBook__LockManager__
#define __ColorBook__LockManager__

#include "UITools.h"

#define xLock   (LockManager::getInstance())

class RewardAdLoadingDelegate;

class LockManager : public Node
{
public:
    static LockManager* getInstance();
    void destroyInstance();
    
    void purchase(const string& identifier);
    void restore();
    
    void rewardAd(int index);
    bool isLocked(const string& identifier);
    bool isLocked(int index);
    void saveNetworkState();
    
    CC_SYNTHESIZE(bool, _networkState, NetworkState);
    
public:
    void purchaseSuccessful(EventCustom* event);
    void rewardAdSuccessful(EventCustom* event);
    
    void update(float dt);
    ~LockManager();
    
protected:
    LockManager();
    bool isPack(const string& identifier);
    int getIdentifierIndex(const string& identifier);
    
private:
    int  _index = 0;
};

#endif /* defined(__ColorBook__LockManager__) */
