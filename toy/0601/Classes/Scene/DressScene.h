//
//  DressScene.h
//  PaintDress
//
//  Created by maxiang on 6/26/15.
//
//

#ifndef __PaintDress__DressScene__
#define __PaintDress__DressScene__


#include "cocos2d.h"
#include "cocosGUI.h"
#include "cocos-ext.h"
#include "../Model/AppConfigs.h"
#include "../Widget/DressIconWidget.h"
#include "../Widget/PatternWidget.h"
#include "../Layer/ModelLayer.h"

class DressScene : public cocos2d::Layer,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
public:
    static cocos2d::Scene* createScene(const ResType type);
    static DressScene* create(const ResType type);
    bool init(const ResType type);
    
    virtual void onEnterTransitionDidFinish() override;
    virtual ~DressScene();

    DressScene();
    void setNeedShowFullScreenAds(bool need){_needShowFullScreenAds = need;};

protected:
    void homeAction();
    void nextAction();
    
    void selectHairAction();
    void selectBottomAction();
    void selectDressAction();
    void selectShoesAction();
    void selectTopAction();
    void selectAccessoryAction();

    void initTableView();
    
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view){};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view){};
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
protected:
    cocos2d::extension::TableView *_hairTableView;
    cocos2d::extension::TableView *_bottomTableView;
    cocos2d::extension::TableView *_dressTableView;
    cocos2d::extension::TableView *_shoesTableView;
    cocos2d::extension::TableView *_topTableView;
    cocos2d::extension::TableView *_accessoryTableView;

    cocos2d::ui::ImageView *_dressBackground;
    cocos2d::Touch *_beginTouch;
    BaseIconWidget *_selectIconWidget;
    ModelLayer *_modelLayer;
    ResType _currentType;
    bool _needShowFullScreenAds;
    bool _isDone = false;

    Res _res;
};


#endif /* defined(__PaintDress__DressScene__) */
