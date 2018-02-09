//
//  FillMaterialModel.cpp
//  COOK016_TacoMaker
//
//  Created by jianghai on 15-4-23.
//
//

#include "FillMaterialModel.h"
FillMaterialModel::FillMaterialModel():
_close(nullptr),
_magnify(nullptr),
_item(nullptr),
_listener(nullptr),
_eventType(EventType::NONE),
_orignalRotate(0),
_isTipsFrameShow(true),
_enable(true),
isShotScreen(true),
hidden(false),
limitRect(Rect::ZERO)
{
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(FillMaterialModel::onReciveNotify), kShotScreenEvent, nullptr);
}

FillMaterialModel::~FillMaterialModel()
{
    __NotificationCenter::getInstance()->removeObserver(this, kShotScreenEvent);
}

FillMaterialModel* FillMaterialModel::create(const string& ImageName, bool small)
{
    auto pRet = new FillMaterialModel();
    if(pRet->init(ImageName, small))
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

bool FillMaterialModel::init(const string& ImageName,bool small)
{
    if (small == true) {
        linefile = "content/common/decorate/public_frame_small.png";
        transportfile = "content/common/decorate/transparent_small.png";
        ItemSpace = 115;
    }else {
        linefile = "content/common/decorate/public_frame.png";
        transportfile = "content/common/decorate/transparent.png";
        ItemSpace = 310;
    }
    if ( !Sprite::initWithFile(linefile))
    {
        return false;
    }
    
    _resourceName = ImageName;
    
    _listener = EventListenerTouchOneByOne::create();
    _listener->retain();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(FillMaterialModel::onTouchBegan, this);
    _listener->onTouchMoved = CC_CALLBACK_2(FillMaterialModel::onTouchMoved, this);
    _listener->onTouchEnded = CC_CALLBACK_2(FillMaterialModel::onTouchEnded, this);
    _listener->onTouchCancelled = CC_CALLBACK_2(FillMaterialModel::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
//    _close = CocosHelper3::getButton("ui/decorate/controlTip_close.png","ui/decorate/controlTip_close.png");
    
    Scale9Sprite* sp = Scale9Sprite::create("content/common/decorate/public_btn_close.png");
    _close = ControlButton::create(sp);
    _close -> setPreferredSize(Size(64,64));
    _close->addTargetWithActionForControlEvents(this, cccontrol_selector(FillMaterialModel::deleteSelf), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    _magnify = Sprite::create("content/common/decorate/public_zoom.png");
    _item = Sprite::create(ImageName);
    
    
    _close->setPosition(0, ItemSpace);
    _magnify->setPosition(ItemSpace, 0);
    _item->setPosition(ItemSpace*0.5, ItemSpace*0.5);
    
    this->addChild(_item);
    this->addChild(_close);
    this->addChild(_magnify);
    return true;
}

void FillMaterialModel::removeTouch()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

void FillMaterialModel::enableTouch()
{
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener -> clone(), this);
}


void FillMaterialModel::removeCloseBtn(){
    _close->setOpacity(0);
    _close->setEnabled(false);
}

void FillMaterialModel::changeItemTexture(const string &filename){
    _resourceName = filename;
    _item->setTexture(filename);
}

void FillMaterialModel::deleteSelf(cocos2d::Ref *sender, Control::EventType type)
{
    if(c_fun){
        c_fun();
    }
    this->removeFromParent();
}


void FillMaterialModel::onEnter()
{
    Sprite::onEnter();
    scheduleOnce(schedule_selector(FillMaterialModel::updateTipsFrameStatus), 1.5f);
    isShotScreen = false;
    if (hidden == true) {
        updateTipsFrameStatus(0);
    }
}

void FillMaterialModel::onExit()
{
    isShotScreen = true;
    Sprite::onExit();
}

void FillMaterialModel::onExitTransitionDidStart()
{
    isShotScreen = true;
    Sprite::onExitTransitionDidStart();
}

bool FillMaterialModel::onTouchBegan(Touch *touch, Event *unused_event)
{
    if(!_enable)
        return false;
    Point po = this->convertToNodeSpace(touch->getLocation());
    Rect rec = Rect::ZERO;
    if (_isTipsFrameShow == true) {
        rec = Rect(-27, -27, this->getContentSize().width + 54, this->getContentSize().height+ 54);
    }else {
        rec = _item->getBoundingBox();
    }
    
    if(!rec.containsPoint(po))
        return false;
    unschedule(schedule_selector(FillMaterialModel::updateTipsFrameStatus));
    _isTipsFrameShow = !_isTipsFrameShow;
    if(_isTipsFrameShow)
    {
        _close->setVisible(true);
        _magnify->setVisible(true);
        this->setSpriteFrame(Sprite::create(linefile)->getSpriteFrame());
        _isTipsFrameShow = true;
    }
    this->getParent()->reorderChild(this, this->getLocalZOrder() + 50);
    
    if(!_magnify->getBoundingBox().containsPoint(po))
        _eventType = MOVE;
    _orignalRotate = this->getRotation();
    return true;
}

void FillMaterialModel::onTouchMoved(Touch *touch, Event *unused_event)
{
    _isTipsFrameShow = true;
    Point po = touch->getLocation();

    if (_eventType == NONE)
    {
        _eventType = SCALE;
    }
    
    
    switch (_eventType) {
        case NONE:
            break;
        case SCALE:
        {
            float distance = po.distance(this->getPosition());
            float orignalLengh = sqrt(ItemSpace*ItemSpace*0.5);
            float m_Scale = distance/orignalLengh;
            this->setScale(m_Scale);
            
            
            log(" po %f, po%f, ", po.x,po.y);
            log(" getPositionX %f, getPositionY%f, ", this->getPositionX(),this->getPositionY());
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
                Vec2 temp1 = touch->getStartLocation() - this->getPosition();
                Vec2 temp2 = po - this->getPosition();
                float degree = temp1.getAngle(temp2);
                float theAngle = CC_RADIANS_TO_DEGREES(degree);
                this->setRotation(_orignalRotate-theAngle);
            }
            _close->setScale(1/m_Scale);
            _magnify->setScale(1/m_Scale);
            
            log("m_ScaleA %f", m_Scale);

        }
            break;
        case ROTATE:
        {
            Vec2 temp1 = touch->getStartLocation() - this->getPosition();
            Vec2 temp2 = po - this->getPosition();
            float theAngle = CC_RADIANS_TO_DEGREES(temp1.getAngle(temp2));
            this->setRotation(_orignalRotate-theAngle);
        }
            break;
        case MOVE:
        {
            Vec2 afterP = this->getPosition()+touch->getDelta();
            if (limitRect.equals(Rect::ZERO)) {
                this->setPosition(afterP);
            }else{
                if (limitRect.containsPoint( afterP)) {
                    this->setPosition(afterP);
                }
            }
        }
            break;
        default:
            break;
    }
}
void FillMaterialModel::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (_isTipsFrameShow == false) {
        updateTipsFrameStatus(0);
    }else {
        scheduleOnce(schedule_selector(FillMaterialModel::updateTipsFrameStatus), 1.5f);
    }
    _eventType = NONE;
    this->getParent()->reorderChild(this, this->getLocalZOrder() - 50);
}
void FillMaterialModel::onTouchCancelled(Touch *touch, Event *unused_event)
{
    onTouchEnded(touch, unused_event);
}

void FillMaterialModel::updateTipsFrameStatus(float)
{
    _close->setVisible(false);
    _magnify->setVisible(false);
    this->setSpriteFrame(Sprite::create(transportfile)->getSpriteFrame());
    _isTipsFrameShow = false;
}

void FillMaterialModel::updateTipsFrameStatusToVisibal()
{
    _close->setVisible(true);
    _magnify->setVisible(true);
    this->setSpriteFrame(Sprite::create(linefile)->getSpriteFrame());
    _isTipsFrameShow = true;
}

void FillMaterialModel::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags){
    if (isShotScreen == true) {
        updateTipsFrameStatus(0);
    }
    Sprite::visit(renderer, parentTransform, parentFlags);
}

void FillMaterialModel::onReciveNotify(cocos2d::Ref *pRef) {
    __String* mesg = dynamic_cast<__String*>(pRef);
    if (mesg->compare("Yes") == 0) {
        isShotScreen = false;
    }else if (mesg->compare("No") == 0){
        isShotScreen = true;
    }
    
}

