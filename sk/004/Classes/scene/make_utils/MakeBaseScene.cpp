//
//  MakeBaseScene.cpp
//  BHUG1062
//
//  Created by maxiang on 5/5/16.
//
//

#include "MakeBaseScene.h"
#include "depends/component/screenadapter/CMVisibleRect.h"
#include "depends/component/audio/CMAudioUtil.h"
#include "SoundConfig.h"

MakeBaseScene::~MakeBaseScene()
{
    
}

bool MakeBaseScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    _contentLayer = Layer::create();
    addChild(_contentLayer, 2);

    //design offset
    _designOffsetX = CMVisibleRect::getDesignOffset().x;
    _designOffsetY = CMVisibleRect::getDesignOffset().y;
    
    _nextButton = Button::create("ui/dec/next_btn.png");
    _nextButton->setScale(0.0);
    _nextButton->setPosition(Vec2(getContentSize().width - _nextButton->getContentSize().width/2 - 20.0, getContentSize().height/2));
    _nextButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            kAudioUtil->playEffect(kSoundNext);
            
            if (onNextButtonTouchCallback)
                onNextButtonTouchCallback();
            
        }
    });
    addChild(_nextButton, 99);
    
    _resetButton = Button::create("ui/dec/reset_btn.png");
    _resetButton->setAnchorPoint(Vec2(1, 1));
    _resetButton->setPosition(CMVisibleRect::getPosition(20, 20,kBorderRight,kBorderTop));
    _resetButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            if (onResetButtonTouchCallback)
                onResetButtonTouchCallback();
            
            kAudioUtil->playEffect(kSoundNext);
        }
    });
    addChild(_resetButton, 99);

    
    return true;
}

void MakeBaseScene::setBackground(const std::string& file)
{
    if (_bgSprite)
        _bgSprite->removeFromParent();
    
    _bgSprite = Sprite::create(file);
    _bgSprite->setPosition(getContentSize() * 0.5);
    addChild(_bgSprite, 1);
}


void MakeBaseScene::addChildToContentLayer(Node *node, int zorder)
{
    if (zorder < 0)
        _contentLayer->addChild(node);
    else
        _contentLayer->addChild(node, zorder);
}

Node *MakeBaseScene::getChildInContentLayer(const std::string& name)
{
    return _contentLayer->getChildByName(name);
}

Node *MakeBaseScene::getChildInContentLayer(const int tag)
{
    return _contentLayer->getChildByTag(tag);
}

void MakeBaseScene::removeAllChidrenInContentLayer()
{
    _contentLayer->removeAllChildren();
}

void MakeBaseScene::removeChidrenInContentLayerExcept(std::vector<Node*> nodes)
{
    if (nodes.size() == 0)
        removeAllChidrenInContentLayer();
    else
    {
        for (auto node : nodes){
            node->retain();
        }
        removeAllChidrenInContentLayer();
        
        for (auto node : nodes){
            _contentLayer->addChild(node);
            node->release();
        }
    }
}

void MakeBaseScene::addNode(Node *node, float x, float y, int zorder)
{
    if (!node)
        return;
    node->setPosition(Vec2(x, y));
    addChild(node, zorder);
}

void MakeBaseScene::showNextButton()
{
    if (_nextButton->getScale() > 0.1)
        return;
    _nextButton->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 1.2), ScaleTo::create(0.3, 1.0), NULL)));
    _nextButton->runAction(Sequence::create(EaseExponentialInOut::create(ScaleTo::create(0.5, 1.0)), NULL));
}

void MakeBaseScene::hideNextButton()
{
    _nextButton->getActionManager()->removeAllActionsFromTarget(_nextButton);
    _nextButton->setScale(0.0);
}

