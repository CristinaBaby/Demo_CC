
#include "DecorationUtil.h"

DecorationUtil::DecorationUtil()
{
    utilDeletFuncCallback = nullptr;
    m_bFrameShowing = true;
    m_LimitRect = Rect::ZERO;
    m_bIsShotScreen = true;
    m_bIgnoreTip = false;
    m_bSwallow = true;
    m_bTouchEnable = true;
    m_bIgnoreRotate = false;
    m_bCirlce = false;
}

DecorationUtil::~DecorationUtil()
{
    
}

DecorationUtil* DecorationUtil::createDefault(Size size)
{
    std::string pathFrame;
    std::string pathZoom;
    std::string pathDelete;
    pathZoom = "content/common/decorate/public_zoom.png";
    pathDelete = "content/common/decorate/public_btn_close.png";
    pathFrame = "content/common/decorate/public_frame_small.png";
    return DecorationUtil::create(size, pathFrame, pathZoom, pathDelete);
}
DecorationUtil* DecorationUtil::create(Size size,std::string pathFrame,std::string pathZoom,std::string pathDelete)
{
    auto pRet = new DecorationUtil();
    if(pRet->init(size,pathFrame,pathZoom,pathDelete))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool DecorationUtil::init(Size size,std::string pathFrame,std::string pathZoom,std::string pathDelete)
{
    if ( !Node::init())
    {
        return false;
    }
    m_sFramePath = pathFrame;
    m_sZoomPath = pathZoom;
    
    if (size.width<130) {
        size.width = 130;
    }
    if (size.height<130) {
        size.height = 130;
    }
    m_pFrame = Scale9Sprite::create(pathFrame);
    m_pFrame->setPreferredSize(size);
    m_pFrame->setContentSize(size);
    m_pFrame->setInsetLeft(10);
    m_pFrame->setInsetRight(10);
    m_pFrame->setInsetTop(10);
    m_pFrame->setInsetBottom(10);
    this->addChild(m_pFrame);
    m_pFrame->setLocalZOrder(100);
    
    m_pZoom = Sprite::create(pathZoom);
    this->addChild(m_pZoom);
    m_pZoom->setLocalZOrder(100);
    Vec2 posRightBottom =  Vec2(m_pFrame->getBoundingBox().getMaxX(), m_pFrame->getBoundingBox().getMinY());
    m_pZoom->setPosition(posRightBottom);
    
    string deletPath(pathDelete);
    m_pBtnDelet = Button::create(deletPath,deletPath,deletPath);
    this->addChild(m_pBtnDelet);
    m_pBtnDelet->setLocalZOrder(100);
    Vec2 posLeftTop =  Vec2(m_pFrame->getBoundingBox().getMinX(), m_pFrame->getBoundingBox().getMaxY());
    m_pBtnDelet->setPosition(posLeftTop);
    m_pBtnDelet->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            if (utilDeletFuncCallback) {
                utilDeletFuncCallback();
            }
            this->removeFromParent();
        }
    });
    
    m_nFrameSpace = min(m_pFrame->getContentSize().width, m_pFrame->getContentSize().height);
    this->setContentSize(Size(m_pFrame->getContentSize().width+m_pZoom->getContentSize().width, m_pFrame->getContentSize().height+m_pZoom->getContentSize().height));
    return true;
}


