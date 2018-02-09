//
//  PaintScene.h
//  PaintDress
//
//  Created by maxiang on 6/16/15.
//
//

#ifndef __PaintDress__PaintScene__
#define __PaintDress__PaintScene__

#include "cocos2d.h"
#include "cocosGUI.h"
#include "cocos-ext.h"
#include "../Model/v2.0/ScribblePatternNode.h"
#include "../Model/v2.0/ScribbleUtil.h"
#include "../Model/AppConfigs.h"
#include "../Widget/ColorWidget.h"
#include "../Widget/PatternWidget.h"
#include "../Model/ResManager.h"

class PaintScene : public cocos2d::Layer,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
public:
    static cocos2d::Scene* createScene(const Res& res);
    static PaintScene* create(const Res& res);
    bool init(const Res& res);
    
    PaintScene();
    virtual ~PaintScene();

protected:
    void backAction();
    void doneAction();
    void eraserAction();
    void selectPenAction();
    void selectPatternAction();
    
    void initTableView();
    bool setupScribble();
    void onSaveSuccess();
    
    /* register touch event */
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view){};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view){};
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
protected:
    cocos2d::extension::TableView *_penTableView;
    cocos2d::extension::TableView *_patternTableView;
    cocos2d::ui::Button *_eraserButton;

    BasePenWidget *_selectColorPenWidget;
    BasePenWidget *_selectPatternPenWidget;

    cocos2d::Touch *_beginTouch;
    cocos2d::LayerColor *_paintingLayer;

    /* scribble */
    Sprite *_resPicture;
    ScribblePatternNode *_pictureScribble;
    Image *_resPicturePixelMask;
    
    int _selectPenIndex;
    int _selectPatternIndex;

    Res _res;
    
    bool _isColouring;
    bool _isSelectEraser;
    bool _isDone = false;

    cocos2d::CustomCommand _callbackCommand;
    cocos2d::BlendFunc _blendFunc;
};


#endif /* defined(__PaintDress__PaintScene__) */
