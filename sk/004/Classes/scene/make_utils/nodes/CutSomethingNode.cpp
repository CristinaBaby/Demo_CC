//
//  CutSomethingNode.cpp
//  BHUG1064
//
//  Created by maxiang on 5/31/16.
//
//

#include "CutSomethingNode.h"
#include "component/actions/ActionHelper.h"
#include "component/makeComponent/combinedComponent/ComponentCutter.h"
#include "CutterGuideLine.h"
#include "SoundConfig.h"

CutSomethingNode::~CutSomethingNode()
{
    
}

CutSomethingNode* CutSomethingNode::create(std::vector<Vec2> cutterOffsetVector,
                                           const std::string& fileFormat,
                                           int startIndex,
                                           int endIndex)
{
    CutSomethingNode *pRet = new CutSomethingNode();
    if (pRet && pRet->init(cutterOffsetVector, fileFormat, startIndex, endIndex))
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

bool CutSomethingNode::init(std::vector<Vec2> cutterOffsetVector,
                            const std::string& fileFormat,
                            int startIndex,
                            int endIndex)
{
    if (!MakeBaseNode::init())
        return false;
    _cutterOffsetVector = cutterOffsetVector;
    _startIndex = startIndex;
    _endIndex = endIndex;
    _partIndex = 0;

    _count = endIndex - startIndex + 1;
    auto zorder = (_count - 1) * 2 + 1;
    for (int i = 1; i <= _count; ++i)
    {
        auto partSprite = Sprite::create(StringUtils::format(fileFormat.c_str(), i));
        //-2是为了穿插刀具
        addChild(partSprite, zorder, i);
        zorder -= 2;
        
        _mainContent = partSprite;
    }
    
    return true;
}

Sprite* CutSomethingNode::getContent(int tag)
{
    return dynamic_cast<Sprite*>(getChildByTag(tag));
}

void CutSomethingNode::setKnife(const std::string& file)
{
    if (_knife)
        _knife->removeFromParent();
    
    _knife = Sprite::create(file);
    _knife->setVisible(false);
    addChild(_knife);
}

void CutSomethingNode::setKnifeZorder(int zorder)
{
    if (!_knife)
        return;
    _knife->setLocalZOrder(zorder);
}

void CutSomethingNode::beginAutoCut()
{
    if (!_knife)
        return;
    _knife->removeAllComponents();
    
    auto height = _mainContent->getContentSize().height;
    ComponentCutter::CutModeAutoConfig cutConfig = {height, 0.3, 1};
    auto cutComp = ComponentCutter::create();
    _knife->addComponent(cutComp);
    cutComp->setCutMode(ComponentCutter::CutMode::kCutModeAuto);
    cutComp->setAutoCutConfig(cutConfig);
    
    _partIndex = _cutterOffsetVector.size();
    setupCutter();
    
    auto touchComp = ComponentTouchMove::create();
    _knife->addComponent(touchComp);
    touchComp->setIsCanMove(false);
    touchComp->setName("ComponentTouchMove");
    touchComp->touchEndedCall = [=](Touch* touch)
    {
        touchComp->setEnabled(false);
        cutComp->start();
        SoundConfig::getInstance()->playLoopEffect(kSoundCut);

    };
    cutComp->cutFinishCallBack = [=]()
    {
        touchComp->setEnabled(true);
        finishCutOnePart();
        
        SoundConfig::getInstance()->stopLoopEffect();
    };
    touchComp->start();
}

void CutSomethingNode::showGuideLine(const Vec2& position)
{
    auto guideLine = CutterGuideLine::create();
    guideLine->setTag(1234);
    guideLine->setPosition(position + Vec2(0.0, -60.0));
    guideLine->start();
    addChild(guideLine, 100);
}

void CutSomethingNode::removeGuideLine()
{
    removeChildByTag(1234);
}

void CutSomethingNode::setupCutter()
{
    if (!_knife)
        return;
    auto position = _cutterOffsetVector.at(_partIndex - 1);
    auto zorder = (_count - _partIndex) * 2;
    
    _knife->setPosition(position + Vec2(96.0, -84.0));
    _knife->setLocalZOrder(zorder);
    _knife->setVisible(true);
    
    showGuideLine(position);

    if (setupKnifeCallback)
        setupKnifeCallback(_partIndex, _knife);
}

void CutSomethingNode::beginDragCut()
{
    if (!_knife)
        return;
    
    _knife->removeAllComponents();
    
    auto height = _mainContent->getContentSize().height;
    ComponentCutter::CutModeDragConfig cutConfig = {height, 20.0, 10.0, 10.0, 1.0};
    auto cutComp = ComponentCutter::create();
    _knife->addComponent(cutComp);
    cutComp->setCutMode(ComponentCutter::CutMode::kCutModeDrag);
    cutComp->setDragCutConfig(cutConfig);
    cutComp->start();
    
    _partIndex = _cutterOffsetVector.size();
    setupCutter();

    cutComp->_dragCutting = [=]
    {
        //sound
        SoundConfig::getInstance()->playLoopEffect(kSoundCut);
    };
    cutComp->_dragCuttingStop = [=]
    {
        kAudioUtil->stopAllEffect();
    };
    cutComp->cutFinishCallBack = [=]()
    {
        finishCutOnePart();
        SoundConfig::getInstance()->stopLoopEffect();
        this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=](){
            
            cutComp->start();
        }), NULL));
    };
}

void CutSomethingNode::finishCutOnePart()
{
    removeGuideLine();

    int index = 1;
    while (index <= _partIndex) {
        
        auto part = getChildByTag(index);
        part->runAction(MoveBy::create(0.3, Vec2(-40.0, 0.0)));
        ++index;
    }
    
    --_partIndex;
    if (_partIndex <= 0)
    {
        removeGuideLine();
        if (finishCutCallback)
            finishCutCallback();
        return;
    }
 
    runAction(Sequence::create(DelayTime::create(0.4), CallFunc::create([=](){
        
        setupCutter();
    }), NULL));
}




