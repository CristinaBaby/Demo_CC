//
//  FrameRead.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang on 16/2/12.
//
//

#include "FrameStatus.h"

USING_NS_CC;
using namespace std;
using namespace cocostudio;
using namespace timeline;
const cocos2d::Vector<cocostudio::timeline::Frame*> FrameStatus::zero;

FrameStatus::FrameStatus():_searchAction(nullptr){
    
}

FrameStatus::~FrameStatus(){
    CC_SAFE_RELEASE_NULL(_searchAction);
}

Vector<cocostudio::timeline::Frame*> FrameStatus::searchFrame(const std::string& animationName){
    Vector<cocostudio::timeline::Frame*> temps;
    do{
        if(!_searchAction){
            CCLOG("_searchAction is null");
            break;
        }
        if(!_searchAction->IsAnimationInfoExists(animationName)){
            CCLOG("animation %s not found",animationName.c_str());
            break;
        }
        auto index = _searchAction->getAnimationInfo(animationName).startIndex;
        auto timelines = _searchAction->getTimelines();
        for(auto timeline:timelines){
            for(auto frame:timeline->getFrames()){
                auto node = frame->getNode();
                if(node!= nullptr&& node->getName().compare(nodeName) == 0 && frame->getFrameIndex() == index)
                    temps.pushBack(frame);
            }
        }
    }while (0);
    return temps;
    
    //    if(_searchAction == nullptr)
    //        return FrameStatus::zero;
    //    auto itor = caches.find(animationName);
    //    if(itor != caches.end())
    //        return itor->second;
    //    if(!_searchAction->IsAnimationInfoExists(animationName))
    //        return FrameStatus::zero;
    //    auto index = _searchAction->getAnimationInfo(animationName).startIndex;
    //    Vector<cocostudio::timeline::Frame*> temps;
    //    auto timelines = _searchAction->getTimelines();
    //    for(auto timeline:timelines){
    //        for(auto frame:timeline->getFrames()){
    //            auto node = frame->getNode();
    //            if(node!= nullptr&& node->getName().compare(nodeName) == 0 && frame->getFrameIndex() == index)
    //                temps.pushBack(frame);
    //        }
    //    }
    //    caches.insert(decltype(caches)::value_type(animationName,temps));
    //    return caches[animationName];
}


void FrameStatus::remove(){
    do{
        CC_BREAK_IF(!_searchAction);
        auto timelines = _searchAction->getTimelines();
        for(auto timeline:timelines){
            for(auto frame:timeline->getFrames()){
                auto node = frame->getNode();
                if(node != nullptr&& node->getName().compare(nodeName) == 0)
                    frame->setNode(nullptr);
            }
        }
    }while (0);
}

Vec2 FrameStatus::searchPostion(const string& animationName){
    Vec2 pos;
    auto pFrames = searchFrame(animationName);
    for(auto f:pFrames){
         auto posF = dynamic_cast<PositionFrame*>(f);
        if(posF){
            pos = posF->getPosition();
            break;
        }
    }
    return pos;
}

ActionInterval* FrameStatus::createAction(float time,const std::string& animationName){
    ActionInterval* temp = nullptr;
    auto frames = searchFrame(animationName);
    Vector<FiniteTimeAction*> arrays;
    for(auto pFrame:frames){
        auto pos = dynamic_cast<PositionFrame*>(pFrame);
        if(pos != nullptr) {
            arrays.pushBack(MoveTo::create(time, pos->getPosition()));
        } else {
            auto rotate = dynamic_cast<RotationFrame*>(pFrame);
            if(rotate != nullptr){
                arrays.pushBack(RotateTo::create(time, rotate->getRotation()));
            } else {
                auto scale = dynamic_cast<ScaleFrame*>(pFrame);
                if(scale != nullptr){
                    arrays.pushBack(ScaleTo::create(time, scale->getScaleX(), scale->getScaleY()));
                } else {
                    auto sckew = dynamic_cast<SkewFrame*>(pFrame);
                    if(sckew != nullptr) {
                        if(abs(sckew->getSkewX() - sckew->getSkewY())<1)
                            arrays.pushBack(RotateTo::create(time,sckew->getSkewX()));
                        else
                            arrays.pushBack(SkewTo::create(time, sckew->getSkewX(), sckew->getSkewY()));
                    }
                }
            }
        }
    }
    if(arrays.size()>0)
        temp = Spawn::create(arrays);
    return temp;
}

void FrameStatus::applayAnimation(const std::string& animationName){
    auto temps = searchFrame(animationName);
    for(auto frame:temps)
        frame->apply(0);
}

FrameStatus* FrameStatus::create(const std::string&nodeName){
    auto pFrame = new(std::nothrow) FrameStatus();
    if(pFrame != nullptr&& pFrame->init(nodeName)){
        pFrame->autorelease();
        return pFrame;
    }else {
        CC_SAFE_RELEASE_NULL(pFrame);
        return nullptr;
    }
}

bool FrameStatus::init(const std::string&nodeName){
    this->nodeName = nodeName;
    return true;
}