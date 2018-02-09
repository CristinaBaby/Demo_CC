//
//  IAPManager.cpp
//  KidsFramework_CocosV3.2
//
//  Created by zhangguangzong1 on 9/1/14.
//
//

#include "IAPManager.h"

#define kShop0 "Shop0" //UnlockAll
#define kShop1 "Shop1"
#define kShop2 "Shop2"
#define kShop3 "Shop3"
#define kShop4 "Shop4"
#define kShop5 "Shop5"
//上版本的顺序
std::string oldIapKey[] = {kShop0,kShop5,kShop1,kShop2,kShop3,kShop4};

static IAPManager* instance = nullptr;

IAPManager::IAPManager():m_fnAfterPurchase(nullptr),m_fnAfterRestore(nullptr)
{
}

IAPManager* IAPManager::getInstance()
{
    if (!instance)
    {
        instance = new IAPManager();
    }
    return instance;
}

void IAPManager::initIAP(const string& csvFile)
{
    if (m_vProducts.size() != 0)
    {
        log("old iap information will be cleared.");
        m_vProducts.clear();
    }
    
    CSVParse* parse = CSVParse::create(csvFile.c_str());
    if (parse)
    {
        this->csvParse(parse);
    }else
    {
        log("csv file has some problems.");
        return;
    }
    
    this->initPurchase();
    this->checkOldPurchase();
}

void IAPManager::checkOldPurchase()
{
    for (int i = 0; i<m_vProducts.size(); i++)
    {
        bool isOldPurchased = kUserDefault -> getBoolForKey(oldIapKey[i].c_str());
        if (isOldPurchased)
        {
            log("old key is %s",  oldIapKey[i].c_str());
            log("old package %s is free",  m_vProducts.at(i)->m_sProductId.c_str());
            freePackge(m_vProducts.at(i)->m_sProductId.c_str());
        }
    }
    
}


void IAPManager::initPurchase()
{
    m_oAppBilling.setIABDelegate(this);
}

void IAPManager::updatePurchase(const string& id)
{
    kUserDefault->setBoolForKey(id.c_str(),true);
    kUserDefault->flush();
}

bool IAPManager::isUnlockAll()
{
    return this->isFree(UNLOCK_ALL);
}

bool IAPManager::isShowAds()
{
    if (m_vProducts.size() == 0)
    {
        return true;
    }
    
    bool isUnlockAll = this->isUnlockAll();
    if (isUnlockAll)
    {
        return false;
    }
    if (this->findProductByName(NO_ADS))
    {
        return !this->isFree(NO_ADS);
    }
    return true;
}

void IAPManager::purchase(int index)
{
    if (index < 0 || index >= m_vProducts.size())
    {
        log("invalid package index.");
        return;
    }
//    purchaseSuccessful(m_vProducts.at(index)->m_sProductId.c_str());
    m_oAppBilling.purchase(m_vProducts.at(index)->m_sProductId.c_str());
}

void IAPManager::restore()
{
    m_oAppBilling.restore();
}

bool IAPManager::isFree(const string& name,int index)
{
    bool flag = true;
    for (auto iter = m_vProducts.begin(); iter != m_vProducts.end(); iter++)
    {
        Product* product = *iter;
        if (product->isInPaidRange(name, index)){
            flag = kUserDefault->getBoolForKey(product->m_sProductId.c_str(),false);
            if(flag)
                return flag;
        }
        
    }
    return flag;
}

bool IAPManager::isPackagePurchased(int index)
{
    if (index < 0 || index >= m_vProducts.size())
    {
        return true;
    }
    return this->isPackagePurchased(m_vProducts.at(index)->m_sProductId);
}

bool IAPManager::isPackagePurchased(const string& iapId)
{
    return kUserDefault->getBoolForKey(iapId.c_str(),false);
}

void IAPManager::csvParse(CSVParse* prase)
{
    int rows = prase->getRows();
    for (int i = 1; i < rows; i++)
    {
        string iapId = prase->getDatas(i,IAP_ID);
        string imgName= prase->getDatas(i,IAP_IMG_NAME);
        string range = prase->getDatas(i,IAP_RANGE);
        
        if( iapId != "")
        {
            Product* product = this->findProduct(iapId);
            if (!product)
            {
                product = Product::create(iapId);
                m_vProducts.pushBack(product);
            }
            product->insertImg(imgName,PointFromString(range));
        }
    }
}

Product* IAPManager::findProduct(const string& productId)
{
    for (auto iter = m_vProducts.begin(); iter != m_vProducts.end(); iter++)
    {
        if ((*iter)->m_sProductId == productId)
        {
            return *iter;
        }
    }
    return NULL;
}

Product* IAPManager::findProductByName(const string& name)
{
    for (auto iter = m_vProducts.begin(); iter != m_vProducts.end(); iter++)
    {
        if ((*iter)->hasImg(name))
        {
            return *iter;
        }
    }
    return NULL;
}

