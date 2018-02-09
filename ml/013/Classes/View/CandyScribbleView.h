//
//  CandyScribbleView.h
//  Chups006
//
//  Created by QAMAC01 on 14-9-17.
//
//

#ifndef __Chups006__CandyScribbleView__
#define __Chups006__CandyScribbleView__

#include <iostream>
#include "cocos2d.h"
#include "Scribble.h"
#include "ScribbleTouchNode.h"

class CandyScribbleView : public cocos2d::Layer
{
    enum class ToolTag{
        eBrushTag,
        eTowelTag,
        eAcneToolTag
    };
public:
    // implement the "static node()" method manually
    CREATE_FUNC(CandyScribbleView);
    CandyScribbleView();
    ~CandyScribbleView();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    virtual bool touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    void setCoatingConfig(int h,int s, int v,const char* path);
    
    void updateScribble(const char* name);
    void updateScribbleWithTexture(Texture2D* txt , std::string BottleName);
    void forbidScribble();
    void openScribble();
    
private:// a selector callback
    void brushAction();
    void coatingSelTouched(Ref* obj);
    virtual void onExit();

    
private://UI
    void initCandyCoating();
    
private:
    Scribble *_scribble;
    Sprite *_targetCandyCoating;
    RenderTexture *_canvasCandyCoating;
    ScribbleTouchNode* _coatingNode;
    ToolTag _currentToolTag;
//    RenderTexture *_rtx;
    CC_SYNTHESIZE(RenderTexture *, _rtx, Rtx);
    
    Vector<Texture2D *> unitImg_;
    Sprite* pSprite2_;
    Sprite* hand_;

    bool needSetTarget_ = true;
    
};
#endif /* defined(__Chups006__CandyScribbleView__) */
