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

MixSpoonNode::~MixSpoonNode()
{
    SoundConfig::getInstance()->stopLoopEffect();
    
    _mixContentSprite->getActionManager()->removeAction(_animation);
    CC_SAFE_RELEASE_NULL(_animation);
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
    mixComp->setMixDoneLength(40000);
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
    
    auto moveComp = ComponentTouchMove::create();
    addComponent(moveComp);
    moveComp->start();
    moveComp->touchMovedCall = [=](Touch* touch)
    {
        SoundConfig::getInstance()->playLoopEffect("Liquid mixing.mp3");
        
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
    moveComp->touchBeganCall = [=](Touch* t)
    {
        if (_enableAnimation)
            mixSprite->runAction(_animation);
    };
    moveComp->touchEndedCall = [=](Touch* t)
    {
        SoundConfig::getInstance()->stopLoopEffect();
        
        if (_enableAnimation)
            mixSprite->getActionManager()->removeAction(_animation);
        
        if (_mixAnimationSprite)
            _mixAnimationSprite->setVisible(false);
    };
}

void setMixContent(const std::string& fileFormat, int count)
{
    
}





