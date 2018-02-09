//
//  ShareLayer.h
//  Make
//
//  Created by QAMAC01 on 15-2-12.
//
//

#ifndef __Make__ShareLayer__
#define __Make__ShareLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "MakeLayer.h"

USING_NS_CC;

class ShareLayer : public BaseLayer
{
public:
    static Scene* scene();
    CREATE_FUNC(ShareLayer);
    bool init();
    
private:
    
    virtual void onEnterTransitionDidFinish();
    virtual void favBtnClicked();
    virtual void newOneBtnClicked();
    virtual void onExit();
    virtual void reeatClicked();
    void showEatAgain(Ref* obj);
    
    RenderTexture* screenShot();
    MakeLayer* makeLayer_;
    ui::Button* _reEatBtn;

};


#endif /* defined(__Make__ShareLayer__) */