bool IAPManager::checkAllNormalPackage()
{
    bool buyAll = true;
    for (auto iter = m_vProducts.begin(); iter != m_vProducts.end(); iter++)
    {
        if (!this->isPackagePurchased((*iter)->m_sProductId))
        {
            if (!this->isUnlockAllPackage((*iter)->m_sProductId))
            {
                buyAll = false;
                return false;
            }
        }
    }
    if (buyAll)
    {
        Product* product = this->findProductByName(UNLOCK_ALL);
        if (product)
        {
            this->updatePurchase(product->m_sProductId);
        }
    }
    return true;
}

bool IAPManager::checkUnlockAllPackage()
{
    Product* product = this->findProductByName(UNLOCK_ALL);
    if (product)
    {
        if (this->isPackagePurchased(product->m_sProductId))
        {
            for (auto iter = m_vProducts.begin(); iter != m_vProducts.end(); iter++)
            {
                 this->updatePurchase((*iter)->m_sProductId);
            }
        }
    }
    return true;
}

bool IAPManager::isUnlockAllPackage(const string& pid)
{
    Product* product = this->findProductByName(UNLOCK_ALL);
    if (product)
    {
        return product->m_sProductId == pid;
    }
    return false;
}

void IAPManager::freePackge(const char* pid){
    this->updatePurchase(string(pid));
    if (this->isUnlockAllPackage(pid))
    {
        this->checkUnlockAllPackage();
    }else
    {
        this->checkAllNormalPackage();
    }
}

void IAPManager::purchaseSuccessful(const char* pid)
{
    this->updatePurchase(string(pid));
    if (this->isUnlockAllPackage(pid))
    {
        this->checkUnlockAllPackage();
    }else
    {
        this->checkAllNormalPackage();
    }
    
    if (m_fnAfterPurchase)
    {
        m_fnAfterPurchase(true);
    }
}

void IAPManager::purchaseFailed(const char *pid, int errorCode)
{
    if (m_fnAfterPurchase)
    {
        m_fnAfterPurchase(false);
    }
}

void IAPManager::restoreSuccessful(const char* pid)
{
    this->updatePurchase(string(pid));
    if (this->isUnlockAllPackage(pid))
    {
        this->checkUnlockAllPackage();
    }else
    {
        this->checkAllNormalPackage();
    }
}

void IAPManager::restoreSuccessfulNotify(bool isPurchase)
{
    if (m_fnAfterRestore)
    {
        m_fnAfterRestore(true,isPurchase);
    }
}

void IAPManager::restoreFailed(const char* pid, int errorCode)
{
    if (m_fnAfterRestore)
    {
        m_fnAfterRestore(false,false);
    }
}

void IAPManager::printProductInfos()
{
    int count = (int)m_vProducts.size();
    for (int i = 0; i < count; i++)
    {
        Product* product = m_vProducts.at(i);
        product->printProductInfos();
    }
}

/*************************@implementation product****************************/
Product* Product::create(const string& iapId)
{
    Product* product = new Product(iapId);
    if (product)
    {
        product->autorelease();
        return product;
    }
    CC_SAFE_DELETE(product);
    return NULL;
}

bool Product::isInPaidRange(const string& name,int index)
{
    ImgMap::iterator it = m_mImgs.find(name);
    if (it != m_mImgs.end())
    {
        PaidRange vRange = it->second;
        for (int i = 0; i < vRange.size();i++ )
        {
            Vec2 range = vRange[i];
            if (index >= range.x && index <= range.y)
            {
                return true;
            }
        }
    }else
    {
        return false;
    }
    return false;
}

void Product::insertImg(const string& imgName,const Vec2& range)
{
    ImgMap::iterator it = m_mImgs.find(imgName);
    if (it != m_mImgs.end())
    {
        (it->second).push_back(range);
    }else
    {
        PaidRange vRange;
        vRange.push_back(range);
        m_mImgs.insert(pair<string,PaidRange>(imgName,vRange));
    }
}

bool Product::hasImg(const string& imgName)
{
    ImgMap::iterator it = m_mImgs.find(imgName);
    if(it != m_mImgs.end())
    {
        return true;
    }else
    {
        return false;
    }
}

void Product::printProductInfos()
{
    for (auto it = m_mImgs.begin(); it != m_mImgs.end(); it++)
    {
        PaidRange vRange = it->second;
        __String* rangStr = __String::createWithFormat("range = ");
        for (int i = 0; i < vRange.size(); i++)
        {
            rangStr->appendWithFormat("{%d,%d},",(int)(vRange.at(i).x),(int)(vRange.at(i).y));
        }
        log("IAP ID:%s\nImgName = %s\n%s",m_sProductId.c_str(),it->first.c_str(),rangStr->getCString());
    }
}