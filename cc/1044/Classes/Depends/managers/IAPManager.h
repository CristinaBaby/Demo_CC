//
//  IAPManager.h
//  KidsFramework_CocosV3.2
//
//  Created by zhangguangzong1 on 9/1/14.
//
//

#ifndef __KidsFramework_CocosV3_2__IAPManager__
#define __KidsFramework_CocosV3_2__IAPManager__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "../modules/CFAppBilling.h"
#include "../utils/CSVParse.h"

#define IAP_ID          "IapId"
#define IAP_IMG_NAME    "ImgName"
#define IAP_RANGE       "Range"

#define UNLOCK_ALL      "unlockall"
#define NO_ADS          "noads"

using namespace std;
USING_NS_CC;

#define userDefault         UserDefault::getInstance()
#define kIAPManager         IAPManager::getInstance()

class Product;

typedef vector<Vec2> PaidRange;
typedef unordered_map<string, PaidRange> ImgMap;

class IAPManager : public CFAppBillingDelegate{
    
protected:
    IAPManager();
public:
    static IAPManager* getInstance();
public:
    /*initialization IAP information*/
    void initIAP(const string& csvFile);
    
    void initPurchase();
public:
    /**
     *  after purchase successfully you should call this function
     *
     *  @param iapid
     */
    void updatePurchase(const string& iapid);
    /**
     *  if the img is free
     *
     *  @param name img name
     *
     *  @param index the index of img
     *
     *  @return true->free false->paid
     */
    bool isFree(const string& name,int index = 0);
    /**
     *  if buy all the packages
     *
     *  @return true->unlockall
     */
    bool isUnlockAll();
    /**
     *  if show ads
     *
     *  @return true->show
     */
    bool isShowAds();
    /**
     *  if this package is purchased
     *
     *  @param index package index
     *
     *  @return true->has purchased
     */
    bool isPackagePurchased(int index);
    /**
     *  if this package is purchased
     *
     *  @param iapId iap id for this package
     *
     *  @return true->has purchased
     */
    bool isPackagePurchased(const string& iapId);
    /**
     *  purchase package by index
     *
     *  @param index package index.
     */
    void purchase(int index);
    
    void restore();
public:
    /*purchase call*/
    std::function<void(bool)>   m_fnAfterPurchase;
    std::function<void(bool,bool)>   m_fnAfterRestore;
public:
    /*purchase callback*/
    virtual void purchaseSuccessful(const char* pid);
    
    virtual void purchaseSuccessfulNocallback(const char* pid);

    virtual void purchaseFailed(const char *pid, int errorCode);
    
    virtual void restoreSuccessful(const char* pid);
    
    virtual void restoreSuccessfulNotify(bool isPurchase);
    
    virtual void restoreFailed(const char* pid, int errorCode);
private:
    void csvParse(CSVParse*);
    Product* findProduct(const string& iapId);
    Product* findProductByName(const string& name);
    bool checkAllNormalPackage();
    bool checkUnlockAllPackage();
    bool isUnlockAllPackage(const string& pid);
private:
    /*iap products*/
    Vector<Product*>    m_vProducts;
    
    CFAppBilling    m_oAppBilling;
public:
    /*debug print*/
    void printProductInfos();
};

/**
 *  IAP product
 */
class Product : public Ref
{
protected:
    Product(const string& iapId):m_sProductId(iapId)
    {}
    virtual ~Product(){}
public:
    static Product* create(const string& iapId);
public:
    /**
     *  insert img iap info
     *
     *  @param imgName img name
     *  @param range   paid range
     */
    void insertImg(const string& imgName,const Vec2& range);
    
    bool isInPaidRange(const string& name,int index);
    
    bool hasImg(const string& imgName);
public:
    /*iap id*/
    string      m_sProductId;
    /*key:img name,value:a vector */
    ImgMap      m_mImgs;
    
public:
    
    /*debug print*/
    void printProductInfos();
    
};

#endif /* defined(__KidsFramework_CocosV3_2__IAPManager__) */
