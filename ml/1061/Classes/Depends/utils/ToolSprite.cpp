//
//  ToolSprite.cpp
//  PetRescue_wangbosheng1
//
//  Created by wangbosheng1 on 14-8-15.
//
//

#include "ToolSprite.h"

ToolSprite::ToolSprite():m_bCanDragBack(false),m_bCancelEvent(false),m_bCanDrag(true),m_pDelegate(NULL),m_bIsMove(true)
{
    m_pIconTexture2D = NULL;
    m_pDragTexture2D = NULL;
}

ToolSprite::~ToolSprite()
{
}

ToolSprite* ToolSprite::createWithTexture(Texture2D *texture)
{
    ToolSprite *sprite = new (std::nothrow) ToolSprite();
    if (sprite && sprite->initWithTexture(texture) && sprite->initData())
    {
        sprite->autorelease();
        sprite->m_pDragTexture2D = texture;
        sprite->m_pIconTexture2D = texture;
        sprite->m_oDragRect.size = sprite->m_pDragTexture2D->getContentSize();
        sprite->m_oIconRect.size = sprite->m_pIconTexture2D->getContentSize();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}


ToolSprite* ToolSprite::create(const char* dragFileName, const char* iconFileName)
{
    ToolSprite *sprite = new (std::nothrow) ToolSprite();
    if (sprite && sprite->init(dragFileName, iconFileName))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool ToolSprite::init(const char* dragFileName, const char* iconFileName)
{
    bool bRet = false;
    do
    {
        if (!iconFileName) {
            iconFileName = dragFileName;
        }
        CC_BREAK_IF(!Sprite::initWithFile(iconFileName));
        this->setTextureByFileName(dragFileName, iconFileName);
        CC_BREAK_IF(!initData());
        
        bRet = true;
    } while (0);
    return bRet;
}

bool ToolSprite::initData()
{
//    m_vMoveRect = Rect(Director::getInstance()->getVisibleOrigin().x, Director::getInstance()->getVisibleOrigin().y + 250, Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
    m_vMoveRect = Rect::ZERO;
    m_rNoBackRect = Rect::ZERO;
    m_oOrginOffset = Vec2::ZERO;
    m_oResponsePoint = this->getAnchorPoint();
    
    return true;
}


void ToolSprite::setTextureByFileName(const char* dragFileName, const char* iconFileName)
{
    m_pDragTexture2D = Director::getInstance()->getTextureCache()->addImage(dragFileName);
    m_pIconTexture2D = Director::getInstance()->getTextureCache()->addImage(iconFileName);
    
    m_oDragRect.size = m_pDragTexture2D->getContentSize();
    m_oIconRect.size = m_pIconTexture2D->getContentSize();
    
//    this->setTexture(m_pIconTexture2D);
//    this->setTextureRect(m_oIconRect);
    
}

void ToolSprite::onEnter()
{
    Sprite::onEnter();
    
    // 计算出响应点距离锚点的距离
    Vec2 anchor = this->getAnchorPoint();
    Size size = this->getContentSize();
    m_oResponseDelta = Vec2(size.width * (m_oResponsePoint.x - anchor.x),
                           size.height * (m_oResponsePoint.y - anchor.y));
    
    Node* parent = this->getParent();
    if (parent)
    {
        Rect rect = RectApplyAffineTransform(Rect(0, 0, parent->getContentSize().width, parent->getContentSize().height),parent->nodeToWorldTransform());
        m_oOrginOffset = Vec2(rect.getMinX(),rect.getMinY());
    }
    
    // 设置回来的动画
    m_oOriginPoint = this->getPosition();
    
    EventListenerTouchOneByOne* e = EventListenerTouchOneByOne::create();
    e->onTouchBegan = CC_CALLBACK_2(ToolSprite::TouchBegan, this);
    e->onTouchMoved = CC_CALLBACK_2(ToolSprite::TouchMoved, this);
    e->onTouchEnded = CC_CALLBACK_2(ToolSprite::TouchEnded, this);
    e->onTouchCancelled = CC_CALLBACK_2(ToolSprite::TouchCancelled, this);
    e->setSwallowTouches(true);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
}

void ToolSprite::onExit()
{
    Sprite::onExit();
}

void ToolSprite::setResponsePoint(const Vec2& pos)
{
    // 默认设置响应点为锚点
    m_oResponsePoint = pos;
    // 计算出响应点距离锚点的距离
    Vec2 anchor = this->getAnchorPoint();
    Size size = this->getContentSize();
    m_oResponseDelta = Vec2(size.width * (m_oResponsePoint.x - anchor.x),
                           size.height * (m_oResponsePoint.y - anchor.y));
}

bool ToolSprite::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if (m_bCancelEvent) {
        return false;
    }
    
    if(!isVisible()){
        return false;
    
    }
    
    Vec2 touchPos = pTouch->getLocation();
    Vec2 realPos = this->getParent()->convertToNodeSpace(touchPos);
    if (boundingBox().containsPoint(realPos) && isVisible())
    {
//        this->setZOrder(this->getZOrder() + 1);
        if (m_pDelegate)
        {
            if (!m_bCanDrag) {
                m_pDelegate->onTouchCancel(this, pTouch);
            }else{
                m_pDelegate->onTouchDown(this, pTouch);
            }
        }
        // 改变图片
//        if (m_bCanDragBack || m_bCanDrag) {
//            this->setTexture(m_pDragTexture2D);
//            this->setTextureRect(m_oDragRect);
//        }
        return true;
    }
    return false;
}
void ToolSprite::setIsMove(bool b){

    m_bIsMove = b;

}
void ToolSprite::TouchMoved(Touch *pTouch, Event *pEvent)
{
    if (m_bCancelEvent || !m_bCanDrag) {
        return;
    }
    if (!m_bIsMove) {
        if (m_pDelegate)
        {
            m_pDelegate->onTouchMove(this, pTouch);
        }
        return;
    }
    
    
    Vec2 deltaPos = pTouch->getDelta();
    Vec2 vec = this->getPosition();
    // get current response point
    Vec2 newPos = this->getJudgePosition()+deltaPos;
    if(m_vMoveRect.equals(Rect::ZERO) || m_vMoveRect.containsPoint(newPos))
    {
        this->setPosition(this->getPosition() + deltaPos);
        // judge the sprite's response point in the rect
        int index = this->isInRect(newPos);
        if(index != -1)
        {
            if (m_pDelegate)
            {
                m_pDelegate->onTouchMoveInRect(this, pTouch, m_vRects[index], index);
            }
            
        }else{
            if (m_pDelegate)
            {
          
                m_pDelegate->onTouchMoveOutRect(this, pTouch);
            }
        }

    }else
    {
        if(newPos.x>m_vMoveRect.getMaxX())
        {
            this->setPositionX(m_vMoveRect.getMaxX()-m_oResponseDelta.x-m_oOrginOffset.x);
        }
        else
            if(newPos.x<m_vMoveRect.getMinX())
        {
            this->setPositionX(m_vMoveRect.getMinX()-m_oResponseDelta.x-m_oOrginOffset.x);
        }
        
        if(newPos.y>m_vMoveRect.getMaxY())
        {
            this->setPositionY(m_vMoveRect.getMaxY()-m_oResponseDelta.y-m_oOrginOffset.y);
        }
        else
            if(newPos.y<m_vMoveRect.getMinY())
        {
            this->setPositionY(m_vMoveRect.getMinY()-m_oResponseDelta.y-m_oOrginOffset.y);
        }

    }
    
    
//    log("------------------------------>%d",this->getTag());
    
    if (m_pDelegate)
    {
        m_pDelegate->onTouchMove(this, pTouch);
    }
}

void ToolSprite::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if (m_bCancelEvent) {
//        m_bCancelEvent = false;
        return;
    }
    if (!m_bCanDrag) {
        return;
    }
//    this->setZOrder(this->getZOrder() - 1);
    if (m_pDelegate)
    {
        m_pDelegate->onTouchUp(this, pTouch);
    }
    Vec2 newPos = this->getJudgePosition();
    int index = this->isInRect(newPos);
    if(index != -1 && m_pDelegate)
    {
        m_pDelegate->onTouchUpInRect(this, pTouch, m_vRects[index], index);
    }else if(m_pDelegate)
    {
        m_pDelegate->onTouchUpOutRect(this, pTouch);
    }
    if(m_rNoBackRect.equals(Rect::ZERO) || !m_rNoBackRect.containsPoint(newPos))
    {
        this->moveBack();
    }
}

