//
//  STPageController.h
//  SpotDifference
//
//  Created by Steven.Xc.Tian.
//
//

#ifndef __SpotDifference__STPageController__
#define __SpotDifference__STPageController__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Widget/CBPageControl.h"
#include "../Widget/CBPackPage.h"

class STIndicator;
class STPage;

class STPageController :
public cocos2d::extension::TableView,
public cocos2d::extension::TableViewDelegate
{
public:
    enum class TurnPageDirection
    {
        RIGHT, LEFT,
    };
    
    
public:
    static STPageController* create(cocos2d::extension::TableViewDataSource* dataSource, cocos2d::Size size);
    
    static STPageController* create(cocos2d::extension::TableViewDataSource* dataSource, cocos2d::Size size, Node *container);
    
    void reloadData();
    
public:
    void turnPage(TurnPageDirection dire);
    
    void turnToPage(unsigned pageIndex);
    void turnToPage(unsigned pageIndex, bool animation);

    const unsigned getFocusPageIndex()
    {
        return _mFocusPageIndex;
    }
    
    void setIndicator(STIndicator* indicator);
    void setPageControl(colorbook::PageControl* pageControl);
    
    void setTouchMoved(bool moved);
    void backToOriginal();

protected:
    virtual bool init(cocos2d::extension::TableViewDataSource* dataSource, cocos2d::Size size, Node *container);
    
    STPageController() :
     _mpIndicator(nullptr), _mpPageCell(nullptr),
    _mFocusPageIndex(0),_mStartPoint(0, 0)
    {
    }
    
public: // from TableView
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
    virtual void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
    
protected: // from TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual void tableCellHighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual void tableCellUnhighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual void tableCellWillRecycle(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
protected:
    void relocateContainer();
    
protected:
    unsigned _mFocusPageIndex;
    STIndicator  *_mpIndicator;
    cocos2d::Point _mStartPoint;
    STPage *_mpPageCell;
    
    //MX
    colorbook::PageControl *_pageControl;
    colorbook::Page *_selectPage;
};




class STIndicator   : public cocos2d::LayerColor
{
protected:
    cocos2d::SpriteFrame *mp_NormalFrame;
    cocos2d::SpriteFrame *mp_SelectedFrame;
    
    unsigned mPreFocusPosition;
    cocos2d::Vector<cocos2d::Sprite *> _mDots;
    
public:
    static STIndicator* create(const unsigned capacity, cocos2d::SpriteFrame *normalFrame, cocos2d::SpriteFrame *focusFrame);
    static STIndicator* create(const unsigned capacity, std::string normalSpriteFrameName, std::string focusSpriteFrameName);
    
    void onTurnPage(unsigned curPageIndex);
    
protected:
    virtual bool init(const unsigned capacity, cocos2d::SpriteFrame *normalFrame, cocos2d::SpriteFrame *focusFrame);
    virtual bool init(const unsigned capacity, std::string normalSpriteFrameName, std::string focusSpriteFrameName);
    
    ~STIndicator();
};






class STPage : public cocos2d::extension::TableViewCell
{
protected:
    cocos2d::LayerColor* _backgroundLayer;
    
public:
    
    
    static STPage * create(const cocos2d::Size &size);
    /** creates a CCLayer with color. Width and height are the window size. */
    static STPage * create(const cocos2d::Color4B& color, const cocos2d::Size &size);
    
    
    
protected:
    STPage(){};
    
    bool init(const cocos2d::Size &size);
    bool initWithColor(const cocos2d::Color4B& color, const cocos2d::Size &size);
    
public:
    virtual void onClickedPage(cocos2d::Touch *pTouch){};
};

#endif /* defined(__SpotDifference__STPageController__) */
