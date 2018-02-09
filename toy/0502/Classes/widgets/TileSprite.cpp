//
//  TileSprite.cpp
//  cocosTest
//
//  Created by zhangguangzong1 on 1/27/14.
//
//

#include "TileSprite.h"
#include "../Jigsaw/PuzzleScene.h"
#include <cmath>
#include "../AppGlobal.h"
#include "../utilities/Utils.h"
#include "../Jigsaw/JPSideBar.h"

TileSprite::TileSprite():mType(-1),mIsPlace(false),isSurpportRotate(false),mGroupIndex(-1)
{
}

TileSprite* TileSprite::create()
{
    TileSprite* sprite = new TileSprite();
    
    if (sprite)
    {
        sprite->init();
        sprite->autorelease();
        
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    
    return NULL;
}


TileSprite* TileSprite::create(const char* fileName)
{
    TileSprite* sprite = new TileSprite();
    if (sprite && sprite->initWithFile(fileName))
    {
        sprite->init();
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    
    return NULL;
}


TileSprite* TileSprite::create(CCTexture2D* texture)
{
    TileSprite* sprite = new TileSprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->init();
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

TileSprite* TileSprite::create(CCTexture2D* texture , Rect rect)
{
    TileSprite* sprite = new TileSprite();
    
    if (sprite && sprite->initWithTexture(texture , rect))
    {
        sprite->init();
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void TileSprite::onEnter()
{
    Sprite::onEnter();
}

TileSprite::~TileSprite()
{
    //_eventDispatcher->removeEventListenersForTarget(this);
    _eventDispatcher->removeEventListener(_listener);
}

float topDistrict = 0;
bool TileSprite::init()
{
    Size size  = Director::getInstance()->getVisibleSize();
    float scaleX = size.width/768.f;
    float scaleY = size.height/1024.f;
    topDistrict = size.height - 283*fmax(scaleX, scaleY) - getContentSize().height/2*getScale();
    isTouch = false;
    isInMap = 0;
    
    
    //add Touch 事件
    _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    
    _listener->onTouchBegan = CC_CALLBACK_2(TileSprite::onTouchBegan, this);
    _listener->onTouchMoved = CC_CALLBACK_2(TileSprite::onTouchMoved, this);
    _listener->onTouchEnded = CC_CALLBACK_2(TileSprite::onTouchEnded, this);
    
    getEventDispatcher()->addEventListenerWithFixedPriority(_listener, 1);
    
    
    return true;
}
void TileSprite::setOriginalPoint(cocos2d::Vec2 point)
{
    originalX = point.x;
    originalY = point.y;
}

void TileSprite::onExit()
{
    Sprite::onExit();
    _eventDispatcher->removeEventListenersForTarget(this);
}

void TileSprite::lockSprite()
{
    scheduleOnce(schedule_selector(TileSprite::lockAction),0.2);
    isTouch = true;
    setTag(LOCK_STATUS);
    //    setZOrder(0);
    getParent()->reorderChild(this, 1
                              );
}

void TileSprite::lockAction(float dt)
{
    getEventDispatcher()->removeEventListenersForTarget(this);
}

bool TileSprite::isLock()
{
    return getTag() == LOCK_STATUS ? true : false;
}

void TileSprite::goTop()
{
    www++;
    getParent()->reorderChild(this, www);
}

void TileSprite::recoverCenterAnchpoint()
{
    setAnchorPoint(Vec2(.5, .5));
    setPositionX(getPositionX()+getContentSize().width*getScale()/2);
    setPositionY(getPositionY()+getContentSize().height*getScale()/2);
}

bool TileSprite::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if(!getParent()->isVisible())
        return false;
    
    if(isTouch)
        return false;
    
    if(isLock())
        return false;
    
    _startPos = getPosition();
    
	Vec2 touchPoint = pTouch->getLocation();
	Vec2 realPoint = this->getParent()->convertToNodeSpace(touchPoint);
	Rect rect = this->boundingBox();
    
	if(rect.containsPoint(realPoint))
	{
        isTouch = true;
        m_touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
        
        _puzzleScene->removeCurHuitanGroupMap(getGroupIndex());
        if(getGroupIndex()!=-1){
            _puzzleScene->groupToTop(getGroupIndex());
        }else
            goTop();
        
        //        CCLOG("------------- index = %d" , get_m_Index());
		return true;
	}
    
    return false;
}

void TileSprite::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(isTouch){
        
        //CCLOG("onTouchMoved   pTouch->start %f   id=%d" , pTouch->getStartLocation().x , m_index);
        
        Vec2 touchPoint = pTouch->getLocation();
        Vec2 preTouchPoint = pTouch->getPreviousLocation();
        
        if (getGroupIndex()==-1) {
//            if(getPositionY() > topDistrict ){
//                
//                setPositionY(topDistrict);
//                return;
//            };
        }else{
            
            //TODO
            float maxY = _puzzleScene->getGroupMaxY(getGroupIndex());
            if(maxY > topDistrict ){
                
                _puzzleScene->moveGroupSprite( Vec2(0, -(maxY - topDistrict)), getGroupIndex() , m_index);
                return;
            };
        }
        
        if(abs(pTouch->getStartLocation().x - touchPoint.x) > 10 || abs(pTouch->getStartLocation().y - touchPoint.y) > 10)
        {
            Vec2 point =  ccpSub(touchPoint,preTouchPoint);
            
            if(getGroupIndex()!=-1){
                _puzzleScene->moveGroupSprite( point, getGroupIndex() , m_index);
                isSurpportRotate = false;
            }
            else
                this->setPosition(ccpAdd(this->getPosition(),point));
        }
        
    }
}

void TileSprite::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *event)
{
    if(isTouch){
        
        //判断是否超过, 然后加到listView, 里面
        Vec2 pos = convertToWorldSpaceAR(Vec2::ZERO);
        if (pos.x > _sidebar) {
            float width = _bar->getContentSize().width;
            
            Layout * layout = Layout::create();
            layout->setContentSize(Size(width, width));
            
            retain();
            removeFromParent();
            
            layout->addChild(this);
            setPosition(layout->getContentSize() / 2);
            release();
            setScale(_startScale);
            
            _bar->addChild(layout);
            _bar->requestRefreshView();
            
            return;
        }
        //检查下边界
        else if (pos.x + getContentSize().width * getScale() / 2 > _sidebar)
        {
            CCFiniteTimeAction* pAcion2 = CCEaseElasticOut::create(CCMoveTo::create(0.5f,_startPos),0.8f);//通过第二个参数来调整振幅，或者速度，值越大越慢振幅越小
            runAction(pAcion2);
        }
        else if (pos.x - getContentSize().width * getScale() / 2 < 0)
        {
            CCFiniteTimeAction* pAcion2 = CCEaseElasticOut::create(CCMoveTo::create(0.5f,_startPos),0.8f);//通过第二个参数来调整振幅，或者速度，值越大越慢振幅越小
            runAction(pAcion2);
        }
        else if (pos.y + getContentSize().height * getScale() / 2 > _parentSize.height)
        {
            CCFiniteTimeAction* pAcion2 = CCEaseElasticOut::create(CCMoveTo::create(0.5f,_startPos),0.8f);//通过第二个参数来调整振幅，或者速度，值越大越慢振幅越小
            runAction(pAcion2);
        }
        else if (pos.y - getContentSize().height * getScale() / 2 < 0)
        {
            CCFiniteTimeAction* pAcion2 = CCEaseElasticOut::create(CCMoveTo::create(0.5f,_startPos),0.8f);//通过第二个参数来调整振幅，或者速度，值越大越慢振幅越小
            runAction(pAcion2);
        }
        
        combination();
        //处理touch事件优先级的问题
        
        isTouch = false;
        
        if(getGroupIndex()==-1){
            
//            if(getPositionY() > topDistrict ){
//                
//                setPositionY(topDistrict);
//                return;
//            };
            
//            Vec2 point = Util::checkBorder(getPositionX() , getPositionY() , getContentSize().width,getContentSize().width/2);
//            if(point.x!=0 || point.y !=0)
//                kickBackAction(point);
        }else{
            float maxY = _puzzleScene->getGroupMaxY(getGroupIndex());
            
            if (maxY > topDistrict) {
                _puzzleScene->moveGroupSprite( Vec2(0, -(maxY - topDistrict)), getGroupIndex() , m_index);
            }
            
//            _puzzleScene->checkGroupBorder(getGroupIndex() , m_index);
            
        }
        
    }

    
}

void  TileSprite::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    isTouch = false;
    
    if(getGroupIndex()==-1){
        Vec2 point = Util::checkBorder(getPositionX() , getPositionY() , getContentSize().width,getContentSize().width/2);
        if(point.x!=0 || point.y !=0)
            kickBackAction(point);
    }else{
        
        _puzzleScene->checkGroupBorder(getGroupIndex() , m_index);
    }
}

//type 为1 表示初始化的时候, type 为0 表示与ScrollView相关的时候
void TileSprite::openTouchEvent(int type)
{
    if (type == 0) {
        
        _puzzleScene = (PuzzleScene*)getParent()->getParent();
        
        //处理touch事件优先级的问题
        isTouch = false;
        isInMap = 1;
        
        combination();
        
        _parentSize = getParent()->getContentSize();
        _sidebar = _parentSize.width * (960 - 230) / 960;
        
        if(getGroupIndex()==-1){
            Vec2 point = Util::checkBorder(getPositionX() , getPositionY() , getContentSize().width,getContentSize().width/2);
            if(point.x!=0 || point.y !=0)
                kickBackAction(point);
        }else{
            
            _puzzleScene->checkGroupBorder(getGroupIndex() , m_index);
        }
    }else{
        
        runAction(CCSequence::create(CCDelayTime::create(0.1), CCCallFunc::create(this,callfunc_selector(TileSprite::delayOpenTouch)) , NULL));
        
    }
}

void TileSprite::delayOpenTouch()
{
    //在离开scrollview后，才开始触摸事件，否则会影响

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(TileSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TileSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TileSprite::onTouchEnded, this);
    
    getEventDispatcher()->addEventListenerWithFixedPriority(listener, -1);

    //处理touch事件优先级的问题
    isTouch = false;
    isInMap = 1;
}

void  TileSprite::kickBackAction(Vec2 distance)
{
    CCFiniteTimeAction* pAcion2 = CCEaseElasticOut::create(CCMoveBy::create(0.5f,distance),0.8f);//通过第二个参数来调整振幅，或者速度，值越大越慢振幅越小
    runAction(pAcion2);
}

void TileSprite::goMainCallSetPriority()
{
    if(getGroupIndex()!=-1)
        _puzzleScene->setGroupTouchPriority(getGroupIndex());
    else
        setMyPriority();
}

void TileSprite::setMyPriority()
{
    mmm--;
    getEventDispatcher()->removeEventListenersForTarget(this);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(TileSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TileSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TileSprite::onTouchEnded, this);
    
    getEventDispatcher()->addEventListenerWithFixedPriority(listener, mmm);
    
    isTouch = false;
}


void TileSprite::rotateActionOver()
{
    action->autorelease();
    combination();
    isTouch = false;
}
//跟其它瓦片组合
void TileSprite::combination()
{
    _puzzleScene->checkCommbine(this);
}

//获取左上方顶点的点坐标
Vec2 TileSprite::getLeftTopPoint()
{
    if(getAnchorPoint().x == 0.5 && getAnchorPoint().y == 0.5)
    {
        return Vec2(getPositionX()-getContentSize().width/2, getPositionY() + getContentSize().height/2);
    }
    return Vec2(getPositionX(), getPositionY());
}

Vec2 TileSprite::getOriginalPoint()
{
//    if(getAnchorPoint().x == 0.5 && getAnchorPoint().y == 0.5)
//    {
//        return Vec2(originalX+getContentSize().width/2, originalY - getContentSize().height/2);
//    }
    return Vec2(originalX, originalY);
}

void TileSprite::setRotation(float fRotation)
{
    Sprite::setRotation(fRotation);
}

Vec2 TileSprite::getCombinePoint()
{
    return Vec2(originalX, originalY);
}


void TileSprite::recoverOriginalPostion()
{
    setPosition(getOriginalPoint());
}

void TileSprite::setPosition(const Vec2& pos)
{
    Sprite::setPosition(pos);
    
}

void TileSprite::supportRotate(bool isSupport)
{
    isSurpportRotate = isSupport;
}

void TileSprite::setGroupIndex(int groupIndex)
{
    mGroupIndex = groupIndex;
    supportRotate(false);
}

int TileSprite::getGroupIndex()
{
    return mGroupIndex;
}

int TileSprite::get_m_Index()
{
    return m_index;
}

void TileSprite::set_m_Index(int index)
{
    m_index = index;
}

bool  TileSprite::isInAngel(int row, int column)
{
    if(get_m_Index()==0 || get_m_Index() == column-1 ||  get_m_Index() == row*column - column || get_m_Index() == row*column-1)
        return true;
    return false;
}

void TileSprite::setSideBarBorder(float sidebar, int sideLen)
{
    _sidebar = sidebar;
    _sideLen = sideLen;
    
    switch (_sideLen) {
        case 2:
        {
            _compare = 30;
        }
            break;
        case 3:
        {
            _compare = 30;
        }
            break;
        case 4:
        {
            _compare = 25;
        }
            break;
        case 6:
        {
            _compare = 20;
        }
            break;
        default:
            CCASSERT(false, "error");
            break;
    }
}

void TileSprite::setStartScale(float startScale)
{
    _startScale = startScale;
}

void TileSprite::setJPSideBar(JPSideBar * sidebar)
{
    _bar = sidebar;
}

bool TileSprite::isInRightPlace()
{
    Vec2 pos = getPosition();
    float offsetx = fabsf(originalX - pos.x);
    float offsety = fabsf(originalY - pos.y);
    
    if (offsetx < _compare  && offsety < _compare) {
        return true;
    }
    else
    {
        return false;
    }
}


