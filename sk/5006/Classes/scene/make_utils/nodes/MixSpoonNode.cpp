//
//  MixSpoonNode.cpp
//  BHUG1064
//
//  Created by maxiang on 5/27/16.
//
//

#include "MixSpoonNode.h"
#include "component/makeComponent/basicComponent/ComponentTouchMove.h"
#include "component/makeComponent/basicComponent/ComponentMixer.h"
#include "component/actions/ActionHelper.h"
#include "SoundConfig.h"
#include "LockUIManager.h"
#include "RewardManager.h"
#include "MiniShopScene.h"
#include "ShopScene.h"
#include "CMVisibleRect.h"

MixSpoonNode::~MixSpoonNode()
{
}

MixSpoonNode* MixSpoonNode::create(const std::string& file)
{
    MixSpoonNode *pRet = new MixSpoonNode();
    if (pRet && pRet->init(file))
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

bool MixSpoonNode::init(const std::string& file)
{
    if (!MakeBaseNode::init())
        return false;
    
    _spoon = Sprite::create(file);
    _spoonCenter = Vec2(-105, -45);
    addChild(_spoon, 1);
    
    _mainContent = _spoon;
    
    _animation = RepeatForever::create(ActionHelper::createRollJelly());
    _animation->retain();
    return true;
}

void MixSpoonNode::addlock()
{
    auto key = kRewardManager->getItemKey("rainbowcupcake", "tool", 1);
    this->setUserObject(__String::create(key));
    if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllFoodString(), "tool", 1, this))
    {
        LockUIManager::getInstance()->registerLock("rainbowcupcake", "tool", 1, this,Vec2(110,120));
    }
}

Sprite* MixSpoonNode::setMixAnimate(const std::string& format,
                                    int start,
                                    int end)
{
    if (_mixAnimationSprite)
        _mixAnimationSprite->removeFromParent();
    
    _mixAnimationSprite = Sprite::create(StringUtils::format(format.c_str(), start));
    _mixAnimationSprite->setPosition(Vec2(-_spoon->getContentSize().width/2, -_spoon->getContentSize().height/2));
    _mixAnimationSprite->setVisible(false);
    addChild(_mixAnimationSprite, 2);
    
    auto animate = ActionHelper::createAnimate(format, start, end);
    _mixAnimationSprite->runAction(RepeatForever::create(animate));
    
    return _mixAnimationSprite;
}

