//
//  CBPictureScene.h
//  ColorBook
//
//  Created by maxiang on 4/23/15.
//
//

#ifndef __ColorBook__CBPictureScene__
#define __ColorBook__CBPictureScene__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"
#include "../Widget/STPageController.h"
#include "../Widget/CBPageControl.h"
#include "../Widget/CBPicturePage.h"

CB_BEGIN_NAMESPACE

class PictureScene : public cocos2d::Layer, cocos2d::extension::TableViewDataSource
{
public:
    //create scene
    static cocos2d::Scene* createSceneWithPackIndex(const int packIndex);
    
    //create func
    CREATE_FUNC(PictureScene);
    
    virtual ~PictureScene();
    
    void setPackIndex(const int index);
    
    virtual bool init();
    
    void backAction();
    
    void onExit();
    void onEnter();
    
protected: // from TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
    void setupCell(cocos2d::extension::TableViewCell *cell, int index);
    
    /* async load picture texture async */
    void loadPictureTexture();
    
    /* the number of the cells */
    int getNumberOfCells();

protected:
    cocos2d::Size _pageSize;
    STPageController *_pageController;
    int _packIndex;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBPictureScene__) */
