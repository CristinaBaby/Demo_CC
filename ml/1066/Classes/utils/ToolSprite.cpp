//
//  ToolSprite.cpp
//  PetRescue_wangbosheng1
//
//  Created by wangbosheng1 on 14-8-15.
//
//

#include "ToolSprite.h"
#include "AdapterScreen.h"

ToolSprite::ToolSprite():m_bCanDragBack(false),m_bCancelEvent(false),m_bCanDrag(false),m_pDelegate(nullptr),m_bIsScale(false),m_bIsShade(false),m_bIsSwallowTouch(true),m_bIsNeedDragBack(false),m_eDir(eNONE),m_iFlag(-1),m_bIsAllowTouch(true),m_bIsPriority(false),m_pListerner(nullptr),m_fDelay(0)
{
    m_pIconTexture2D = nullptr;
    m_pDragTexture2D = nullptr;
}

ToolSprite::~ToolSprite()
{
    log("%s", __FUNCTION__);

    if (m_bIsPriority) {
        this->_eventDispatcher->removeEventListener(m_pListerner);
    }
}

ToolSprite* ToolSprite::create(const char* dragFileName, const char* iconFileName)
{
    ToolSprite* bRet = new ToolSprite();
    
    if (bRet && bRet->init(dragFileName, iconFileName))
    {
        bRet->autorelease();
        return bRet;
    }
    
    CC_SAFE_RELEASE_NULL(bRet);
    
    return nullptr;
}

ToolSprite* ToolSprite::createWithTexture(Texture2D *texture)
{
    ToolSprite *sprite = new (std::nothrow) ToolSprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool ToolSprite::initWithTexture(Texture2D* texture)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Sprite::initWithTexture(texture));
        
        m_vMoveRect = Rect::ZERO;
        m_oOrginOffset = Vec2::ZERO;
        m_oResponsePoint = this->getAnchorPoint();
        
        m_oDragRect.size = texture->getContentSize();
        
        m_oToolSIze = getContentSize();
        
        bRet = true;
    }while (0);
    return bRet;
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
        m_vMoveRect = Rect::ZERO;
        m_oOrginOffset = Vec2::ZERO;
        m_oResponsePoint = this->getAnchorPoint();
        
        this->setTextureByFileName(dragFileName, iconFileName);
        
        m_oToolSIze = getContentSize();

        
        bRet = true;
    } while (0);
    return bRet;
}

void ToolSprite::setTextureByFileName(const char* dragFileName, const char* iconFileName)
{
    m_pDragTexture2D = Director::getInstance()->getTextureCache()->addImage(dragFileName);
    m_pIconTexture2D = Director::getInstance()->getTextureCache()->addImage(iconFileName);
    
    m_oDragRect.size = m_pDragTexture2D->getContentSize();
    
    m_oIconRect.size = m_pIconTexture2D->getContentSize();
    
}

void ToolSprite::setRegisterType(bool b)
{
    m_bIsPriority = b;
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
        
        m_oOrginOffset = -kAdapterScreen->g_oOffset;
//        m_oOrginOffset = Vec2(rect.getMinX(),rect.getMinY());
        log("m_oOrginOffset.x==========%f", m_oOrginOffset.x);
        log("m_oOrginOffset.y==========%f", m_oOrginOffset.y);
    }
    
    // 设置回来的动画
    m_oOriginPoint = this->getPosition();
    
    m_pListerner = EventListenerTouchOneByOne::create();
    
    m_pListerner->onTouchBegan = CC_CALLBACK_2(ToolSprite::TouchBegan, this);
    m_pListerner->onTouchMoved = CC_CALLBACK_2(ToolSprite::TouchMoved, this);
    m_pListerner->onTouchEnded = CC_CALLBACK_2(ToolSprite::TouchEnded, this);
    m_pListerner->onTouchCancelled = CC_CALLBACK_2(ToolSprite::TouchCancelled, this);
    m_pListerner->setSwallowTouches(m_bIsSwallowTouch);
    
    if (!m_bIsPriority) {
        this->_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListerner, this);
    }else{
        this->_eventDispatcher->addEventListenerWithFixedPriority(m_pListerner, 1);
    }
}

void ToolSprite::setOriginPoint(Point point)
{
    m_oOriginPoint = point;
}

Point ToolSprite::getOriginPoint()
{
    return m_oOriginPoint;
}

void ToolSprite::onExit()
{
    Sprite::onExit();
}

