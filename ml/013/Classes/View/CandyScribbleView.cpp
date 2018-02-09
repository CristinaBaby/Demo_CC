//
//  CandyScribbleView.cpp
//  Chups006
//
//  Created by QAMAC01 on 14-9-17.
//
//

#include "CandyScribbleView.h"
#include <cocos2d.h>
#include "MakeConfig.h"
#include "KSVisibleRect.h"
#include "Config.h"
#include "AudioController.h"
#include "IAPManager.h"
#include "ActionController.h"
USING_NS_CC;

const string solidBrushPath = "solid_brush.png";
EventListenerTouchOneByOne* lis_;
Scene* CandyScribbleView::scene(){
    Scene *scene = Scene::create();
    CandyScribbleView *layer = CandyScribbleView::create();
    scene->addChild(layer);
    return scene;
}

CandyScribbleView::CandyScribbleView(){
    _scribble = new Scribble(CCSprite::create("solid_brush.png"));
    _scribble->setBrushShader();
    _scribble->setBrushType(Scribble::eBrush);
    _scribble->setTargetSolid(true);
    _scribble->setTargetAlphaTestValue(0.0f);
    _rtx = nullptr;
}

CandyScribbleView::~CandyScribbleView(){
    _targetCandyCoating->release();
    hand_ = nullptr;
    delete _scribble;
}

bool CandyScribbleView::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    this -> brushAction();
    this -> initCandyCoating();
    _rtx = nullptr;
    hand_ = nullptr;
    lis_ = EventListenerTouchOneByOne::create();
    lis_ -> setSwallowTouches(false);
    lis_ -> onTouchBegan = CC_CALLBACK_2(CandyScribbleView::touchBegan, this);
    lis_ -> onTouchMoved = CC_CALLBACK_2(CandyScribbleView::touchMoved, this);
    lis_ -> onTouchEnded = CC_CALLBACK_2(CandyScribbleView::touchEnded, this);
    lis_ -> retain();
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis_ -> clone(), this);
    
    return true;
}


void CandyScribbleView::initCandyCoating()
{
    _targetCandyCoating = nullptr;
}

void CandyScribbleView::updateScribble(const char* name)
{
//    static int a = 0;
//    if (a == 0)
//    {
//        hand_ = Sprite::create("shou1.png");
//        hand_ -> setPosition(Vec2(321-100, 442-100));
//        hand_ -> runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(200, 250)),MoveBy::create(0.5, Vec2(-200, -250)), DelayTime::create(0.5), NULL)));
//        this -> addChild(hand_,10);
//        a++;
//    }
//    
//    pSprite2_ = Sprite::create(name);
//    pSprite2_->setPosition(Point(pSprite2_->getContentSize().width / 2, pSprite2_->getContentSize().height / 2));
//    _targetCandyCoating = pSprite2_;
//    _targetCandyCoating->retain();
//    
//    Size lCandyCoatingSize = pSprite2_->getTexture()->getContentSizeInPixels();
//    Point lCandyCoatingPosition = Vec2(321, 452+50);
//    log("%f,%f",lCandyCoatingPosition.x,lCandyCoatingPosition.y);
//    _rtx = RenderTexture::create(lCandyCoatingSize.width, lCandyCoatingSize.height);
//    _rtx->setPosition(lCandyCoatingPosition);
//    this->addChild(_rtx);
//    _canvasCandyCoating = _rtx;
}

void CandyScribbleView::updateScribbleWithTexture(Texture2D* txt , std::string bottleName)
{

    CC_SAFE_RELEASE_NULL(_targetCandyCoating);
    
    needSetTarget_ = true;
    
    pSprite2_ = Sprite::createWithTexture(txt);
    pSprite2_->setPosition(Point(pSprite2_->getContentSize().width / 2, pSprite2_->getContentSize().height / 2));
    
    _targetCandyCoating = pSprite2_;
    _targetCandyCoating->retain();
    
    Size lCandyCoatingSize = pSprite2_->getTexture()->getContentSizeInPixels();
    Point lCandyCoatingPosition = Config::getSnowconePos();
    if(!_rtx)
    {
        _rtx = RenderTexture::create(lCandyCoatingSize.width, lCandyCoatingSize.height);
        _rtx->setPosition(lCandyCoatingPosition);
        this->addChild(_rtx);
        _canvasCandyCoating = _rtx;

    }
    
//
    if(!hand_)
    {
        hand_ = Sprite::create();
        hand_ -> setAnchorPoint(Vec2(0.5, 0));
        hand_ -> setRotation(135);
        hand_ -> setFlippedX(true);
        this -> addChild(hand_,10,kTagHint);
    }
    
    hand_ -> setPosition(Vec2(300, 400));
    hand_ -> setTexture(bottleName);
    hand_ -> stopAllActions();
    
}


void CandyScribbleView::forbidScribble()
{
    if(lis_)
    {
        _eventDispatcher -> removeEventListenersForTarget(this);
    }
    
    if(hand_)
    {
        hand_ -> stopAllActions();
        hand_ -> removeFromParent(); hand_ = nullptr;
    }
}

void CandyScribbleView::openScribble()
{
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(lis_ -> clone(), this);
}


void CandyScribbleView::brushAction()
{
    _scribble->setTargetSolid(true);
    _scribble->setBrushType(Scribble::eBrush);
    Sprite *lBrush = Sprite::create("solid_brush.png");
    _scribble->setBrush(lBrush->getTexture());
    _currentToolTag = ToolTag::eBrushTag;
    
}

bool CandyScribbleView::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    Point lTouchPoint = (pTouch->getLocation());
    Point delta = KSVisibleRect::getDesignOffset();
    
    float yDis = IAPManager::getInstance() -> isShowAds()?100:0;

    if(hand_)
        hand_ -> setPosition(pTouch->getLocation() - delta + Vec2(0,-yDis)) ;

    switch (_currentToolTag) {
        case ToolTag::eBrushTag:{
            if (needSetTarget_)
            {
                _scribble->setTarget(_targetCandyCoating);
                needSetTarget_ = false;
            }
            _scribble->paint(nullptr, _canvasCandyCoating, lTouchPoint);
            break;
        }

        default:
            break;
    }
    
    
    if(hand_)
    {
        hand_ -> stopAllActions();
    }

    return true;//????
}

void CandyScribbleView::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
   
    
    float yDis = IAPManager::getInstance() -> isShowAds()?100:0;

    Point delta = KSVisibleRect::getDesignOffset();
    if(hand_)
        hand_ -> setPosition(pTouch->getLocation() - delta + Vec2(0,-yDis)) ;
    
    Point lTouchPoint = (pTouch->getLocation());
    Point lPreTouchPoint = (pTouch->getPreviousLocation());
    switch (_currentToolTag) {
        case ToolTag::eBrushTag:{
//            _scribble->setTarget(_targetCandyCoating);
            _scribble->paint(nullptr, _canvasCandyCoating, lPreTouchPoint,lTouchPoint);
//            AudioController::getInstance() -> playLoopEffect("sound/snowcone.mp3");
            break;
        }

        default:
            break;
    }
}

void CandyScribbleView::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    AudioController::getInstance() -> stopLoopEffect();
}



void CandyScribbleView::onExit()
{
    Node::onExit();
    

}

