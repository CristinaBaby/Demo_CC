//
//  MultiSprite.h
//  Milkshake
//
//  Created by luotianqiang1 on 4/13/15.
//
//

#ifndef __Milkshake__MultiSprite__
#define __Milkshake__MultiSprite__

#include "EventSprite.h"
NS_KD_BEGIN
class MultiSprite : public  EventSprite{
public:
    MultiSprite();
    virtual ~MultiSprite();
    static MultiSprite* create(const std::string& filename);
     static MultiSprite* createWithTex(Texture2D* tex);
    static MultiSprite* createWithTex(Texture2D* tex,Rect r);
    void setFocusrect(Rect rect);
    void setMoveRect(Rect rect);
    void setCenterPoint(Vec2 point);
    void resetSprite();
private:
    Rect focusRect,moveRect; //in world
    std::vector<Touch*> _touches;
    std::vector<Vec2> ornPosition;//保存初始位置和锚点
    std::vector<Vec2> centerPoint;//旋转和缩放的参照点(in world)
    void move(Vec2 distance);
    void scale(float distace);
    void rote(float distace);
    Vec2 touchPoint,touchDirection;//前一个touch位置,前一个旋转方向向量
    float maxScale,minScale;
    float length;//前一次两个touch点之前的距离
    float ornScale;
    std::vector<Vec2> touchCache;
protected:
     virtual bool initWithFile(const std::string& filename);
    virtual bool onTouchBegan(Touch*pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch*pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch*pTouch, Event *pEvent);
    virtual void onTouchCancle(Touch* pTouch, Event *pEvent);
};
NS_KD_END;
#endif /* defined(__Milkshake__MultiSprite__) */
