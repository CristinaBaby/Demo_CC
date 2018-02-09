//
//  FillMaterialModel.cpp
//  FMAKER3042_Taco
//
//  Created by jianghai on 15-4-23.
//
//

#include "FillMaterialModel.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
FillMaterialModel::FillMaterialModel():
_close(nullptr),
_magnify(nullptr),
_item(nullptr),
_listener(nullptr),
_eventType(EventType::NONE),
_orignalRotate(0),
_isTipsFrameShow(true),
_enable(true),
_moveRect(Rect(Director::getInstance()->getVisibleOrigin().x, Director::getInstance()->getVisibleOrigin().y + 250, Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height))
{
    
}

FillMaterialModel::~FillMaterialModel()
{
    
}

FillMaterialModel* FillMaterialModel::create(const char* ImageName)
{
    auto pRet = new FillMaterialModel();
    if(pRet->init(ImageName))
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

bool FillMaterialModel::init(const char* ImageName)
{
    if ( !Sprite::initWithFile("controlTip_bg.png"))
    {
        return false;
    }
    
    _resourceName = ImageName;
    
    _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(FillMaterialModel::onTouchBegan, this);
    _listener->onTouchMoved = CC_CALLBACK_2(FillMaterialModel::onTouchMoved, this);
    _listener->onTouchEnded = CC_CALLBACK_2(FillMaterialModel::onTouchEnded, this);
    _listener->onTouchCancelled = CC_CALLBACK_2(FillMaterialModel::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    _close = SpriteButton::create("controlTip_close.png",std::bind(&FillMaterialModel::deleteSelf, this,std::placeholders::_1));
    _magnify = Sprite::create("controlTip_magnify.png");
    _item = Sprite::create(ImageName);
    
    _close->setPosition(0, 200);
    _magnify->setPosition(200, 0);
    _item->setPosition(100, 100);
    
    this->addChild(_item);
    this->addChild(_close);
    this->addChild(_magnify);
    
    return true;
}

void FillMaterialModel::deleteSelf(cocos2d::Ref *sender)
{
    SimpleAudioEngine::getInstance()->playEffect("delete.mp3");
    this->removeFromParent();
}
void FillMaterialModel::setNotDeleteSelf()
{
    _close->setEnable(false);
}
void FillMaterialModel::onEnter()
{
    Sprite::onEnter();
    this->scheduleOnce(schedule_selector(FillMaterialModel::updateTipsFrameStatus), 1.0);
}
void FillMaterialModel::onExit()
{
    if(this->isScheduled(schedule_selector(FillMaterialModel::updateTipsFrameStatus)))
        this->unschedule(schedule_selector(FillMaterialModel::updateTipsFrameStatus));
    Sprite::onExit();
}

bool FillMaterialModel::onTouchBegan(Touch *touch, Event *unused_event)
{
    if(!_enable)
        return false;
    Point po = this->convertToNodeSpace(touch->getLocation());
    Rect rec = Rect(-27, -27, this->getContentSize().width + 54, this->getContentSize().height+ 54);
    if(!rec.containsPoint(po))
        return false;
    if(!_isTipsFrameShow)
    {
        _close->setVisible(true);
        _magnify->setVisible(true);
        this->setSpriteFrame(Sprite::create("controlTip_bg.png")->getSpriteFrame());
        _isTipsFrameShow = true;
    }
    //停止隐藏提示控件的动作
    if(this->isScheduled(schedule_selector(FillMaterialModel::updateTipsFrameStatus)))
       this->unschedule(schedule_selector(FillMaterialModel::updateTipsFrameStatus));
    
    this->getParent()->reorderChild(this, this->getLocalZOrder() + 50);
    
    if(!_magnify->getBoundingBox().containsPoint(po))
        _eventType = MOVE;
    _orignalRotate = this->getRotation();
    return true;
}
void FillMaterialModel::onTouchMoved(Touch *touch, Event *unused_event)
{
    Point po = touch->getLocation();
    if (_eventType == NONE)
    {
        float theRadians = (this->convertToNodeSpace(po) -  this->convertToNodeSpace(touch->getStartLocation())).getAngle();
        float theAngle = CC_RADIANS_TO_DEGREES(theRadians);
        
        if (-65 < theAngle  && theAngle < -25)
            _eventType = SCALE;
        else if(115 < theAngle && theAngle < 155)
            _eventType = SCALE;
        else
            _eventType = ROTATE;
    }
    
    
    switch (_eventType) {
        case NONE:
            break;
        case SCALE:
        case ROTATE:
        {
            float distance = po.distance(this->getParent()->convertToWorldSpace(this->getPosition()));
            float orignalLengh = sqrt(100*100*2);
            float m_Scale = distance/orignalLengh;
            this->setScale(m_Scale);
            
            if(m_Scale < 0.5)
                this->setScale(0.5);
            if(m_Scale>1.5)
                this->setScale(1.5);
            
//        }
////            break;
//        case ROTATE:
//        {
            Vec2 temp1 = touch->getStartLocation() - this->getParent()->convertToWorldSpace(this->getPosition());
            Vec2 temp2 = po - this->getParent()->convertToWorldSpace(this->getPosition());
            float theAngle = CC_RADIANS_TO_DEGREES(temp1.getAngle(temp2));
            this->setRotation(_orignalRotate-theAngle);
        }
            break;
        case MOVE:
        {
            this->setPosition(this->getPosition()+touch->getDelta());

             //在一个矩形区域移动
           if(this->getParent()->convertToWorldSpace(this->getPosition()).x < _moveRect.getMinX())
            {
                
                this->setPositionX(this->getParent()->convertToNodeSpace(Vec2(_moveRect.getMinX(),_moveRect.getMinY())).x);
                log("---%f",this->convertToNodeSpace(Vec2(_moveRect.getMinX(),_moveRect.getMinY())).x);
            
            }
            if (this->getParent()->convertToWorldSpace(this->getPosition()).x > _moveRect.getMaxX())
            {
                this->setPositionX(this->getParent()->convertToNodeSpace(Vec2(_moveRect.getMaxX(),_moveRect.getMaxY())).x);
            
            }
            if (this->getParent()->convertToWorldSpace(this->getPosition()).y < _moveRect.getMinY())
            {
                
                this->setPositionY(this->getParent()->convertToNodeSpace(Vec2(_moveRect.getMinX(),_moveRect.getMinY())).y);
                
                
            }
            if (this->getParent()->convertToWorldSpace(this->getPosition()).y > _moveRect.getMaxY())
            {
                
                this->setPositionY(this->getParent()->convertToNodeSpace(Vec2(_moveRect.getMaxX(),_moveRect.getMaxY())).y);
                
            }
   
        }
            break;
        default:
            break;
    }
}
void FillMaterialModel::stopTipsFrameStatus()
{
//    if(this->isScheduled(schedule_selector(FillMaterialModel::updateTipsFrameStatus))){
//        
//        this->unschedule(schedule_selector(FillMaterialModel::updateTipsFrameStatus));
//        
//    }
    _close->setVisible(false);
    _magnify->setVisible(false);
    this->setSpriteFrame(Sprite::create("controlTip_bg_Clear.png")->getSpriteFrame());
    _isTipsFrameShow = false;
    
}

void FillMaterialModel::onTouchEnded(Touch *touch, Event *unused_event)
{
    _eventType = NONE;
    this->getParent()->reorderChild(this, this->getLocalZOrder() - 50);
    this->scheduleOnce(schedule_selector(FillMaterialModel::updateTipsFrameStatus), 1.0);
}
void FillMaterialModel::onTouchCancelled(Touch *touch, Event *unused_event)
{
    onTouchEnded(touch, unused_event);
}

void FillMaterialModel::updateTipsFrameStatus(float)
{
    _close->setVisible(false);
    _magnify->setVisible(false);
    this->setSpriteFrame(Sprite::create("controlTip_bg_Clear.png")->getSpriteFrame());
    _isTipsFrameShow = false;
}