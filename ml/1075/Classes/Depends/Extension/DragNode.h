
#ifndef __DragNode__
#define __DragNode__


#include "cocos2d.h"

USING_NS_CC;

class DragNode;

class DragNodeTouchDelegate {
    
    
public:
    virtual void dragNodeTouchBegin(DragNode* dragNode,Point worldPoint) {};
    virtual void dragNodeTouchMoved(DragNode* dragNode,Point worldPoint) {};
    virtual void dragNodeTouchEnded(DragNode* dragNode,Point worldPoint) {};
    virtual void dragNodeClicked(DragNode* dragNode,Point worldPoint) {};
};

class DragNode : public Node
{
public:
    enum class Direction
    {
        NONE,
        VERTICAL,
        HORIZONTAL,
    };
    CREATE_FUNC(DragNode);
    
    static DragNode* create(const std::string name);
    static DragNode* createWithSpriteFrameName(const std::string name);
    static DragNode* create(Texture2D* texture);
    
    
    virtual bool init();
    virtual bool init(const std::string name);
    virtual bool initWithSpriteFrameName(const std::string name);
    virtual bool initWithTexture(Texture2D* texture);
    virtual void onEnter();
    virtual void onExit();
    
    std::function<void(DragNode* pDragNode,Point worldPoint)> dragNodeTouchBegin;
    std::function<void(DragNode* pDragNode,Point worldPoint)> dragNodeTouchMoved;
    std::function<void(DragNode* pDragNode,Point worldPoint)> dragNodeTouchEnded;
    std::function<void(DragNode* pDragNode,Point worldPoint)> dragNodeClicked;
    
    CC_PROPERTY(bool, m_bTouchEnable, TouchEnabled);
    CC_PROPERTY(bool, m_bMovable, Movable);
    CC_PROPERTY(int, m_nTouchPriority, TouchPriority);
    CC_PROPERTY(Point, m_cActionPoint, ActionPoint); //相对于sprite的中心
    CC_PROPERTY(Rect, m_cLimitRect, LimitRect);  //世界坐标
    CC_PROPERTY(Point, m_cOrgPosition, OrgPosition);
    CC_PROPERTY(Sprite*, m_pSprite, DragSprite);
    
    void setOrgPositionDefault();
    Point getOffset(){return m_cOffset;};
    
    void setDelegate(DragNodeTouchDelegate* delegate){m_pDelegate = delegate;}
    
    
    void back(int zorder=0,const std::function<void()>& callback = nullptr){
        this->stopAllActions();
        this->runAction(Sequence::create(MoveTo::create(0.5, m_cOrgPosition),
                                         CallFunc::create([=](){
            this->setLocalZOrder(zorder);
            if (callback) {
                callback();
            }
        }),
                                         NULL));
    }
    
    bool isMoving(){return m_bIsMoving;}
    
    virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void ccTouchCancelled(Touch *pTouch, Event *pEvent);
    
    void setDirection(Direction direction){m_eDiretion = direction;}
    void setCanSwallow(bool swallow){m_bCanSwallow = swallow;}
    Point getWorldSpaceActionPoint();
    int index;
    int flag;
    bool ignoreMoving;
private:
    DragNode():m_bTouchEnable(true),m_bMovable(true),ignoreMoving(false),m_nTouchPriority(0),m_cActionPoint(Point::ZERO),m_cLimitRect(Rect::ZERO),m_cOrgPosition(Point::ZERO),m_pSprite(nullptr){
        m_bMovable = true;
        m_bCanSwallow = true;
        m_eDiretion = Direction::NONE;};
    ~DragNode(){};
    
protected:
    Point m_cBeginPoint;
    Point m_cOffset;
    bool m_bIsMoved;
    bool m_bIsMoving;
    bool m_bIsStart;
    bool m_bCanSwallow; //新加 为解决加了direction后 不吞噬下面层的事件（如scrollview的滑动事件）
    
    Direction m_eDiretion; //新加 判断拖动方向（如果设置了拖动方向，最开始沿其他方向是不能拖动的，解决在scrollview中将组件拖出效果）
    Direction m_eCurDiretion;
    
    DragNodeTouchDelegate* m_pDelegate;
    
    EventListenerTouchOneByOne* m_pListener;
};

#endif