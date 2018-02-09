//
//  MiniGameSelectScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__MiniGameSelectScene__
#define __BedtimeStory__MiniGameSelectScene__

#include "BaseScene.h"

class MiniGameSelectScene : public BaseScene
{
public:
    
    MiniGameSelectScene();
    virtual ~MiniGameSelectScene();
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;

    static bool needShowCross;
    
    virtual void onExit() override;

protected:
    
    void colorBookAction();
    void spotAction();
    void jigsawAction();
};

#endif /* defined(__BedtimeStory__MiniGameSelectScene__) */
