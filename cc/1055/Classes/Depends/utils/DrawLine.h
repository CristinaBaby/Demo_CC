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
    CC_SYNTHESIZE_RETAIN(Node*, _acctionNode, AcctionNode);
    CC_SYNTHESIZE_RETAIN(ActionInterval*,_spwanAction,SpwanAction);
public:
    DrawLine();
    virtual~DrawLine();
    CREATE_FUNC(DrawLine);
    bool init();
    virtual void onEnter();
    bool  touchbegain(Touch*, Event*);
    virtual void touchMove(Touch*, Event*);
    virtual void touchEnd(Touch*, Event*);
    void setPos(const vector<Vec2>& points);
protected:
    vector<Vec2> points;
    DrawNode *_drawNode,*_add;
    int touchIndex;
protected:
    void updateDrawNode();
};

#endif /* DrawLine_hpp */
