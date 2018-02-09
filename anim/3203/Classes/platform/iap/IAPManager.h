//
//  IAPManager.h
//  Doctor
//
//  Created by tanshoumei on 13-8-12.
//
//

#include <iostream>
#include "SingleTon.h"
using namespace std;

enum{
    kIAPUnlockAll = 0,
    kIAPUnlockPets,
    kIAPUnlockDressUp
//    kIAPUnlockDisp,
//    kIAPUnlockSide,
//    kIAPUnlockDink,
//    kIAPUnlockPlate
};

/**
 记录iap状态的类.需要在cpp中提供iap总数及相应id，这是各游戏不同的
 */
class IAPManager: public SingleTon<IAPManager>
{
    friend class SingleTon<IAPManager>;
private:
    IAPManager();
public:
    //根据inx获取物品是解锁
    bool getItemIsBought(const int aInx);
    //根据key获取物品是解锁
    bool getItemIsBought(std::string aKey);
    //根据inx获取iap key
    std::string getItemKey(const int aInx);
    
    //=========各游戏特有接口=====
    //提供index解锁iap
    void setItemBoughtWithIndex(const int aInx);
    //提供key(id)解锁iap, 并注意，如果某个iap解锁，要检查其相关的那些是否也可被解锁。如果所有项目都解锁了，当然unlock all也就被解锁了
    void setItemBoughtWithID(std::string sId);
    
    //是否解锁全部iap
    bool isAllUnLocked();
    
    //是否还有广告
    bool isAdRemoved();
};