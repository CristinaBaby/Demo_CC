//
//  STCoverLayer.cpp
//  SpotDifference
//
//  Created by Steven.Xc.Tian.
//
//

#include "STCoverLayer.h"
#include "../gameconfig/GameConfig.h"
#include "../utilities/STVisibleRect.h"
#include "../AppConfig.h"

USING_NS_CC;



static BackStack* s_BackStack = NULL;

BackStack* BackStack::getInstance()
{
	if(!s_BackStack)
	{
		s_BackStack = new BackStack;
	}
	return s_BackStack;
}

void BackStack::destroy()
{
	if(s_BackStack)
	{
		delete s_BackStack;
		s_BackStack = nullptr;
	}
}

void BackStack::pushBackLayer(CoverLayer* node)
{
	if(m_CoverLayerStack.size() < 1 || m_CoverLayerStack[m_CoverLayerStack.size() - 1] != node)
	{
		m_CoverLayerStack.push_back(node);
	}
}

CoverLayer* BackStack::popBackLayer()
{
	if(m_CoverLayerStack.size() < 1) return nullptr;
    
	CoverLayer* top = m_CoverLayerStack[m_CoverLayerStack.size() - 1];
	m_CoverLayerStack.erase(--m_CoverLayerStack.end());
	return top;
}

CoverLayer* BackStack::getTopBackLayer()
{
	if(m_CoverLayerStack.size() < 1) return nullptr;
    
	return m_CoverLayerStack[m_CoverLayerStack.size() - 1];
}



// ===================================================================




const Color4B CoverLayer::DEFAULT_COLOR = Color4B(0, 0, 0, 180);


CoverLayer * CoverLayer::create()
{
    CoverLayer * layer = new CoverLayer();
    if(layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}


CoverLayer * CoverLayer::create(const Color4B& color)
{
    CoverLayer * layer = new CoverLayer();
    if(layer && layer->initWithColor(color))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool CoverLayer:: init()
{
    return initWithColor(DEFAULT_COLOR);
}

bool CoverLayer:: initWithColor(const Color4B& color)
{
    if (! LayerColor::initWithColor(color))
        return false;
    
    BackStack::getInstance()->pushBackLayer(this);
    
    // shield all the touch events to below nodes.
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [](Touch*, Event*) {
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    /*
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event*){
        
        if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
            CCLOG("Android back button ");
            this->closeCoverLayer(this);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
    */
    
    return true;
}

void CoverLayer:: closeCoverLayer(Ref*)
{
    if(this == BackStack::getInstance()->getTopBackLayer())
	{
        // remove from back stack
		BackStack::getInstance()->popBackLayer();
        
        if (_closedcallback) {
            _closedcallback(this);
        }
        
		this->removeFromParentAndCleanup(true);
	}
}

//Only for jigsaw
//公共方法
Sprite* CoverLayer::generateMapFrame()
{
    
    int column = sqrt(GameConfig::sharedGameConfig()->user_last_pieces_number);
    Sprite *frameSprite = Sprite::createWithSpriteFrameName("ui04_frame.png");
    frameSprite->setAnchorPoint(Vec2(0, 1));
    
    frameSprite->setPosition(V::getCenterOfScene());
    
    float scale = (1024*UI_SCALE + 20*UI_SCALE)/frameSprite->getContentSize().width;
    //    CCLOG("column=%d", column);
    if(column < 6)
        frameSprite->setScale(scale);
    else if(column < 8)
        frameSprite->setScaleY(scale*0.99f);
    else if(column < 12)
        frameSprite->setScaleY(scale*0.99f);
    else if(column<13){
        frameSprite->setScaleY(scale*0.98f);
    }
    frameSprite->setScaleX(scale);
    
    if(column == 2){
        frameSprite->setScaleY(scale*0.995);
        frameSprite->setPosition(Vec2(GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().x-12, GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().y+14));
    }else if(column == 4){
        frameSprite->setScaleY(scale*0.995);
        frameSprite->setPosition(Vec2(GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().x-12, GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().y+14));
    }else
        if( column == 10 ){
            frameSprite->setScaleX(scale*0.995);
            frameSprite->setScaleY(scale*0.99);
            frameSprite->setPosition(Vec2(GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().x-13, GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().y+14));
        }else if(column == 5 || column == 3){
            frameSprite->setPosition(Vec2(GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().x-11, GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().y+12));
        }
    
    if(column == 7){
        frameSprite->setScaleX(scale*0.995);
    }else
        if(column == 9)
            frameSprite->setScaleX(scale*0.99);
        else if(column == 12)
            frameSprite->setScaleX(scale*0.99);
        else if(column == 11)
            frameSprite->setScaleX(scale*0.99);
    
    if( column == 7 ){
        frameSprite->setPosition(Vec2(GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().x-8, GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().y+10));
    }
    return frameSprite;
}

//公共方法
Sprite* CoverLayer::generatePauseMapFrame()
{
    int column = sqrt(GameConfig::sharedGameConfig()->user_last_pieces_number);
    Sprite *frameSprite = Sprite::createWithSpriteFrameName("ui04_frame.png");
    frameSprite->setAnchorPoint(Vec2(0, 1));
    
    frameSprite->setPosition(Vec2(GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().x-10, GameConfig::sharedGameConfig()->getGameMapLeftTopPoint().y+10));
    
    float scale = (1024*UI_SCALE + 20*UI_SCALE)/frameSprite->getContentSize().width;
    frameSprite->setScale(scale);
    return frameSprite;
}






