//
//  MoveSprite.h
//  MyCppGame
//
//  Created by luotianqiang1 on 9/10/14.
//
//

#ifndef __MyCppGame__MoveSprite__
#define __MyCppGame__MoveSprite__

#include <iostream>
#include "EventSprite.h"
#include "LQMath.h"
NS_KD_BEGIN
enum MoveSpriteType {
    kMoveEndBackStart,
    kMoveEndTouchStop,
    KMoveLimitInRect
};
class MoveSprite : public EventSprite{
     CC_SYNTHESIZE_RETAIN(LQPolygon*,limitRect,Rect);
public:
    MoveSprite();
    static MoveSprite* create();
    static MoveSprite* create(const std::string& filename);
    static MoveSprite* createWithNoCache(const std::string& filename);
    static MoveSprite* createWithSpriteFrame(SpriteFrame*);
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancle(Touch *pTouch, Event *pEvent);
    void savePosition();
    Rect getRect();
    void setMoveType(MoveSpriteType moveType);
    //是否在限制区域中
    virtual bool isInLimitRect();
    Vec2 getCheckPointInPoints();//wordPoint
    Vec2 getCheckPointInPoints(Vec2 point);//wordPoint
    void setTargetAnPoint(Vec2 point);
    void setPositionWithTarget(Vec2 point);//wordPoint
    void setBrintToTop(bool flag);
    bool  isRequsetMove();
    virtual void moveBack();
    Vec2 getOrnPos(){return orgiginal;}
    void setOrnPoint(Vec2 point);
protected:
    virtual void backEnd();
    Vec2 orgiginal,oldPosition;
    MoveSpriteType moveType;
    //回到起点
 
    //要检测的位置
    Vec2 targetAnPoint;
    //点击时是否显示到最上面
    bool isBrintFront;
    //最开始的Zorder;
    int oldZorder;
    bool isFisrt;
    bool isGetTouch;
    
    void moveBackSchedule(float);
    Vec2 distance;
    float _moveTime;
};
NS_KD_END
#endif /* defined(__MyCppGame__MoveSprite__) */