void ToolSprite::setSwallowTouch(bool isSwallow)
{

    m_bIsSwallowTouch = isSwallow;
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
    if (!m_bIsAllowTouch)
        return false;
    
    if (m_bCancelEvent)
        return false;
    
    Vec2 touchPos = pTouch->getLocation();
    
    Vec2 realPos = this->getParent()->convertToNodeSpace(touchPos);

    // get boundingBox
    Vec2 an = getAnchorPoint();
    Size conten = Size(m_oToolSIze.width * an.x, m_oToolSIze.height * an.y);
    Vec2 inParentOrn = getPosition() - conten;
    Rect r = Rect(inParentOrn.x, inParentOrn.y, m_oToolSIze.width, m_oToolSIze.height);
    
    if (r.containsPoint(realPos) && isVisible())
    {
        if (m_pDelegate)
        {
            if (!m_bCanDrag)
            {
                m_pDelegate->onTouchCancel(this, pTouch);
            }
            
            if (m_bIsScale)
                this->setScale(1.1f);
            
            if (m_bIsShade)
                this->setColor(Color3B(this->getColor().r / 2, this->getColor().g / 2, this->getColor().b / 2));
            
            m_pDelegate->onTouchDown(this, pTouch);

            return true;
        }
        return false;
    }
    return false;
}

void ToolSprite::TouchMoved(Touch *pTouch, Event *pEvent)
{
    if (m_bCancelEvent || !m_bCanDrag) {
        
        return;
    }
    
    Vec2 deltaPos = pTouch->getDelta();
    
//    if(fabs(deltaPos.x) > 5 || fabs(deltaPos.y) > 5)
//    {
        if (m_pDelegate)
            m_pDelegate->onTouchMove(this, pTouch);
//    }

    // get current response point
    Vec2 newPos = this->getJudgePosition() + deltaPos;
    
    if(m_vMoveRect.equals(Rect::ZERO) || m_vMoveRect.containsPoint(newPos))
    {
        switch (m_eDir)
        {
            case eHORIZONTAL:
            {
                if (fabs(this->getPositionX() + deltaPos.x - m_oOriginPoint.x) < 60) {
                    this->setPosition(this->getPositionX() + deltaPos.x, this->getPositionY());
                }
            }
                break;
            case eVERTICAL:
            {
                if ((this->getPositionY() + deltaPos.y - m_oOriginPoint.y >= -80) && (this->getPositionY() + deltaPos.y - m_oOriginPoint.y <= 200)) {
                    this->setPosition(this->getPositionX(), this->getPositionY() + deltaPos.y);
                }
                
            }
                break;
            case eNONE:
            {
                this->setPosition(this->getPosition() + deltaPos);
                
                // judge the sprite's response point in the rect
                int index = this->isInRect(newPos);
                
                if(index != -1)
                {
                    if (m_pDelegate)
                    {
//                        m_bCanDragBack = m_bIsNeedDragBack;
                        m_pDelegate->onTouchMoveInRect(this, pTouch, m_vRects[index], index);
                    }
                }else{
                    if (m_pDelegate)
                    {
                        m_pDelegate->onTouchMoveOutRect(this, pTouch);
                        return;
                    }
                }
            }
                break;
        }

    }
    else
    {
        if(newPos.x >= m_vMoveRect.getMaxX())
        {
            this->setPositionX(m_vMoveRect.getMaxX() - m_oResponseDelta.x - m_oOrginOffset.x);
        }
        else if(newPos.x <= m_vMoveRect.getMinX())
        {
            this->setPositionX(m_vMoveRect.getMinX() - m_oResponseDelta.x - m_oOrginOffset.x);
        }
        
        
        if(newPos.y >= m_vMoveRect.getMaxY())
        {
            this->setPositionY(m_vMoveRect.getMaxY() - m_oResponseDelta.y - m_oOrginOffset.y);

        }
        else if(newPos.y <= m_vMoveRect.getMinY())
        {
            this->setPositionY(m_vMoveRect.getMinY() - m_oResponseDelta.y - m_oOrginOffset.y);
        }
        return;
    }
}

