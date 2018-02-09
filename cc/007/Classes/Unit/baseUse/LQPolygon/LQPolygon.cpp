//
//  LQPolygon.cpp
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/9/15.
//
//

#include "LQPolygon.h"
#include "PolygonHelp.h"
LQPolygon* LQPolygon::ZERO =  new LQPolygon();

LQPolygon* LQPolygon::create(const std::vector<Vec2>& poits){
    auto p = new LQPolygon();
    if(p&&p->init()){
        p->polygonPoits = poits;
        return p;
    }
    CC_SAFE_RELEASE_NULL(p);
    return nullptr;
}

bool LQPolygon::containsPoint(Vec2 p){
    if(polygonPoits.size()==0)
        return true;
    else {
        return PolygonHelp::isPointInPolygon(p, polygonPoits);
    }
}

bool LQPolygon::init(){
      return true;
}

float LQPolygon::distanceToPoint(Vec2){
    return 0;
}

const std::vector<Vec2>&LQPolygon:: getPolygonPoits(){
    return polygonPoits;
}