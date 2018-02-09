//
//  DecorationLayer.h
//  Make
//
//  Created by QAMAC01 on 15-2-11.
//
//

#ifndef __Make__DecorationLayer__
#define __Make__DecorationLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "MakeLayer.h"
#include "UIUnitScrollView.h"
#include "UITpyeSelView.h"
USING_NS_CC;

class DecorationLayer : public BaseLayer ,public MakeLayerDelegate
{
public:
    static Scene* scene();
    CREATE_FUNC(DecorationLayer);
    bool init();
    
private:
    void showUnitView(std::string typeName);
    void removeUnitView();
    
    void shopClicked();
    void unitSelected(std::string pUnitImageName,int idx);
    
    UIUnitScrollView* unitSelView_;
    
    virtual void nextBtnClicked();
    virtual void resetBtnClicked();
    virtual void preBtnClicked();

    virtual void onEnterTransitionDidFinish();
    virtual void onExit();

    virtual void unitUpdated(){
        nextBtn_ -> setVisible(true);
    };

    void checkUnlock();
    bool isShopClicked_;
    MakeLayer* makeLayer_ = nullptr;
    UITpyeSelView* typeScrollView_;
    
    int _nextCount = 0;
    ui::Button* _preBtn = nullptr;

};
#endif /* defined(__Make__DecorationLayer__) */
