//
//  CBPackScene.h
//  ColorBook
//
//  Created by maxiang on 4/22/15.
//
//

#ifndef __ColorBook__CBPackScene__
#define __ColorBook__CBPackScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Model/CBAppGlobal.h"
#include "../Widget/CBPageControl.h"
#include "../Widget/STPageController.h"
#include "cocosgui.h"
#include "AppConfigs.h"
#include "PackManager.h"

using std::string;

class PackScene : public cocos2d::Scene
{
    virtual void onEnterTransitionDidFinish() override;
public:
    //create func
    CREATE_FUNC(PackScene);
    
    virtual ~PackScene();
    virtual bool init();
    
    void backAction();
    void updatePacks(int tag);
    void onExit();
    void touchDown(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void turnToPage(int pageIndex);
    void beBack(){ _beBack = true;};
    
    // net work
    void networkChanged();
    bool getNetWorkState();
    void onDialog(const string& name);
protected:
    void createLocalPackUI(PackInfo* pack, const int index);
    void selecteItemEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void chooseLocalPack(const int index);
    void reload();
    
protected:
    cocos2d::Size _pageSize;
    STPageController *_pageController;
    PageControl *_pageControl;
    cocos2d::ui::ListView* _listView;
    bool _hasChoosed = false;
    bool _beBack = false;
    bool _block = false;
    cocos2d::ui::Button * _back;
    int _index;
    
    bool _isRewardAd;
};



#endif /* defined(__ColorBook__CBPackScene__) */
