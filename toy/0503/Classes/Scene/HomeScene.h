//
//  HomeScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/28/15.
//
//

#ifndef __BedtimeStory__HomeScene__
#define __BedtimeStory__HomeScene__

#include "BaseScene.h"
#include "STInAppBilling.h"

class HomeScene : public BaseScene, public STIABDelegate
{
public:
    virtual bool init() override;

protected:
    void storyAction();
    void parentsAction();
    void miniGameAction();
    void shopAction();
    void moreGameAction();

    void showParentGate();
    
    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid) override;
    virtual void purchaseFailed(const char *pid, int errorCode) override;
    virtual void restoreSuccessful(const char* pid) override;
    virtual void restoreFailed(const char* pid, int errorCode) override;
protected:
    
    STInAppBilling _iap;
};




#endif /* defined(__BedtimeStory__HomeScene__) */
