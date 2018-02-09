//
//  CutDountComplent.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/14.
//
//

#include "CutDountComplent.h"

IMPLEMENT_COCOS2DX_CLASS(CutDountComplent);

CutDountComplent::CutDountComplent():
dough(nullptr)
,moveOperate(nullptr)
,distanceOperate(nullptr)
,dountPath("")
,maskPath("")
,oragnalNum(0)
,targetNum(0)
,isCutEnd(false)
,containerNode(nullptr){
    
}
CutDountComplent::~CutDountComplent(){
    
}
bool CutDountComplent::init(){
    if(!LQComponent::init())
        return false;
    
    moveOperate = TouchMoveComponent::create();
    moveOperate->setMoveType(MoveComponentType::kTypeBack);
    addComponent(moveOperate);
    moveOperate->addListners(ComponentTouchEnd, CC_CALLBACK_3(CutDountComplent::touchOperate,this));
    moveOperate->addListners(ComponentTouchCancle, CC_CALLBACK_3(CutDountComplent::touchOperate,this));
    
    distanceOperate = DistanceToTarget::create();
    addComponent(distanceOperate);
    distanceOperate->addListners(MoveDistancePercent, [](Node* _node,Component*,OperateListner* _lis){
        auto _per = dynAny_cast<float>(_lis->getData());
        if(_per<0)
            _per =0;
        else if(_per>1)
            _per = 1;
        
        _node->setScale(1+0.2*_per);
    });
    return true;
}

void CutDountComplent::onResume(){
    LQComponent::onResume();
    if(distanceOperate != nullptr)
        distanceOperate->startUpdate();
}

void CutDountComplent::reset(){
    for(auto _node:cutDounts)
        _node->removeFromParent();
    cutDounts.clear();
    isCutEnd = false;
    dough->stopAllActions();
    dough->setOpacity(255);
    if(containerNode != nullptr)
        containerNode->removeAllChildren();
    //    if(containerNode != nullptr){
//        containerNode->removeFromParent();
//        containerNode = nullptr;
//    }
}

void CutDountComplent::setDough(Node* _node) {
    _node->setCascadeOpacityEnabled(false);
    _node->setCascadeColorEnabled(false);
    dough = _node;
    Rect rect;
    rect.origin = _node->convertToWorldSpace(Point::ZERO) - Vec2(40,40);
    rect.size = _node->getContentSize()+Size(80,80);;
    if(moveOperate != nullptr)
        moveOperate->setTarget(LQRect::create(rect));
    if(distanceOperate != nullptr)
        distanceOperate->setTargetRect(rect);
    if(containerNode == nullptr){
        containerNode = ClippingNode::create();
        containerNode->setAlphaThreshold(0.05);
        auto _s = Sprite::createWithSpriteFrame(dynamic_cast<Sprite*>(_node)->getSpriteFrame());
        _s->setPosition(_s->getContentSize()*.5);
        containerNode->setStencil(_s);
        _node->addChild(containerNode);
    }
    
}

void CutDountComplent::setDountPath(const string&str ){
    this->dountPath = str;
    auto pImage = new Image();
    pImage->initWithImageFile(str);
    auto temp = LQMath::getOpacityNum(pImage);
    if(temp != 0)
        oragnalNum = temp;
    pImage->release();
}
void CutDountComplent::setMaskPath(const string& str) {
    this->maskPath = str;
}

void CutDountComplent::setTargetNum(int num){
    this->targetNum = num;
}

const  Vector<Sprite*>& CutDountComplent::getCutDounts(){
    return cutDounts;
}

void CutDountComplent::touchOperate(Node* _node,Component* _move,OperateListner* _lis){
    if(_lis->getKey() == ComponentTouchEnd) {
        bool flag = isCutEnd;
        if(!flag) {
            auto currentPos = _owner->getParent()->convertToWorldSpace(_owner->getPosition());
            for(auto cutDount:cutDounts){
                auto dountPos = cutDount->convertToWorldSpace(cutDount->getContentSize()*.5);
                if(dountPos.distanceSquared(currentPos)<(cutDount->getContentSize().width*cutDount->getContentSize().width)) {
                    flag = true;
                    break;
                    
                }
            }
        }
        if(flag) {
            _dispatcher->setEnabled(false);
            moveOperate->backToStart();
            return;
        }
        
        
        if(dough != nullptr) {
            distanceOperate->pauseUpdate();
            _dispatcher->setEnabled(false);
                    this->playSound();
            _owner->runAction(RotateBy::create(0.3, 360));
            _owner->runAction(Sequence::create(ScaleTo::create(0.6, 1),CallFunc::create([this](){
                if(!dountPath.empty()&&!maskPath.empty()){
            
                    auto pDount = Sprite::create(dountPath);
                    auto pMask = Sprite::create(maskPath);
                    auto maskPos = dough->convertToNodeSpace(_owner->getParent()->convertToWorldSpace(_owner->getPosition()));
                    pDount->setPosition(pDount->getContentSize()*.5);
                    pMask->setAnchorPoint(Vec2(maskPos.x/pMask->getContentSize().width,maskPos.y/pMask->getContentSize().height));
                    pMask->setPosition(pDount->getContentSize()*.5);
                    auto _render = RenderTexture::create(pDount->getContentSize().width, pDount->getContentSize().height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
                    pMask->setBlendFunc({GL_ONE,GL_ZERO});
                    pDount->setBlendFunc({GL_DST_ALPHA,GL_ZERO});
                    _render->begin();
                    pMask->visit();
                    pDount->visit();
                    _render->end();
                    Director::getInstance()->getRenderer()->render();
                    auto _image = _render->newImage();
                    auto _currentNum = LQMath::getOpacityNum(_image);
                    float _percent = _currentNum/(oragnalNum*1.0f);
                    
                    auto tempDough = Sprite::createWithSpriteFrame(pDount->getSpriteFrame());
                    tempDough ->setPosition(maskPos);
                    if(_percent<=0.98) {
                           containerNode->addChild(tempDough);
                        this->dispatcherEvent(cutDountFailed);
                        tempDough->runAction(Sequence::create(DelayTime::create(1), FadeTo::create(0.2, 0),CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, tempDough)), nullptr));
                    } else {
                        dough->addChild(tempDough);
                        this->dispatcherEvent(cutDountSuccess);
                        cutDounts.pushBack(tempDough);
                        if(cutDounts.size() == targetNum) {
                            isCutEnd = true;
                            _dispatcher->setEnabled(true);
                            this->dispatcherEvent(cutDountEnd);
                        }
                    }
                    
                    _image->release();
                    moveOperate->backToStart();
                }
            }), nullptr));
        }
    }else if(_lis->getKey() == ComponentTouchCancle){
        distanceOperate->startUpdate();
        _dispatcher->setEnabled(true);
    }
}