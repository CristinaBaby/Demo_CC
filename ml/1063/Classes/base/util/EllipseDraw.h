//
//  EllipseDraw.h
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/8/17.
//
//

#ifndef __MermaidSeaKingdom__EllipseDraw__
#define __MermaidSeaKingdom__EllipseDraw__

#include "cocos2d.h"
#include "EllipseConfig.h"
USING_NS_CC;
enum class Ellipse_event_type{
    type_moveCenter,
    type_moveA,
    type_moveB,
    type_moveTool,
    type_null
};



class EllipseDraw: public Node{
public:
    static EllipseDraw* create(EllipseConfig _config);
    bool init(EllipseConfig _config);
    void _draw(EllipseConfig _config);
    void _drawPoint(Vec2 _vec,Color4F _color = Color4F::BLUE);
protected:
    Ellipse_event_type _event_type;
    
    DrawNode *_drawNode;
    EllipseConfig _config;
    void onEnter();
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancle(Touch *pTouch, Event *pEvent);
    bool isContain(Vec2 _center,Vec2 _pos);
protected:
    void initTool();
    Node* _tool;
    void upDateTool();
    Label *_labeCenter,*_labeA,*_labeB,*labeTouchPonit;
};

#endif /* defined(__MermaidSeaKingdom__EllipseDraw__) */
