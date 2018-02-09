//
//  DrawLine.hpp
//  lqtest
//
//  Created by luotianqiang1 on 15/12/30.
//
//

#ifndef DrawLine_hpp
#define DrawLine_hpp

#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class DrawLine: public Layer{
public:
    DrawLine();
    CREATE_FUNC(DrawLine);
    bool init();
    virtual void onEnter();
    bool  touchbegain(Touch*, Event*);
    virtual void touchMove(Touch*, Event*);
    virtual void touchEnd(Touch*, Event*);
    void setPos(const vector<Vec2>& points);
protected:
    vector<Vec2> points;
    DrawNode *_drawNode,*_acctionNode,*_add;
    int touchIndex;
protected:
    void updateDrawNode();
};

#endif /* DrawLine_hpp */