void ToolSprite::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if (m_pDelegate)
    {
        if (m_bIsScale)
            this->setScale(1.0f);
        
        if (m_bIsShade)
            this->setColor(Color3B(this->getColor().r * 2, this->getColor().g * 2, this->getColor().b * 2));
        
        m_pDelegate->onTouchUp(this, pTouch);
        if (!m_bCanDrag) {
            Rect limit = Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
            if(limit.containsPoint(this->convertToNodeSpace(pTouch->getLocation())))
            {
                m_pDelegate->onTouchUpInBoundingBox(this, pTouch);
            }
        }
    }
    
    if (m_bCancelEvent) {
        return;
    }
    
    if (!m_bCanDrag) {
        return;
    }
    
    Vec2 newPos = this->getJudgePosition();
    
    int index = this->isInRect(newPos);
    
    if(index != -1 && m_pDelegate)
    {
        m_bCanDragBack = m_bIsNeedDragBack;
        
        m_pDelegate->onTouchUpInRect(this, pTouch, m_vRects[index], index);
        
    }
    else if(m_pDelegate)
    {
        m_pDelegate->onTouchUpOutRect(this, pTouch);
    }

    if(m_bCanDragBack)
    {
        this->moveBack();
    }
}

void ToolSprite::TouchCancelled(Touch *pTouch, Event *pEvent)
{
    this->TouchEnded(pTouch, pEvent);
}

void ToolSprite::addRect(const Rect& rect)
{
    if (!this->isContain(rect))
    {
        m_vRects.push_back(rect);
    }
}

void ToolSprite::addRectByNode(Node* node, float originOffsetX, float originOffsetY, float widthOffset, float heightOffset, Node* root)
{
    if (!node->getParent())
    {
        log("this node has not been added to parent,rect maybe incorrect!");
    }
    
    Rect rect = RectApplyAffineTransform(Rect(0, 0, node->getContentSize().width, node->getContentSize().height),node->nodeToWorldTransform());
    rect.setRect(rect.origin.x + originOffsetX - kAdapterScreen->g_oOffset.x, rect.origin.y + originOffsetY - kAdapterScreen->g_oOffset.y, rect.size.width + widthOffset, rect.size.height + heightOffset);
    
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

void ToolSprite::clearRect()
{
    m_vRects.clear();
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
    if (!m_bCanDragBack) {
        return;
    }
    
    m_bCancelEvent = true;

    MoveTo* m_pMoveTo = MoveTo::create((this->getPosition() - m_oOriginPoint).length() / 1500 + m_fDelay, m_oOriginPoint);
    
    Sequence* m_pSeq = Sequence::create(m_pMoveTo, CallFuncN::create(CC_CALLBACK_1(ToolSprite::moveOver, this)), nullptr);
    
    this->runAction(m_pSeq);
}

void ToolSprite::moveOver(Ref* pSender)
{
//    this->setScale(1.0f);

//    this->setTexture(m_pIconTexture2D);
//    
//    this->setTextureRect(m_oIconRect);
    
    m_bCancelEvent = false;
    
    m_pDelegate->onMoveBackCallback(this);
    
    m_bIsAllowTouch = true;
}

void ToolSprite::setAllowTouch(bool isAllow)
{
    m_bIsAllowTouch = isAllow;
}

void ToolSprite::setMoveRectByNode(Node* node, float originOffsetX, float originOffsetY, float widthOffset, float heightOffset, bool isFlag)
{
    m_vMoveRect = RectApplyAffineTransform(Rect(0, 0, node->getContentSize().width, node->getContentSize().height),node->nodeToWorldTransform());
    
    if (isFlag) {
        m_vMoveRect.setRect(m_vMoveRect.origin.x + originOffsetX - kAdapterScreen->g_oOffset.x, m_vMoveRect.origin.y + originOffsetY - kAdapterScreen->g_oOffset.y, m_vMoveRect.size.width + widthOffset, m_vMoveRect.size.height + heightOffset);
    }else{
        m_vMoveRect.setRect(m_vMoveRect.origin.x + originOffsetX - kAdapterScreen->g_oOffset.x, m_vMoveRect.origin.y + originOffsetY - kAdapterScreen->g_oOffset.y, m_vMoveRect.size.width + widthOffset + 2 * kAdapterScreen->g_oOffset.x, m_vMoveRect.size.height + heightOffset + 2 * kAdapterScreen->g_oOffset.y);
    }
}

void ToolSprite::setScaleBtn(bool isScale)
{
    m_bIsScale = isScale;
}

void ToolSprite::setShadeBtn(bool isShade)
{
    m_bIsShade = isShade;
}

void ToolSprite::setNeedDragBack(bool isNeed)
{
    m_bIsNeedDragBack = isNeed;
}

void ToolSprite::setLimitDirection(eDirection dir)
{
    m_eDir = dir;
}

void ToolSprite::setFlag(int flag)
{
    m_iFlag = flag;
}

int ToolSprite::getFlag()
{
    return m_iFlag;
}

