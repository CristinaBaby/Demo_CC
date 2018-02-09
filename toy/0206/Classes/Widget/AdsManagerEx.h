//
//  AdsManagerEx.hpp
//  TOYBM0205
//
//  Created by luoxiaopeng on 15/10/20.
//
//

#ifndef AdsManagerEx_hpp
#define AdsManagerEx_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AdsManager.h"

#define xAdsEx                  (AdsManagerEx::getInstance())

using namespace cocos2d;
using std::string;

class AdsManagerEx {
    
    class CAccessor : public AdsManager
    {
        friend class AdsManagerEx;
    };
    
public:
    static AdsManagerEx* getInstance();
    static void destroy();
    
    void setBannerShow(bool s);
    bool isShowBanner();
    
private:
    AdsManagerEx();
    CAccessor  *me = nullptr;
};

#endif /* AdsManagerEx_hpp */
