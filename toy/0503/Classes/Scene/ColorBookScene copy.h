//
//  ColorBookScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__ColorBookScene__
#define __BedtimeStory__ColorBookScene__

#include "BaseScene.h"
#include "ScribblePatternNode.h"
#include "ColorPenSprite.h"
#include "PatternPenSprite.h"

class ColorBookScene : public BaseScene,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
public:
    CREATE_SCENE_FUNC(ColorBookScene);
    CREATE_FUNC(ColorBookScene);
    
    void startGameWithGameLevel(const int level);
    
protected:
    virtual bool init() override;
    ColorBookScene();
    virtual ~ColorBookScene();

    void finishGame();
    
    //scribble
    bool setupScribble();
    
    //ui
    bool setupUI();

    //show quit layer
    void showQuitGameLayer();
    
    //button actions
    void penButtonAction(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    void patternButtonAction(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    void deleteButtonAction(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    void eraserButtonAction(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    
    //select pen
    void selectColorPen(const int index);
    void selectPatternPen(const int index);

    //table view protocol
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    virtual void tableCellWillRecycle(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    /* register touch event */
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
private:
    cocos2d::ProgressTimer              *_loadProgressTimer;
    cocos2d::ui::ListView               *_pensListView;
    cocos2d::ui::ListView               *_patternsListView;

    cocos2d::ui::Widget                 *_ui;
    cocos2d::ui::Button                 *_penButton;
    cocos2d::ui::Button                 *_patternButton;
    cocos2d::ui::Button                 *_deleteButton;
    cocos2d::ui::Button                 *_eraserButton;

    /* container layers */
    cocos2d::Layer *_pensContainerLayer;
    cocos2d::Layer *_pens1Layer;
    cocos2d::Layer *_pens2Layer;
    cocos2d::Layer *_currentVisbleLayer;
    cocos2d::Layer *_patternLayer;
    cocos2d::Layer *_colouringLayer;
    
    /* the current select pen or pattern */
    ColorPenSprite      *_selectColorPen;
    PatternPenSprite    *_selectPatternPen;
    
    int  _gameLevel;
    
    bool _finishLoadPicture;
    bool _finishLoadMask;
    bool _finishLoadHistory;
    
    bool _isColouring;
    
    /* scribble */
    Sprite *_picture;
    ScribblePatternNode *_pictureScribble;
    Image *_picturePixelMask;
};



#endif /* defined(__BedtimeStory__ColorBookScene__) */
