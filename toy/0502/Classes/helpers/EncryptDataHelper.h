//
//  EncryptDataHelper.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#ifndef __LVUP002__EncryptDataHelper__
#define __LVUP002__EncryptDataHelper__

#include <cocos2d.h>

class EncryptDataHelper : public cocos2d::Ref
{
public:
//    static EncryptDataHelper* getInstance();
//    static void destroy();
    
    /**
     *  for game finish layer to update the new high scores
     *
     *  @return the highest score
     */
    static int  updateHighScore(const std::string &key, int scores);
    
    static bool getPurchaseFlag(const std::string &key);
    static void setPurchaseFlag(const std::string &key, bool value);
    
    static long getRateUsTimestamp(const std::string &key);
    static void setRateUsTimestamp(const std::string &key, long value);
    
    static int getPorpsQuantity(const std::string &key, unsigned defVal);
    static void setPorpsQuantity(const std::string &key, unsigned quantity);
    
    static bool isLevelModeLocked(const std::string &key);
    static void unlockLevelMode(const std::string &key);
    
    static cocos2d::Value getValueFromST(const std::string &key, const cocos2d::Value &defVal);
    static void setValeInST(const std::string &key, const cocos2d::Value &val);
    
    
private:
    EncryptDataHelper(){}
};



#endif /* defined(__LVUP002__EncryptDataHelper__) */
