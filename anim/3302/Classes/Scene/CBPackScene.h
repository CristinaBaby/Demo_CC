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
#include "../Widget/STPageController.h"
#include "../Widget/CBPageControl.h"

CB_BEGIN_NAMESPACE

class PackScene : public cocos2d::Layer, cocos2d::extension::TableViewDataSource
{
public:
    //create scene
    CREATE_SCENE_FUNC(PackScene);
    
    //create func
    CREATE_FUNC(PackScene);
    
    static bool showCrossPromo;
    
    virtual ~PackScene();
    
    virtual bool init();
    virtual void onEnterTransitionDidFinish();

    void backAction();
    void reloadPacks();

    void turnToPage(int pageIndex);

protected:
    // from TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);

protected:
    int getNumberOfCells();
    
protected:
    cocos2d::Size _pageSize;
    STPageController *_pageController;
    PageControl *_pageControl;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBPackScene__) */