void MixSpoonNode::startMix(const Rect& worldArea,
                            Sprite *mixSprite,
                            const std::string& mixFileFormat,
                            int start,
                            int end,
                            bool needRotate)
{
    static int intervalIndex = 0;//100个进度分成的N个单位的索引
    static int startIndex = 1;//图片第一张索引
    
    auto mixComp = ComponentMixer::create();
    addComponent(mixComp);
    mixComp->setMixDoneLength(_mixLength);
    mixComp->setEffectiveArea(worldArea);
    mixComp->start();
    
    _mixContentSprite = mixSprite;
    intervalIndex = 0;
    startIndex = start;
    
    int count = end - start + 1;//有多少张图片，小于2返回
    if (count < 2 && !mixFileFormat.empty())
        return;
    float interval = 100.0 / count;//这么多图片每隔多少进度显示一张
    static int rotateA = 0;
    
    mixComp->componentMixCallback = [=](float completePercent){
        
        if (mixPercentCallback)
            mixPercentCallback(completePercent);
        if (needRotate)
            mixSprite->setRotation(rotateA++);
        
        if (intervalIndex <= count)
        {
            if (completePercent > interval * intervalIndex &&
                completePercent <= interval * (intervalIndex + 1))
            {
                if(!mixFileFormat.empty())
                {
                    mixSprite->setOpacity(255);
                    auto file = StringUtils::format(mixFileFormat.c_str(), startIndex++);
                    auto sprite = Sprite::create(file);
                    sprite->setOpacity(0);
                    sprite->runAction(FadeIn::create(0.3));
                    sprite->setPosition(mixSprite->getContentSize()/2);
                    mixSprite->addChild(sprite);
                    
                    mixSprite->setCascadeOpacityEnabled(false);
                    mixSprite->runAction(Sequence::create(FadeOut::create(0.5), CallFunc::create([=](){
                        if (mixChanged)
                            mixChanged();
                        
                        sprite->removeFromParent();
                        mixSprite->setTexture(file);
                        mixSprite->setOpacity(255);
                    }), NULL));
                    intervalIndex++;
                }
            }
        }
    };
    
    if (mixPaused)
        mixComp->_componentMixPaused = mixPaused;
    
    _cTouch = ComponentTouchMove::create();
    addComponent(_cTouch);
    _cTouch->start();
    _cTouch->touchMovedCall = [=](Touch* touch)
    {
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllFoodString());
        if (!hasPurchased)
        {
            if (kIAPManager->isFree("tool", 1))
            {
                auto keyObject = this->getUserObject();
                if(keyObject)
                {
                    string key = ((__String*)(keyObject))->getCString();
                    if(kRewardManager->isLocked(key))
                    {
                        return;
                    }
                }
            }
        }
        auto location = _spoon->convertToWorldSpaceAR(_spoonCenter);
        if (worldArea.containsPoint(location)){
            
            if (_mixAnimationSprite)
                _mixAnimationSprite->setVisible(true);
        }
        else
        {
            if (_mixAnimationSprite)
                _mixAnimationSprite->setVisible(false);
        }
    };
    _cTouch->touchBeganCall = [=](Touch* t)
    {
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllFoodString());
        if (!hasPurchased)
        {
            if (kIAPManager->isFree("tool", 1))
            {
                auto keyObject = this->getUserObject();
                if(keyObject)
                {
                    string key = ((__String*)(keyObject))->getCString();
                    if(kRewardManager->isLocked(key))
                    {
                        if (this->getChildByName(LOCK_NAME_REWARD))
                        {
                            static bool s_isClicked = false;
                            if (s_isClicked)
                            {
                                return;
                            }
                            s_isClicked = true;
                            this->runAction(Sequence::create(DelayTime::create(0.5f),
                                                             CallFunc::create([=]
                                                                              {
                                                                                  s_isClicked = false;
                                                                              }),
                                                             NULL));

                            auto node = MiniShopScene::create();
                            this->getParent()->addChild(node,9999);
                            node->setPosition(CMVisibleRect::getPosition(0, 0));
                            node->showAllFoodButton();
                            node->onCloseCallback = []
                            {
                                LockUIManager::getInstance()->updateLockStatus();
                            };
                            node->_onRewardButtonClicked = [=]
                            {
                                kRewardManager->showRewardAds(key);
                            };
                            node->_onShopButtonClicked = [=]
                            {
                                ShopScene* layer = ShopScene::create();
                                layer->setPosition(CMVisibleRect::getPosition(0, 0));
                                _director->getRunningScene()->addChild(layer,9999);
                            };
                        }
                        _cTouch->setIsCanMove(false);
                        return;
                    }
                }
            }
        }
    
    
        if (_enableAnimation)
            mixSprite->runAction(_animation);
    
        if (mixerTouchBegan)
            mixerTouchBegan(this->getTag());
    };
    _cTouch->touchEndedCall = [=](Touch* t)
    {
        _cTouch->setIsCanMove(true);

        SoundConfig::getInstance()->stopLoopEffect();
        
        if (_enableAnimation)
            mixSprite->getActionManager()->removeAction(_animation);
        
        if (_mixAnimationSprite)
            _mixAnimationSprite->setVisible(false);
    };
}

void MixSpoonNode::setAutoConfig()
{
    _spoon->setTexture("res/cake_pop/blender/blende1_1.png");
    _drill = Sprite::create("res/cake_pop/blender/blender1.png");
    _spoon->addChild(_drill);
    _drill->setPosition(142,322);

    _drillAnimation = RepeatForever::create(ActionHelper::createAnimate("res/cake_pop/blender/blende1_%d.png", 1, 3));
    
    _drillAnimation->retain();
    
}

void MixSpoonNode::startDrill()
{
    if (!_drill)
    {
        return;
    }
    if (_first)
    {
        _spoon->runAction(_drillAnimation);
        _first = false;
    }
    else
    {
        _actionManager->resumeTarget(_spoon);
    }
}

void MixSpoonNode::pauseDrill()
{
    if (!_drill)
    {
        return;
    }

    _actionManager->pauseTarget(_spoon);
}

void MixSpoonNode::setMixMoveRect(Rect r)
{
    _moveRect = r;
    _cTouch->setMoveArea(_moveRect);
}

void MixSpoonNode::onExit()
{
    MakeBaseNode::onExit();
    SoundConfig::getInstance()->stopLoopEffect();
    if(_animation)
        _mixContentSprite->getActionManager()->removeAction(_animation);
    CC_SAFE_RELEASE_NULL(_animation);
 
}