void DecorationUtil::onEnter()
{
    Node::onEnter();
    scheduleOnce(schedule_selector(DecorationUtil::updateFrameStatus), 1.5f);
    m_bIsShotScreen = false;
    //    if (hidden == true) {
    //        updateFrameStatus(0);
    //    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(m_bSwallow);
    listener->onTouchBegan = CC_CALLBACK_2(DecorationUtil::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(DecorationUtil::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(DecorationUtil::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(DecorationUtil::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void DecorationUtil::onExit()
{
    m_bIsShotScreen = true;
    Node::onExit();
}

void DecorationUtil::onExitTransitionDidStart()
{
    m_bIsShotScreen = true;
    Node::onExitTransitionDidStart();
}

bool DecorationUtil::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (!m_bTouchEnable) {
        return false;
    }
    Point po = touch->getLocation();
    Rect rec = Rect(-m_pFrame->getAnchorPointInPoints().x, -m_pFrame->getAnchorPointInPoints().y, _contentSize.width, _contentSize.height);
    if (m_bFrameShowing == true) {
        rec.origin.x -= m_pZoom->getContentSize().width*0.5;
        rec.origin.y -= m_pZoom->getContentSize().height*0.5;
        rec.size.width += m_pZoom->getContentSize().width;
        rec.size.height += m_pZoom->getContentSize().height;
    }
    
    Rect boundBox = RectApplyAffineTransform(rec, getNodeToParentAffineTransform());
    boundBox.origin = this->getParent()->convertToWorldSpace(boundBox.origin);
    
    if (!boundBox.containsPoint(po)) {
        log("dont contain");
        return false;
    }
    if (m_bIgnoreTip) {
        updateFrameStatus(0);
        return true;
    }
    unschedule(schedule_selector(DecorationUtil::updateFrameStatus));
    m_bFrameShowing = !m_bFrameShowing;
    if(m_bFrameShowing)
    {
        m_pFrame->setVisible(true);
        m_pZoom->setVisible(true);
        m_pBtnDelet->setVisible(true);
        m_bFrameShowing = true;
    }
    //    this->getParent()->reorderChild(this, this->getLocalZOrder() + 50);
    
    Rect zoomRect = RectApplyAffineTransform(m_pZoom->getBoundingBox(), getNodeToParentAffineTransform());
    zoomRect.origin = this->getParent()->convertToWorldSpace(zoomRect.origin);
    if (!zoomRect.containsPoint(po) || m_bIgnoreRotate) {
        m_eEventType = EventType::MOVE;
    }else{
        m_eEventType = EventType::SCALE;
    }
    m_fOrignalRotate = this->getRotation();
    return true;
}

void DecorationUtil::onTouchMoved(Touch *touch, Event *unused_event)
{
    if (m_bIgnoreTip) {
        m_bFrameShowing = false;
    }else{
        m_bFrameShowing = true;
    }
    Point po = touch->getLocation();
    
    if (m_eEventType == EventType::NONE)
    {
        m_eEventType = EventType::MOVE;
    }
    
    
    switch (m_eEventType) {
        case NONE:
            break;
        case SCALE:
        {
            Vec2 location = this->getPosition();
            location = this->getParent()->convertToWorldSpace(location);
            float distance = po.distance(location);
            float orignalLengh = sqrt(m_pFrame->getContentSize().width*m_pFrame->getContentSize().height*0.5);
            float m_Scale = distance/orignalLengh;
            this->setScale(m_Scale);
            
            
            log(" po %f, po%f, ", po.x,po.y);
            log(" getPositionX %f, getPositionY%f, ", location.x,location.y);
            log("distance %f",distance);
            log("m_ScaleB %f", m_Scale);
            
            if(m_Scale < 0.5){
                m_Scale = 0.55f;
                this->setScale(0.55);
                //                this->setPosition(this->getPosition()+touch->getDelta());
            }else if(m_Scale >= 1.5){
                m_Scale = 1.45f;
                this->setScale(1.45);
                //                this->setPosition(this->getPosition()+touch->getDelta());
            }else {
                Vec2 temp1 = touch->getStartLocation() - location;
                Vec2 temp2 = po - location;
                float degree = temp1.getAngle(temp2);
                float theAngle = CC_RADIANS_TO_DEGREES(degree);
                this->setRotation(m_fOrignalRotate-theAngle);
            }
            m_pBtnDelet->setScale(1/m_Scale);
            m_pZoom->setScale(1/m_Scale);
            
            log("m_ScaleA %f", m_Scale);
            
        }
            break;
        case ROTATE:
        {
            Vec2 location = this->getPosition();
            location = this->getParent()->convertToWorldSpace(location);
            Vec2 temp1 = touch->getStartLocation() -location;
            Vec2 temp2 = po - location;
            float theAngle = CC_RADIANS_TO_DEGREES(temp1.getAngle(temp2));
            this->setRotation(m_fOrignalRotate-theAngle);
        }
            break;
        case MOVE:
        {
            Vec2 afterP = this->getPosition()+touch->getDelta();
            if (m_LimitRect.equals(Rect::ZERO)) {
                this->setPosition(afterP);
            }else{
                if (m_LimitRect.containsPoint( afterP)) {
                    this->setPosition(afterP);
                }else{
                    Vec2 pos = afterP;
                    if (afterP.x<m_LimitRect.getMinX()) {
                        pos.x = m_LimitRect.getMinX();
                    }
                    if (afterP.x>m_LimitRect.getMaxX()) {
                        pos.x = m_LimitRect.getMaxX();
                    }
                    if (afterP.y<m_LimitRect.getMinY()) {
                        pos.y = m_LimitRect.getMinY();
                    }
                    if (afterP.y>m_LimitRect.getMaxY()) {
                        pos.y = m_LimitRect.getMaxY();
                    }
                    if(m_bCirlce){
                        Vec2 originPos = Vec2(m_LimitRect.getMidX(), m_LimitRect.getMidY());
                        float a = m_LimitRect.size.width/2;
                        float b = m_LimitRect.size.height/2;
                        float dX = pos.x-originPos.x;
                        if (dX<-a) {
                            dX = -a;
                        }
                        if (dX>a) {
                            dX = a;
                        }
                        float dY = (1.0-pow(dX, 2)/pow(a, 2))*pow(b, 2);
                        float posY = sqrt(dY);
                        if (pos.y-originPos.y>0) {
                            pos.y = originPos.y+posY;
                        }else{
                            pos.y = originPos.y-posY;
                        }
                    }
                    this->setPosition(pos);
                }
            }
        }
            break;
        default:
            break;
    }
}
void DecorationUtil::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (m_bFrameShowing == false) {
        updateFrameStatus(0);
    }else {
        scheduleOnce(schedule_selector(DecorationUtil::updateFrameStatus), 1.5f);
    }
    m_eEventType = NONE;
    //    this->getParent()->reorderChild(this, this->getLocalZOrder() - 50);
}
void DecorationUtil::onTouchCancelled(Touch *touch, Event *unused_event)
{
    onTouchEnded(touch, unused_event);
}

void DecorationUtil::updateFrameStatus(float)
{
    m_pBtnDelet->setVisible(false);
    m_pZoom->setVisible(false);
    m_pFrame->setVisible(false);
    m_bFrameShowing = false;
}

void DecorationUtil::updateFrameStatusToVisible()
{
    m_pBtnDelet->setVisible(true);
    m_pZoom->setVisible(true);
    m_pFrame->setVisible(true);
    m_bFrameShowing = true;
}


void DecorationUtil::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags){
    if (m_bIsShotScreen == true) {
        updateFrameStatus(0);
    }
    Node::visit(renderer, parentTransform, parentFlags);
}


