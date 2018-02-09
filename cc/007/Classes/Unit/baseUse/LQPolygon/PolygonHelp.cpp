//
//  PolygonHelp.cpp
//  IceCreamSundae
//
//  Created by luotianqiang1 on 16/5/26.
//
//

#include "PolygonHelp.h"
using namespace std;
using namespace cocos2d;
bool PolygonHelp::isPointInPolygon(const Vec2& p,const vector<Vec2>& polygon){
    int nCross=0;
    auto num = polygon.size();
    for(decltype(num) i=0;i<num;i++) {
        auto p1 = polygon[i];
        auto p2 = polygon[(i+1)%num];
        auto slope = (p2.y-p1.y)/(p2.x-p1.x);
        bool cond1 = (p1.x<=p.x)&&(p.x<p2.x);
        bool cond2 = (p2.x<=p.x)&&(p.x<p1.x);
        bool above = (p.y<slope*(p.x-p1.x)+p1.y);
        if((cond1||cond2)&&above)
            nCross++;
    }
    return nCross%2==1;
}

bool PolygonHelp::isPolygonInPolygon(const std::vector<cocos2d::Vec2>& innerPolygon,const std::vector<cocos2d::Vec2>& polygon){
    bool isFlag = true;
    for(const auto& point:innerPolygon){
        if(!isPointInPolygon(point, polygon)){
            isFlag = false;
            break;
        }
    }
    return isFlag;
}


std::vector<cocos2d::Vec2> PolygonHelp::convertoNodePoints(cocos2d::Node* node,const std::vector<cocos2d::Vec2>& chipmunkPoints){
    std::vector<cocos2d::Vec2>  temps;
    if(node != nullptr) {
        for(const auto& point:chipmunkPoints)
            temps.push_back(point + node->getContentSize()*.5);
    }
    return temps;
}

std::vector<cocos2d::Vec2> PolygonHelp::convertoWordPoints(cocos2d::Node* node,const std::vector<cocos2d::Vec2>& chipmunkPoints){
    std::vector<cocos2d::Vec2>  temps;
    if(node != nullptr) {
        for(const auto& point:chipmunkPoints)
            temps.push_back(node->convertToWorldSpace(point + node->getContentSize()*.5));
    }
    return temps;
}