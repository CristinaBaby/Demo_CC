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
    virtual bool containsPoint(Vec2);
    virtual bool init();
    virtual float distanceToPoint(Vec2);
};
#endif /* defined(__MermaidSeaKingdom__LQPolygon__) */
