//
//  ActionHelper.h
//
//  Created by luotianqiang1 on 3/9/15.
//
//

#ifndef __ActionHelper__
#define __ActionHelper__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

enum ActionDirection
{
    kActionDirectionNone,
    kActionDirectionTop,
    kActionDirectionBottom,
    kActionDirectionLeft,
    kActionDirectionRight
};


class CC_DLL SpriteTextureRectTo : public ActionInterval
{
public:
    /**
     * Creates the action.
     *
     * @param duration Duration time, in seconds.
     * @param deltaPosition The delta distance in 2d, it's a Vec2 type.
     * @return An autoreleased MoveBy object.
     */
    static SpriteTextureRectTo* create(float duration, const Rect& deltaPosition)
    {
        SpriteTextureRectTo *ret = new (std::nothrow) SpriteTextureRectTo();
        
        if (ret)
        {
            if (ret->initWithDuration(duration, deltaPosition))
            {
                ret->autorelease();
            }
            else
            {
                delete ret;
                ret = nullptr;
            }
        }
        return ret;
    }
    
    //
    // Overrides
    //
//    virtual MoveBy* clone() const override;
//    virtual MoveBy* reverse(void) const  override;
    virtual void startWithTarget(Node *target)
    {
        ActionInterval::startWithTarget(target);
        auto t = dynamic_cast<Sprite*>(target);
        _positionDelta.origin = _endPosition.origin - t->getTextureRect().origin;
        _positionDelta.size = _endPosition.size - t->getTextureRect().size;

//        _previousPosition = _startPosition = t->getTextureRect();
    }
    /**
     * @param time in seconds
     */
    virtual void update(float time)
    {
        if (_target)
        {
            auto sp = dynamic_cast<Sprite*>(_target);
            
#if CC_ENABLE_STACKABLE_ACTIONS
            Rect currentPos = sp->getTextureRect();
            Rect diff;
            diff.origin = currentPos.origin - _previousPosition.origin;
            diff.size = currentPos.size - _previousPosition.size;
            _startPosition.origin = _startPosition.origin + diff.origin;
            _startPosition.size = _startPosition.size + diff.size;
            Rect newPos;
            newPos.origin=  _startPosition.origin + (_positionDelta.origin * time);
            newPos.size=  _startPosition.size + (_positionDelta.size * time);
            log("%f, %f, %f, %f",newPos.origin.x,newPos.origin.y,newPos.size.width,newPos.size.height);
            sp->setTextureRect(newPos);
            _previousPosition = newPos;
#else
//            Rect newPos;
//            newPos.origin=  _startPosition.origin + (_positionDelta.origin * time);
//            newPos.size=  _startPosition.size + (_positionDelta.size * time);
//
//            sp->setTextureRect(newPos);
#endif // CC_ENABLE_STACKABLE_ACTIONS

        }

    
    }
    
CC_CONSTRUCTOR_ACCESS:
    SpriteTextureRectTo(){}
    virtual ~SpriteTextureRectTo() {}
    
    /** initializes the action */
    bool initWithDuration(float duration, const Rect& deltaPosition)
    {
        bool ret = false;
        
        if (ActionInterval::initWithDuration(duration))
        {
            _endPosition = deltaPosition;
            ret = true;
        }
        
        return ret;
    }
    
protected:
    Rect _positionDelta;
    Rect _startPosition;
    Rect _previousPosition;
    Rect _endPosition;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(SpriteTextureRectTo);
};


class ActionHelper
{
public:
    /**
     *  根据node入场的参数(终点，方向)将Node放到屏幕外，为入场动画站好位置
     *
     *  @param node       作用对象
     *  @param endPosiont 入场的目标点
     *  @param direction  从哪个方向进入场景
     */
    static void putNodeToSceneOutBorder(Node* node, Vec2 endPosiont,ActionDirection direction);
    
    /**
     *  物品以Bounce方式进入场景封装函数
     *
     *  @param node               操作对象
     *  @param endPosiont         进入场景目标点
     *  @param direction          入场方向
     *  @param actionEndCallCall  入场完成回调
     */
    static void moveInToSceneBounce(Node* node, Vec2 endPosiont, ActionDirection direction, std::function<void()> actionEndCallCall = nullptr);
    
    /**
     *  物品以ExponentialOut方式进入场景封装函数
     *
     *  @param node               操作对象
     *  @param endPosiont         进入场景目标点
     *  @param direction          入场方向
     *  @param actionEndCallCall  入场完成回调
     */
    static void moveInToSceneExponentialOut(Node* node, Vec2 endPosiont,ActionDirection direction, std::function<void()> actionEndCall = nullptr);
    
    /**
     *  物品移动出场景外，并移除自己
     *
     *  @param node               操作对象
     *  @param direction          出场方向
     *  @param actionEndCall      出场完成回调
     */
    static void moveOutofSceneAndRemoveSelf(Node* node,ActionDirection direction, std::function<void()> actionEndCall = nullptr);
    
    /**
     *  创建动画帧
     *
     *  @param pathFormat         路径格式
     *  @param startIdx           第一帧索引
     *  @param endIdx             结束帧索引
     *  @param interval           一帧间隔
     *  @param frameRevise        反向播放序列帧
     */

    static Animate* createAnimate(const std::string &pathFormat,
                                  int startIdx,
                                  int endIdx,
                                  float interval = 0.1f ,
                                  bool frameRevise = false);
    
    
    static ActionInterval* createRollJelly();
    
    static ActionInterval* createFloatAction(float duration = 0.4f);
    
    static void changeFade(Sprite* preSprite, Sprite* curentSprite);
    
    static ActionInterval* createShakeAction(float distance = 5);
    
    static ActionInterval* createRotatePromptLeft();
    static ActionInterval* createRotatePromptRight();

    static ActionInterval* createRoteAction(float rote = 10 );
    
    static ActionInterval* createMovePrompt();
    
    static ActionInterval* createScalePop();
    static ActionInterval* createScaleHint();
    static ActionInterval* moveToPostionByBezier(Vec2 startPos,Vec2 endPo,float duration);

    
    static void runMoveHideAction(Node* node, Vec2 startPos,Vec2 endPo,float time = 0.8f);
    
    static void runMoveHideAction2(Node* node);
};

#endif /* defined(__PizzaMaker__ActionHelper__) */
