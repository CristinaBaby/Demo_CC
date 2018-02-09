//
//  ChoosePageScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__ChoosePageScene__
#define __BedtimeStory__ChoosePageScene__

#include "BaseScene.h"
#include "MXPageView.hpp"
#include "PageControl.h"
#include "STInAppBilling.h"

class ChoosePageScene : public BaseScene, public STIABDelegate
{
public:

    virtual bool init() override;
    virtual void onExit() override;
    virtual void onEnterTransitionDidFinish() override;

    ChoosePageScene();
    virtual ~ChoosePageScene();
    
    static bool needShowCross;
    static bool needShowFullScreen;

    void setupGameType(GameType type);
protected:
    
    void spotGame(const int level);
    void jigsawGame(const int level);
    void colorbookGame(const int level);
    void storyGame(const int level);
    void recordGame(const int level);
    
    void chooseLevelAction(const int index);
    
    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid) override;
    virtual void purchaseFailed(const char *pid, int errorCode) override;
    virtual void restoreSuccessful(const char* pid) override;
    virtual void restoreFailed(const char* pid, int errorCode) override;

protected:
    MXPageView              *_pageView;
    cocos2d::ui::Layout     *_copyPage;
    PageControl             *_pageControl;
    STInAppBilling _iap;

    std::vector<cocos2d::ui::Widget*> _locksVector;
    
    GameType _gameType;
};

#endif /* defined(__BedtimeStory__ChoosePageScene__) */