void ToolSprite::TouchCancelled(Touch *pTouch, Event *pEvent)
{
    this->TouchEnded(pTouch, pEvent);
}

void ToolSprite::addRect(const Rect& rect, Node* root)
{
    if (!this->isContain(rect))
    {
        if (root) {
            LayerColor* test = LayerColor::create(Color4B(255, 0, 0, 100));
            test->setAnchorPoint(Vec2(0, 0));
            test->setContentSize(Size(rect.getMaxX() - rect.getMinX(), rect.getMaxY() - rect.getMinY()));
            test->setPosition(Vec2(rect.getMinX(), rect.getMinY()));
            root->addChild(test,100);
        }

        m_vRects.push_back(rect);
    }
}

void ToolSprite::addRectByNode(Node* node, Node* root)
{
    if (!node->getParent())
    {
        log("this node has not been added to parent,rect maybe incorrect!");
    }
    Rect rect = RectApplyAffineTransform(Rect(0, 0, node->getContentSize().width, node->getContentSize().height),node->nodeToWorldTransform());
    log("====================>minX:%f  minY:%f  maxX:%f    maxY:%f", rect.getMinX(), rect.getMinY(), rect.getMaxX(), rect.getMaxY());
    if (root) {
        LayerColor* test = LayerColor::create(Color4B(255, 0, 0, 100));
        test->setAnchorPoint(Vec2(0, 0));
        test->setContentSize(Size(rect.getMaxX() - rect.getMinX(), rect.getMaxY() - rect.getMinY()));
        test->setPosition(Vec2(rect.getMinX(), rect.getMinY()));
        root->addChild(test);
    }
    this->addRect(rect);
}

