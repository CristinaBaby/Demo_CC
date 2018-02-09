//
//  CBPackPage.h
//  ColorBook
//
//  Created by maxiang on 4/23/15.
//
//

#ifndef __ColorBook__CBPackPage__
#define __ColorBook__CBPackPage__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "../Model/CBAppGlobal.h"
#include "../Widget/CBPage.h"
#include "../Model/CBPackManager.h"
#include "../Module/STInAppBilling.h"

CB_BEGIN_NAMESPACE

class PackPage : public Page, public STIABDelegate
{
public:
    
    CREATE_FUNC(PackPage);
    
    virtual bool init();
    virtual void reset();
    
    void reload(const int pageIndex);
    virtual void touchEnd(cocos2d::Touch *touch);
    
    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid);
    virtual void purchaseFailed(const char *pid, int errorCode);
    virtual void restoreSuccessful(const char* pid);
    virtual void restoreFailed(const char* pid, int errorCode);
    
protected:
    void choosePack(const int index, cocos2d::Sprite *logoSprite);

    void createServerPackUI(PackInfo* pack, const int index);
    void createLocalPackUI(PackInfo* pack, const int index);
    void createSpecialPackUI(PackInfo* pack, const int index);

    void chooseServerPack(const int index, cocos2d::Sprite *logoSprite);
    void chooseLocalPack(const int index, cocos2d::Sprite *logoSprite);
    void chooseSpecialPack(const int index, cocos2d::Sprite *logoSprite);
    
    //Fake a progress timer, used to indicate download progress
    cocos2d::ProgressTimer* createProgressTimer(cocos2d::Sprite *containerSprite, const std::string& packName);

protected:
    /* ------IMPORTANT------
       because load picture scene maybe taking a long time, this value avoid user
       tap too many times 
     */
    bool _hasChoosed;
    int _pageIndex;
    bool _ispurchasing;
    STInAppBilling _appBilling;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBPackPage__) */
