//
//  AdsManagerEx.cpp
//  TOYBM0205
//
//  Created by luoxiaopeng on 15/10/20.
//
//

#include "AdsManagerEx.h"
#include "UICpp.h"

static AdsManagerEx* _sharedAdsManagerEx = NULL;

AdsManagerEx* AdsManagerEx::getInstance()
{
    if (NULL == _sharedAdsManagerEx)
    {
        _sharedAdsManagerEx = new AdsManagerEx();
    }
    
    return _sharedAdsManagerEx;
}

void AdsManagerEx::destroy()
{
    CC_SAFE_DELETE(_sharedAdsManagerEx);
}

AdsManagerEx::AdsManagerEx()
{
    me = (CAccessor*)xAds;
}

void AdsManagerEx::setBannerShow(bool s){
    me->m_bVisibleFlagBannber = s;
}

bool AdsManagerEx::isShowBanner(){
    return me->m_bVisibleFlagBannber;
}
