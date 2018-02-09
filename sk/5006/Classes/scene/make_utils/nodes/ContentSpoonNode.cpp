//
//  ContentSpoonNode.cpp
//  BHUG1064
//
//  Created by maxiang on 5/25/16.
//
//

#include "ContentSpoonNode.h"
#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"

ContentSpoonNode::~ContentSpoonNode()
{
    
}

ContentSpoonNode* ContentSpoonNode::create(const std::string& spoon, const std::string& file)
{
    ContentSpoonNode *pRet = new ContentSpoonNode();
    if (pRet && pRet->init(spoon, file))
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

bool ContentSpoonNode::init(const std::string& spoon, const std::string& file)
{
    if (!MakeBaseNode::init())
        return false;
    
    _spoon = Sprite::create(spoon);
    
    if (file.empty())
        _content = nullptr;
    else
        _content = Sprite::create(file);

    addChild(_spoon, 1);
    
    if (_content)
        addChild(_content, INT_MAX);

    _mainContent = _spoon;
    
    _particlePosition = Vec2(-getContentSize().width/2, -getContentSize().height/2 + 20.0);
    return true;
}

void ContentSpoonNode::setContent(const std::string& file)
{
    if (_content)
    {
        _content->removeFromParent();
        _content = nullptr;
    }
    
    if (file.empty())
        return;
    _content = Sprite::create(file);
    addChild(_content, INT_MAX);
}

void ContentSpoonNode::setContent(Sprite *content)
{
    if (_content)
        _content->removeFromParent();
    
    _content = content;
    addChild(_content, INT_MAX);
}

Sprite* ContentSpoonNode::getContent()
{
    return _content;
}

void ContentSpoonNode::setEnableMove(bool enable)
{
    if (_compMove)
    {
        _compMove->setEnableMove(enable);
    }
}

void ContentSpoonNode::setTapCall(const std::function<void()>& showCallback)
{
    
    if (_compMove)
    {
        _compMove->touchEndedCall=[=](Touch *t)
        {
            if (showCallback)
            {
                showCallback();
            }
            
        };
    }

}


void ContentSpoonNode::startDragToWorldPositonPour(const Vec2& worldPoint,
                                                   const std::string& particleFile,
                                                   const std::function<void()>& showCallback)
{
    _originalPosition = getPosition();
    
    removeComponent(_compMove);
    _compMove = ComponentTouchMoveToTarget::create(worldPoint);
    _compMove->setMaxDistanceForMoveToTarget(_moveToTargetTriggerDistance);
    addComponent(_compMove);
    _compMove->start();
    
    if (_enableTouchZoom)
    {
        _compMove->touchBeganCall = [=](Touch* touch)
        {
            this->runAction(ScaleTo::create(0.3, 1.2));
        };
        
        _compMove->touchEndedCall = [=](Touch* touch)
        {
            this->runAction(ScaleTo::create(0.3, 1.0));
        };
    }
    
    _compMove->componentTouchMoveToTargetDoneCall = [=]()
    {
        this->runAction(Sequence::create(RotateBy::create(0.3, _rotateAngel), CallFunc::create([=](){
 
            //particle
            ParticleSystemQuad *particle = nullptr;
            if (!particleFile.empty())
            {
                particle = ParticleSystemQuad::create(particleFile);
                particle->setPosition(_particlePosition);
                this->addChild(particle, 100);
            }
            
            if (_content)
            {
                _content->runAction(Sequence::create(FadeOut::create(1.0), CallFunc::create([=](){
                    
                    if (showCallback)
                        showCallback();
                    
                    if (particle)
                        particle->removeFromParent();
                }), nullptr));
            }
            else
            {
                if (showCallback)
                    showCallback();
            }
        }), DelayTime::create(1.0),
            RotateBy::create(0.3, -_rotateAngel),
            MoveTo::create(0.3, _originalPosition),
            CallFunc::create([=](){
            
            if (backToOriginalPositionCallback)
                backToOriginalPositionCallback();
        }), NULL));
    };
}





