//
//  EatSprite.cpp
//  JuiceMaker
//
//  Created by luotianqiang1 on 4/2/15.
//
//

#include "EatSprite.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
EatSprite::EatSprite():
eatPercentageCallback(nullptr)
{
    maksLayer = nullptr;
    touchListener = nullptr;
    _pixcelBegin = 0;
    _pixcelEnd = 0;
    _clippingNode = nullptr;
    _touchCallBack = nullptr;
    _enable = true;
    _eatMaskPath = "";
    _particlePath = "plists/biting.plist";
    _soundPath =  "sounds/eat.mp3";
    checkEnable = true;
}

EatSprite::~EatSprite() {
    CC_SAFE_RELEASE_NULL(touchListener);
    this->unschedule(schedule_selector(EatSprite::checkEatStatus));
}

bool EatSprite::init() {
    if(Node::init()) {
        setContentSize(Director::getInstance()->getWinSize());
        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        _clippingNode = ClippingNode::create();
        addChild(_clippingNode,3);
        _clippingNode->setAlphaThreshold(0.05);
        _clippingNode->setInverted(true);
        maksLayer = Layer::create();
        _clippingNode->setStencil(maksLayer);
        
        _downClippingNode = ClippingNode::create();
        _downClippingNode->setAlphaThreshold(0.05);
        _downClippingNode->setInverted(true);
        _downMaskLayer = Layer::create();
        _downClippingNode->setStencil(_downMaskLayer);
        addChild(_downClippingNode,2);
        return true;
    }
    return false;
}

void EatSprite::CloneSprite(){
    for(auto s:eatSprites){
        auto _newS = Sprite::createWithSpriteFrame(s->getSpriteFrame());
        _newS->setPosition(s->getPosition());
        _newS->setRotation(s->getRotation());
        _newS->setScale(s->getScale());
        Color3B _color;
        _color.r = s->getColor().r*.8;
        _color.g = s->getColor().g*.8;
        _color.b = s->getColor().b*.8;
        _newS->setColor(_color);
        _downClippingNode->addChild(_newS);
    }
}

void EatSprite::addDownSprite(Sprite* s){
    if(s != nullptr){
        Color3B _color;
        _color.r = s->getColor().r*.8;
        _color.g = s->getColor().g*.8;
        _color.b = s->getColor().b*.8;
        s->setColor(_color);
        _downClippingNode->addChild(s);
    }
}

void EatSprite::addSprite(Sprite* s) {
    eatSprites.pushBack(s);
    _clippingNode->addChild(s);
    
}

bool EatSprite::onTouchBegin(Touch *touch,Event* event) {
    if(!_enable)
        return false;
    if(nullptr != _touchCallBack)
        _touchCallBack();
    for(auto eat:eatSprites) {
        Rect _rect;
        _rect.size = eat->getContentSize();
        if(_rect.containsPoint(eat->convertToNodeSpace(touch->getLocation())))
        {
            
            if(!_particlePath.empty()) {
                ParticleSystemQuad *par = ParticleSystemQuad::create(_particlePath);
                par->setPosition(this->getParent()->convertToNodeSpace(touch->getLocation()));
                this->getParent()->addChild(par,15);
                
            }
            return true;
        }
    }
    
    return false;
}



void EatSprite::onTouchEnded(Touch *touch,Event* event)
{
    if(!_soundPath.empty()) {
        SimpleAudioEngine::getInstance()->playEffect(_soundPath.c_str());
    }

    auto mask = Sprite::create(_eatMaskPath);
    if(mask != nullptr){
        mask->setPosition(this->convertToNodeSpace(touch->getLocation()));
        maksLayer->addChild(mask);
        
        mask = Sprite::create(_eatMaskPath);
        mask->setPosition(this->convertToNodeSpace(touch->getLocation()));
        _downMaskLayer->addChild(mask);
        mask->setScale(0.85f);
    }
    
}

void EatSprite::onEnterTransitionDidFinish() {
    Node::onEnterTransitionDidFinish();
    if(nullptr == touchListener) {
        touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(EatSprite::onTouchBegin,this);
        touchListener->onTouchEnded = CC_CALLBACK_2(EatSprite::onTouchEnded, this);
        touchListener->setSwallowTouches(false);
        touchListener->setEnabled(true);
        touchListener->retain();
    }
    _eventDispatcher->removeEventListener(touchListener);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    if(checkEnable)
        this->schedule(schedule_selector(EatSprite::checkEatStatus), 2);
}
void EatSprite::onExitTransitionDidStart() {
    _eventDispatcher->removeEventListener(touchListener);
    Node::onExitTransitionDidStart();
}
//获取不透明像素点
size_t EatSprite::calcute(Image *pImage)
{
    size_t lCounter = 0;
    for (int i = 0; i < pImage->getHeight(); ++i) {
        for (int j = 0; j < pImage->getWidth(); ++j) {
            unsigned char *lData = pImage->getData();
            size_t lPixcelIndex = i * pImage->getWidth() + j;
            unsigned char lRed = lData[lPixcelIndex * 4];
            unsigned char lGreen = lData[lPixcelIndex * 4 + 1];
            unsigned char lBlue = lData[lPixcelIndex * 4 + 2];
            unsigned char lAlpha = lData[lPixcelIndex * 4 + 3];
            if (lAlpha > 10) {
                if (lRed > 0 || lGreen > 0 || lBlue > 0) {
                    ++lCounter;
                }
            }
        }
    }
    return lCounter;
}

void EatSprite::reset(){
    maksLayer->removeAllChildren();
    _downMaskLayer->removeAllChildren();
    this->unscheduleAllCallbacks();
    _pixcelBegin = 0;
    _pixcelEnd = 0;
    if(checkEnable)
        this->schedule(schedule_selector(EatSprite::checkEatStatus), 2);
}

void EatSprite::checkEatStatus(float)
{
    if(!_enable)
        return ;
    auto _size = this->getContentSize();
    RenderTexture* render = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    
    if(_pixcelBegin == 0)
    {
        render->begin();
        this->cocos2d::Node::visit();
        render->end();
        Director::getInstance()->getRenderer()->render();
        _image = render->newImage();
        _pixcelBegin = calcute(_image);
        if(_pixcelBegin==0)
            _pixcelBegin = 1;
        delete _image;
        return;
    }
    render->begin();
    this->visit();
    render->end();
    Director::getInstance()->getRenderer()->render();
    _image = render->newImage();
    _pixcelEnd = calcute(_image);
    delete _image;
    if(nullptr != eatPercentageCallback)
        eatPercentageCallback(1 - _pixcelEnd/(_pixcelBegin *1.0));
}

float EatSprite::getEatPercentage()
{
    return 1 - _pixcelEnd/(_pixcelBegin *1.0);
}

void EatSprite::stopCheckPercentage()
{
    this->unschedule(schedule_selector(EatSprite::checkEatStatus));
}