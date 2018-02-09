//
//  JigsawPuzzleScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__JigsawPuzzleScene__
#define __BedtimeStory__JigsawPuzzleScene__

#include "BaseScene.h"
#include "JigsawTileSprite.h"
#include "STInAppBilling.h"

class JigsawPuzzleScene : public BaseScene,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate,
public STIABDelegate
{
public:
    JigsawPuzzleScene();
    virtual ~JigsawPuzzleScene();

    virtual bool init() override;

    void startGameWithGameLevel(const int level);
protected:
    
    void setupJigsawTiles(const int level);
    
    //end moving tile sprite
    void moveTableTileSprite(cocos2d::Touch* touch);
    void endTouchTableTileSprite(cocos2d::Touch* touch);
    
    void moveContainerTileSprite(cocos2d::Touch* touch);
    void endTouchContainerTileSprite(cocos2d::Touch* touch);
    
    bool adaptTileSpriteOriginalPosition(JigsawTileSprite *tileSprite, const cocos2d::Point& point);
    bool adaptTileSpriteRelativePosition(JigsawTileSprite *tileSprite, const cocos2d::Point& point);
    
    //check linked child
    void findLinkedChildren(JigsawTileSprite* tileSprite);
    void recursiveFindLinkedTileSprite(std::vector<JigsawTileSprite*>& linkedTileVector, JigsawTileSprite *tileSprite);

    void restoreTouchState();
    
    void timerUpdate(float time);
    
    void showParticle(const cocos2d::Point& position);
    
    void finishGame();
    
    void showHint();
    
    //show quit layer
    void showQuitGameLayer();
    
    //table view protocol
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    virtual void tableCellWillRecycle(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;

    /* register touch event */
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid) override;
    virtual void purchaseFailed(const char *pid, int errorCode) override;
    virtual void restoreSuccessful(const char* pid) override;
    virtual void restoreFailed(const char* pid, int errorCode) override;
private:
    JigsawGameData                     *_gameData;
    cocos2d::extension::TableView      *_tableView;
    cocos2d::Layer                     *_gameContentLayer;

    cocos2d::ui::ImageView             *_tileSpriteContainer;
    cocos2d::ui::Text                  *_timeLabel;
    cocos2d::ui::Button                *_hintButton;

    cocos2d::Vector<JigsawTileSprite*>  _tileSpritesVector;
    cocos2d::Vector<JigsawTileSprite*>  _containerTileSpritesVector;

    cocos2d::Vector<cocos2d::extension::TableViewCell*>    _visibleCells;

    JigsawTileSprite  *_touchedTableTileSprite;
    JigsawTileSprite  *_touchedContainerTileSprite;

    cocos2d::Point   _tableTileSpriteOriginalPosition;
    cocos2d::Point   _tableCellOriginalPosition;
    cocos2d::Point   _tableOriginalOffset;
    
    STInAppBilling _iap;
    void showPurchaseDialog();

    int     _gameLevel;
    float   _jigsawToResScale;
    float   _jigsawToContainerScale;

    //check if touched in table view tiles
    bool    _isTouchInTableTileSprite;
    
    //check if touched in container tiles
    bool    _isTouchInContainerTileSprite;
    
    //when draging a tile from table view
    bool    _isMovingTableTileSprite;
    
    //when draging a tile from container
    bool    _isMovingContainerTileSprite;
    
    //when scrolling table view
    bool    _isScrollingTableView;

    //is is animating
    bool    _isAnimating;
    
    //the matrix row
    int     _matrixRow;
    
    //the fixed tile sprite
    int     _fixedTileSpriteCount;

    int     _usedTime;
    bool    _pause;
};

#endif /* defined(__BedtimeStory__JigsawPuzzleScene__) */
