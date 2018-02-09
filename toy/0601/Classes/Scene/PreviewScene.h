//
//  PreviewScene.h
//  PaintDress
//
//  Created by maxiang on 6/30/15.
//
//

#ifndef __PaintDress__PreviewScene__
#define __PaintDress__PreviewScene__

#include "cocos2d.h"
#include "cocosGUI.h"
#include "cocos-ext.h"
#include "../Model/AppConfigs.h"
#include "../Widget/DressIconWidget.h"
#include "../Widget/PatternWidget.h"
#include "../Widget/STPageController.h"
#include "../Model/ResManager.h"

class PreviewScene : public cocos2d::Layer,
public cocos2d::extension::TableViewDataSource
{
public:
    static cocos2d::Scene* createScene(const Model* model);
    static PreviewScene* create(const Model* model);
    virtual bool init(const Model* model);
    PreviewScene();
    virtual ~PreviewScene();
    virtual void onEnterTransitionDidFinish() override;

protected:
    // from TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
protected:
    void homeAction();
    void takePhotoAction();
    void closeAction();
    void albumAction();
    void getScreenShot();
    
protected:
    cocos2d::Layer *_photoLayer;
    cocos2d::extension::Scale9Sprite *_photoFrame;
    cocos2d::ui::ImageView *_backgroundImageView;
    cocos2d::ui::Widget *_photoFrameContainer;
    cocos2d::Image *_photoImage;
    bool _isDone = false;

    STPageController *_pageController;
};



#endif /* defined(__PaintDress__PreviewScene__) */