bool ToolSprite::isContain(const Rect& rect)
{
    return false;
}

int ToolSprite::isInRect(const Vec2& pos)
{
    for (int i = 0; i < m_vRects.size(); i++)
    {
        if (m_vRects[i].containsPoint(pos))
        {
            return i;
        }
    }
    return -1;
}
void ToolSprite::removeRectByID(int id)
{
    
}


Vec2 ToolSprite::getJudgePosition()
{
    Vec2 nowPos = this->getPosition();
    return (nowPos + m_oResponseDelta) + m_oOrginOffset;
}

void ToolSprite::setDelegate(ToolSpriteDelegate* delegate)
{
    m_pDelegate = delegate;
}

void ToolSprite::setDrag(const bool canDrag)
{
    m_bCanDrag = canDrag;
}

void ToolSprite::cancelEvent(const bool cancelEvent)
{
    m_bCancelEvent = cancelEvent;
}

void ToolSprite::setDragBack(const bool canDragBack)
{
    m_bCanDragBack = canDragBack;
}

void ToolSprite::setOriginalPoint(Vec2 vec)
{
    m_oOriginPoint=vec;
}

void ToolSprite::moveBack()
{
    if (!m_bCanDragBack||m_bCancelEvent) {
        return;
    }
    m_bCancelEvent = true;
    MoveTo* m_pMoveTo = MoveTo::create(0.3f, m_oOriginPoint);
//    Spawn* m_pSpawn = Spawn::create(m_pMoveTo, NULL);
    Sequence* m_pSeq = Sequence::create(m_pMoveTo, CallFuncN::create(CC_CALLBACK_1(ToolSprite::moveOver, this)), nullptr);
    this->runAction(m_pSeq);
}

void ToolSprite::moveOver(Ref* pSender)
{
    this->setScale(1.0f);
//    return;
    this->setTexture(m_pIconTexture2D);
    this->setTextureRect(m_oIconRect);
    m_bCancelEvent = false;
}

void ToolSprite::setMoveRectByNode(Node* node)
{
    m_vMoveRect = RectApplyAffineTransform(Rect(0, 0, node->getContentSize().width, node->getContentSize().height),node->nodeToWorldTransform());
    m_vMoveRect.setRect(m_vMoveRect.origin.x,m_vMoveRect.origin.y,m_vMoveRect.size.width,m_vMoveRect.size.height);

}

void ToolSprite::setMoveRect(const Rect& rect)
{
    m_vMoveRect = rect;
}
void ToolSprite::clearRectVector()
{

    m_vRects.clear();
    
}

void ToolSprite::setNoBackRectByNode(Node* node)
{
    m_rNoBackRect = RectApplyAffineTransform(Rect(0, 0, node->getContentSize().width, node->getContentSize().height),node->nodeToWorldTransform());
}


