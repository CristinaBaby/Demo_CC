//
//  MultiSprite.cpp
//  Milkshake
//
//  Created by luotianqiang1 on 4/13/15.
//
//

#include "MultiSprite.h"
#include <math.h>
USING_NS_KD;
MultiSprite::MultiSprite() {
    moveRect.size= Director::getInstance()->getWinSize();
    moveRect.origin = Vec2::ZERO;
    touchDirection = Vec2::ZERO;
    touchPoint = Vec2::ZERO;
    maxScale = 1.5;
    minScale = 0.4;
    length = 0;
}
MultiSprite::~MultiSprite() {
    
}

 bool MultiSprite::initWithFile(const std::string& filename) {
     auto flag = EventSprite::initWithFile(filename);
     focusRect = Rect(0,0,getContentSize().width,getContentSize().height);
     return flag;
}

void MultiSprite::setFocusrect(Rect rect) {
    this->focusRect = rect;
}

void MultiSprite::setMoveRect(Rect rect) {
    this->moveRect = rect;
}

void MultiSprite::setCenterPoint(Vec2 point) {
    centerPoint.clear();
    centerPoint.push_back(point);
}

MultiSprite* MultiSprite::create(const std::string& filename) {
    auto *sprite = new (std::nothrow) MultiSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MultiSprite* MultiSprite::createWithTex(Texture2D* tex) {
    auto *sprite = new (std::nothrow) MultiSprite();
    if (sprite && sprite->initWithTexture(tex))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

 MultiSprite* MultiSprite::createWithTex(Texture2D* tex,Rect r){
     auto *sprite = new (std::nothrow) MultiSprite();
     if (sprite && sprite->initWithTexture(tex, r))
     {
         sprite->autorelease();
         return sprite;
     }
     CC_SAFE_DELETE(sprite);
     return nullptr;
}

bool MultiSprite::onTouchBegan(Touch*pTouch, Event *pEvent) {
    if(!focusRect.containsPoint(pTouch->getLocation())) {
        if(_touches.size()==0)
            return false;
        else
            return true;
    }
    if(_touches.size()>=2)
        return true;
    
    if(std::find(_touches.begin(), _touches.end(), pTouch) == _touches.end())
        _touches.push_back(pTouch);
    if(_touches.size() ==1) {
        if(ornPosition.size() == 0) {
            ornPosition.push_back(getParent()->convertToWorldSpace(getPosition()));
            ornPosition.push_back(getAnchorPoint());
            ornScale = getScale();
        }
        touchPoint = pTouch->getLocation();
        touchCache.clear();
        touchCache.push_back(touchPoint);
    }
    else if(_touches.size() == 2) {
        touchPoint = _touches[0]->getLocation().getMidpoint(_touches[1]->getLocation());
        length = _touches[0]->getLocation().distance(_touches[1]->getLocation());
        touchDirection = _touches[0]->getLocation()-_touches[1]->getLocation();
    }
    return true;
}
void MultiSprite::onTouchMoved(Touch*pTouch, Event *pEvent) {
    if(std::find(_touches.begin(), _touches.end(), pTouch) != _touches.end()) {
        if(_touches.size() == 1) {
//            auto newPoint = pTouch->getLocation();
//            if(touchCache.size()<3){
//                for(auto point:touchCache){
//                    if(point == newPoint)
//                        return;
//                }
//                touchCache.push_back(newPoint);
//            } else if(touchCache.size()==3){
//                move(touchCache[2]-touchPoint);
//                touchPoint = touchCache[2];
//                touchCache.clear();
//            }
            auto newPoint = pTouch->getLocation();
            auto disance = newPoint-touchPoint;
            touchPoint = newPoint;
            move(disance);
        }
        else if(_touches.size() == 2) {
            
            auto newPoint = _touches[0]->getLocation().getMidpoint(_touches[1]->getLocation());
            auto disance = newPoint-touchPoint;
            touchPoint = newPoint;
            
            auto curentLen =  _touches[0]->getLocation().distance(_touches[1]->getLocation());
            auto distanceLen = curentLen-length;
            length = curentLen;
         
            
            auto curentDirection = _touches[0]->getLocation()-_touches[1]->getLocation();
            auto degree = Vec2::angle(curentDirection, touchDirection);
            Vec3 v1 = Vec3(curentDirection.x,curentDirection.y,0);
            Vec3 v2 = Vec3(touchDirection.x,touchDirection.y,0);
            v1.cross(v2);
            if(v1.z<0)
                degree=-degree;
            touchDirection = curentDirection;
            
            auto ro = degree/3.1415926*180;
            log("ro==%f,dis==%f",degree*distanceLen,distanceLen);
            rote(ro);
            scale(distanceLen);
        }
    }
}

void  MultiSprite::rote(float distacedegree) {
    auto touchPoint = this->touchPoint;
    if(centerPoint.size()>0)
        touchPoint = centerPoint.at(0);
    auto touchInNode = convertToNodeSpace(touchPoint);
    setAnchorPoint(Vec2(touchInNode.x/getContentSize().width,touchInNode.y/getContentSize().height));
    auto t2 = convertToWorldSpace(touchInNode);
    setPosition(getPosition()-t2+touchPoint);
    setRotation(getRotation()+distacedegree);
}

void MultiSprite::scale(float distace) {
    auto touchPoint = this->touchPoint;
    if(centerPoint.size()>0)
        touchPoint = centerPoint.at(0);
    
    auto scale = distace/Vec2(getContentSize()).getLength();
    auto newscale = scale+getScale();
    if(newscale>maxScale||newscale<minScale)
        return;
    auto center = convertToNodeSpace(touchPoint);
    setScale(newscale);
    auto newCenter = convertToWorldSpace(center);
    setPosition(getPosition()-newCenter+touchPoint);
}

void MultiSprite::resetSprite() {
    if(ornPosition.size()>1) {
        setPosition(getParent()->convertToNodeSpace(ornPosition.at(0)));
        setAnchorPoint(ornPosition.at(1));
        setScale(ornScale);
        setRotation(0);
    }
 
}

void MultiSprite::move(Vec2 distance) {
    auto p = convertToWorldSpace(getContentSize()*.5)+distance;
    
    if(distance.y>0 && p.y>moveRect.getMaxY())
            return;
    else if(distance.y<0&& p.y<moveRect.getMinY())
            return;
    if(distance.x>0&&p.x>moveRect.getMaxX())
            return;
    else if(distance.x<0&&p.x<moveRect.getMinX())
            return;
    
//    auto bootomLeft = convertToWorldSpace(distance);
//    auto topRight =convertToWorldSpace(distance+getContentSize());
//    if(distance.y>0 && bootomLeft.y>moveRect.getMinY())
//        return;
//    else if(distance.y<0&& topRight.y<moveRect.getMaxY())
//        return;
//    if(distance.x>0&&bootomLeft.x>moveRect.getMinX())
//        return;
//    else if(distance.x<0&&topRight.x<moveRect.getMaxX())
//        return;
    setPosition(getPosition()+distance);
    //runAction(MoveBy::create(0, distance));
}

void MultiSprite::onTouchEnded(Touch* pTouch, Event *pEvent) {
    auto it = std::find(_touches.begin(), _touches.end(), pTouch);
    if(it != _touches.end()) {
        _touches.erase(it);
        if(_touches.size() == 1)
            touchPoint = _touches[0]->getLocation();
        else if(_touches.size() == 0)
            touchCache.clear();
    }
    
  
}
void MultiSprite::onTouchCancle(Touch* pTouch, Event *pEvent) {
    auto it = std::find(_touches.begin(), _touches.end(), pTouch);
    if(it != _touches.end()) {
        _touches.erase(it);
        if(_touches.size() == 1)
            touchPoint = _touches[0]->getLocation();
        else if(_touches.size() == 0)
            touchCache.clear();
    }
    
}

