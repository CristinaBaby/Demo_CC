//
//  ModelLayer.cpp
//  PaintDress
//
//  Created by maxiang on 6/29/15.
//
//

#include "ModelLayer.h"
#include "../Model/STVisibleRect.h"

USING_NS_CC;
using namespace ui;

#define RES_MODEL_BODY              "res/model/body.png"
#define RES_MODEL_FACE              "res/model/face1.png"
#define RES_MODEL_FACE_ANIMATION    "res/model/face"

#define ANIMATION_COUNT             7
#define ANIMATION_DURATION          1.0

ModelLayer::ModelLayer() :
_hair(nullptr),
_bottom(nullptr),
_top(nullptr),
_shoes(nullptr),
_dress(nullptr),
_accessory(nullptr),
_model(Model{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr})
{}

ModelLayer::~ModelLayer()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool ModelLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    
    do {
        //setup body
        _modelBodySprite = Sprite::create(RES_MODEL_BODY);
        namespaceST::STVisibleRect::setPosition(_modelBodySprite, 185, 395, false, true, true, false);
        addChild(_modelBodySprite);
        
        //setup face
        auto face = Sprite::create(RES_MODEL_FACE);
        face->setPosition(Vec2(174, 530));
        _modelBodySprite->addChild(face);
        
        auto animation = Animation::create();
        for( int i = 1; i < ANIMATION_COUNT; ++i)
        {
            char name[100];
            sprintf(name, "%d.png", i);
            animation->addSpriteFrameWithFile(RES_MODEL_FACE_ANIMATION + std::string(name));
        }
        animation->setDelayPerUnit(ANIMATION_DURATION / ANIMATION_COUNT);
        animation->setRestoreOriginalFrame(true);
        
        auto action = Animate::create(animation);
        face->runAction(RepeatForever::create(Sequence::create(action, DelayTime::create(3), NULL)));

        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(false);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Action methods

void ModelLayer::setRes(Res* res)
{
    if (res == nullptr)
        return;
    
    switch (res->type)
    {
        case ResType::ResType_Hair:
        {
            _model.hairRes = res;
            setHair(res->resPicturePath, Vec2(res->positionX, res->positionY));
            break;
        }
        case ResType::ResType_Bottom:
        {
            if (_model.dressRes != nullptr)
            {
                _model.dressRes = nullptr;
                if (_dress)
                {
                    _dress->removeFromParent();
                    _dress = nullptr;
                }
            }
            
            _model.bottomRes = res;
            setBottom(res->resPicturePath, Vec2(res->positionX, res->positionY));
            break;
        }
        case ResType::ResType_Dress:
        {
            if (_model.topRes != nullptr)
            {
                _model.topRes = nullptr;
                if (_top)
                {
                    _top->removeFromParent();
                    _top = nullptr;
                }
            }
            if (_model.bottomRes != nullptr)
            {
                _model.bottomRes = nullptr;
                if (_bottom)
                {
                    _bottom->removeFromParent();
                    _bottom = nullptr;
                }
            }
            
            _model.dressRes = res;
            setDress(res->resPicturePath, Vec2(res->positionX, res->positionY));
            break;
        }
        case ResType::ResType_Top:
        {
            if (_model.dressRes != nullptr)
            {
                _model.dressRes = nullptr;
                if (_dress)
                {
                    _dress->removeFromParent();
                    _dress = nullptr;
                }
            }
            
            _model.topRes = res;
            setTop(res->resPicturePath, Vec2(res->positionX, res->positionY));
            break;
        }
        case ResType::ResType_Shoes:
        {
            _model.shoesRes = res;
            setShoes(res->resPicturePath, Vec2(res->positionX, res->positionY));
            break;
        }
        case ResType::ResType_Accessory:
        {
            _model.accessoryRes = res;
            setAccessory(res->resPicturePath, Vec2(res->positionX, res->positionY));
            break;
        }
        default:
            break;
    }
}

void ModelLayer::setHair(const std::string& hair, const cocos2d::Vec2& position)
{
    if (!_hair) {
        _hair = Sprite::create();
        addChild(_hair, 1);
    }
    _hair->setTexture(hair);
    _hair->setPosition(position);
}

void ModelLayer::setDress(const std::string& dress, const cocos2d::Vec2& position)
{
    if (!_dress) {
        _dress = Sprite::create();
        _dress->setScale(0.69);
        addChild(_dress, 5);
    }
    _dress->setTexture(dress);
    _dress->setPosition(position);
}

void ModelLayer::setBottom(const std::string& bottom, const cocos2d::Vec2& position)
{
    if (!_bottom) {
        _bottom = Sprite::create();
        _bottom->setScale(0.69);
        addChild(_bottom, 4);
    }
    _bottom->setTexture(bottom);
    _bottom->setPosition(position);
}

void ModelLayer::setTop(const std::string& top, const cocos2d::Vec2& position)
{
    if (!_top) {
        _top = Sprite::create();
        _top->setScale(0.69);
        addChild(_top, 5);
    }
    _top->setTexture(top);
    _top->setPosition(position);
}

void ModelLayer::setAccessory(const std::string& accessory, const cocos2d::Vec2& position)
{
    if (!_accessory) {
        _accessory = Sprite::create();
        addChild(_accessory, 4);
    }
    _accessory->setTexture(accessory);
    _accessory->setPosition(position);
}

void ModelLayer::setShoes(const std::string& accessory, const cocos2d::Vec2& position)
{
    if (!_shoes) {
        _shoes = Sprite::create();
        _shoes->setScale(0.69);
        addChild(_shoes, 6);
    }
    _shoes->setTexture(accessory);
    _shoes->setPosition(position);
}

