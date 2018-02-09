//
//  LQPolygon.h
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/9/15.
//
//

#ifndef __MermaidSeaKingdom__LQPolygon__
#define __MermaidSeaKingdom__LQPolygon__

#include "cocos2d.h"
USING_NS_CC;
class LQPolygon : public Ref{
public:
    static LQPolygon* create(const std::vector<Vec2>& poits);
    CREATE_FUNC(LQPolygon);
    virtual bool containsPoint(Vec2);
    virtual bool init();
    virtual float distanceToPoint(Vec2);
    static  LQPolygon* ZERO;
    const std::vector<Vec2>& getPolygonPoits();
protected:
    std::vector<Vec2> polygonPoits;
};
#endif /* defined(__MermaidSeaKingdom__LQPolygon__) */
