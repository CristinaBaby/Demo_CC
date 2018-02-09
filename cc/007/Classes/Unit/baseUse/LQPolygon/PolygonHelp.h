//
//  PolygonHelp.h
//  IceCreamSundae
//
//  Created by luotianqiang1 on 16/5/26.
//
//

#ifndef PolygonHelp_hpp
#define PolygonHelp_hpp

#include <iostream>
#include "cocos2d.h"
class PolygonHelp {
public:
    static bool isPointInPolygon(const cocos2d::Vec2& point,const std::vector<cocos2d::Vec2>& polygon);
    static bool isPolygonInPolygon(const std::vector<cocos2d::Vec2>& innerPolygon,const std::vector<cocos2d::Vec2>& polygon);
    static std::vector<cocos2d::Vec2> convertoNodePoints(cocos2d::Node* node,const std::vector<cocos2d::Vec2>& chipmunkPoints);
    static std::vector<cocos2d::Vec2> convertoWordPoints(cocos2d::Node* node,const std::vector<cocos2d::Vec2>& chipmunkPoints);
};
#endif /* PolygonHelp_hpp */
