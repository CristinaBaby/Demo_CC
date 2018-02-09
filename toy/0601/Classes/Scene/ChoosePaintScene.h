//
//  ChoosePaintScene.h
//  PaintDress
//
//  Created by maxiang on 6/25/15.
//
//

#ifndef __PaintDress__ChoosePaintScene__
#define __PaintDress__ChoosePaintScene__


#include "cocos2d.h"
#include "cocosGUI.h"
#include "cocos-ext.h"

#include "../Model/AppConfigs.h"
#include "../Model/ResManager.h"
#include "../Widget/PaintIconWidget.h"

class ChoosePaintScene : public cocos2d::Layer,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
public:
    static cocos2d::Scene* createScene(const ResType type);
    static ChoosePaintScene* create(const ResType type);
    bool init(const ResType type);
    
    ChoosePaintScene();
    virtual ~ChoosePaintScene();
    
    void refreshCoinLabel();
    virtual void onEnterTransitionDidFinish() override;

    void setNeedShowFullScreenAds(bool need){_needShowFullScreenAds = need;};
    
protected:
    void backAction();
    void shopAction();
    void doneAction();
    
    void selectTopAction();
    void selectBottomAction();
    void selectDressAction();
    void selectShoesAction();

    void initTableView();

    //table view protocol
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view){};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view){};
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
protected:
    cocos2d::extension::TableView *_topTableView;
    cocos2d::extension::TableView *_bottomTableView;
    cocos2d::extension::TableView *_dressTableView;
    cocos2d::extension::TableView *_shoesTableView;
    
    cocos2d::Sprite *_pictureImageView;
    cocos2d::Sprite *_pictureWhiteMask;

    
    Res *_selectRes;

    PaintIconWidget *_selectTopIcon;
    PaintIconWidget *_selectDressIcon;
    PaintIconWidget *_selectBottomIcon;
    PaintIconWidget *_selectShoesIcon;
    
    int _selectTopIndex;
    int _selectDressIndex;
    int _selectBottomIndex;
    int _selectShoesIndex;

    cocos2d::ui::Text *_coinLabel;
    cocos2d::ui::Widget *_doneButton;

    cocos2d::Size _cellSize;
    cocos2d::Touch *_beginTouch;
    cocos2d::Sprite *_guideArrow;
    
    bool _needShowFullScreenAds;
    bool _isDone;
};

#endif /* defined(__PaintDress__ChoosePaintScene__) */
