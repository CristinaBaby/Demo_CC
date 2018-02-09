//
//  AddSprinkle.cpp
//  DoutCakeMaker
//
//  Created by luotianqiang1 on 16/8/8.
//
//

#include "AddSprinkle.h"
#include "PicHelp.h"

void AddSprinkle::setSprinklePath(const vector<string>& temp){
    sprinklePath = temp;
}

void AddSprinkle::caculatePos(){
    if(getRenderNode())
        caculatePos(getRenderNode()->getContentSize());
}

void AddSprinkle::caculatePos(const Size& parentSize){
    this->parentSize = parentSize;
    auto parent = getRenderNode();
    if(parent != nullptr) {
        auto render = RenderTexture::create(parentSize.width, parentSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        auto an =parent->getAnchorPoint();
        auto pos = parent->getPosition();
        parent->setAnchorPoint(Vec2::ZERO);
        parent->setPosition(Vec2::ZERO);
        render->begin();
        parent->visit();
        render->end();
        Director::getInstance()->getRenderer()->render();
        parent->setAnchorPoint(an);
        parent->setPosition(pos);
        PicHelp::caluteImage(render, maxYs, minYs);
        
    }
}

void AddSprinkle::setEnable(bool flag){
    enable = flag;
    if(lis != nullptr)
        lis->setEnabled(enable);
}

void AddSprinkle::setSwallowTouches(bool flag){
    isSwallowTouches = flag;
    if(lis != nullptr)
        lis->setSwallowTouches(isSwallowTouches);
}

bool AddSprinkle::init(){
    if(Node::init()) {
        enable = true;
        _sprinkleScale = 1;
        isSwallowTouches = false;
        lis = nullptr;
        touchCallBack = nullptr;
        
        return true;
    }
    return false;
}

void AddSprinkle::onEnter(){
    Node::onEnter();
    if(lis  == nullptr){
        lis = EventListenerTouchOneByOne::create();
        lis->setEnabled(enable);
        lis->setSwallowTouches(isSwallowTouches);
        lis->onTouchBegan = CC_CALLBACK_2(AddSprinkle::touchBegan, this);
        lis->onTouchEnded = CC_CALLBACK_2(AddSprinkle::touchEnded, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis, this);
    }
}

bool AddSprinkle::touchBegan(Touch* touch,Event*){
    auto pos = getRenderNode()->convertToNodeSpace(touch->getLocation());
    Rect limit;
    limit.origin = (getRenderNode()->getContentSize()-parentSize)*0.5;
    limit.size.width = parentSize.width;
    limit.size.height = getRenderNode()->convertToNodeSpace(Director::getInstance()->getVisibleOrigin()+ Director::getInstance()->getVisibleSize()).y;
    return limit.containsPoint(pos) && sprinklePath.size()>0;
}

void AddSprinkle::touchEnded(Touch* touch,Event*){
    auto pos = getRenderNode()->convertToNodeSpace(touch->getLocation());
    Rect limit;
    limit.origin = (getRenderNode()->getContentSize()-parentSize)*0.5;
    limit.size.width = parentSize.width;
    limit.size.height = getRenderNode()->convertToNodeSpace(Director::getInstance()->getVisibleOrigin()+ Director::getInstance()->getVisibleSize()).y;
    if(limit.containsPoint(pos)&& sprinklePath.size()>0) {
        if(touchCallBack != nullptr)
            touchCallBack(touch);
        
        map<float,Sprite*> falls;
        
        int minx =0;
        int maxx = maxYs.size()-1;
        for(int i=0;i<minYs.size();i++){
            if(minYs[i]!=0||maxYs[i] !=0){
                minx = i;
                break;
            }
        }
        
        for(int i=minYs.size()-1;i>=0;i--){
            if(minYs[i]!=0||maxYs[i] !=0){
                maxx = i;
                break;
            }
        }
        int  touchY = pos.y;
        
        
        minx +=10;
        maxx -=10;
        for(int i=0;i<15;i++){
            Sprite* s = nullptr;
            auto path =  sprinklePath.at(rand()%sprinklePath.size());
            auto pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(path);
            if(pFrame != nullptr)
                s = Sprite::createWithSpriteFrame(pFrame);
            else
                s = Sprite::create(path);
            s->setRotation(rand()%360);
            int x = minx+rand()%(maxx - minx-1);
            Vec2 startPos = Vec2(x, touchY);
            s->setPosition(startPos);
            s->setScale(_sprinkleScale);
            s->setVisible(false);
            Vec2 endPos;
            endPos.x = startPos.x;
            int maxY = std::min(touchY,maxYs[x]);
            if(minYs[x]>=maxY) {
                endPos.y = minYs[x];
            }else if(maxY-120<minYs[x])
                endPos.y = minYs[x] +rand()%(maxY-minYs[x]);
            else
                endPos.y = maxY- rand()%120;
            
            falls.insert(std::pair<float, Sprite*>(endPos.y,s));
        }
        int i=0;
        for(auto temp = falls.begin();temp != falls.end();temp++){
            if(temp->second->getPositionY()>=temp->first){
                addChild(temp->second);
                temp->second->runAction(Sequence::create(DelayTime::create(0.15*(i/4)), Show::create(),EaseIn::create(MoveTo::create(0.6, Vec2(temp->second->getPositionX(),temp->first)),1.2),nullptr));
            }
            i++;
        }
        
    }
}