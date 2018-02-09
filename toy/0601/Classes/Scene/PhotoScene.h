//
//  PhotoScene.h
//  PaintDress
//
//  Created by maxiang on 6/29/15.
//
//

#ifndef __PaintDress__PhotoScene__
#define __PaintDress__PhotoScene__

#include "cocos2d.h"
#include "cocosGUI.h"
#include "cocos-ext.h"
#include "../Model/AppConfigs.h"
#include "../Widget/DressIconWidget.h"
#include "../Widget/PatternWidget.h"
#include "../Widget/STPageController.h"

class PhotoScene : public cocos2d::Layer,
public cocos2d::extension::TableViewDataSource
{
public:
    CREATE_SCENE_FUNC(PhotoScene);
    CREATE_FUNC(PhotoScene);
    
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    void setNeedShowFullScreenAds(bool need){_needShowFullScreenAds = need;};
    PhotoScene():_needShowFullScreenAds(false){};
    virtual ~PhotoScene();

protected:
    // from TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
protected:
    void homeAction();
    void deleteAction();

protected:
    PageControl *_pageControl;
    STPageController *_pageController;
    cocos2d::ui::Widget *_deleteButton;
    bool _needShowFullScreenAds = false;
};

#endif /* defined(__PaintDress__PhotoScene__) */
