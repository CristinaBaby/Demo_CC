//
//  ScribbleSauce.hpp
//  MLABS1059
//
//  Created by wusonglin1 on 15/11/27.
//
//

#ifndef ScribbleSauce_hpp
#define ScribbleSauce_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

//涂酱，有一点立体的感觉
class ScribbleSauce : public Node{
public:
    enum class touchType{
        touchBegin,
        touchMove,
        touchEnd,
        touchCancle
    };
  CC_SYNTHESIZE(Color3B, _drawColor, DrawColor);
    CC_SYNTHESIZE(float,_brushScale,BrushScale);
    CC_SYNTHESIZE(bool, _touchBeginEnable, TouchBeginEnable);
public:
    ScribbleSauce();
    ~ScribbleSauce();
public:
    static ScribbleSauce* create(const Size contentSize);
    bool init(const Size contentSize);
private:
    //线性插值算法
    vector<Point> interpolate(Point pStartPosition, Point pEndPosition, float pDelta);
    void paint(RenderTexture *pCanvas, vector<Point> pWorldPositions,Texture2D* texture);
public:
    //设置画刷
    void setBrush(Sprite* brush);
    void setBrush(Texture2D* brush);
    void paint(const Vec2& wordPos);
    void reset();
    
    inline void setDrawing(bool pDrawing){_drawing = pDrawing;}
    inline bool getDrawing(){return _drawing;}
private:
//    Render
    Size m_sContentSize;
    RenderTexture* m_rCanvas;
    Texture2D* m_tBrush;
    
    bool _drawing;
    Sprite* spNode;
public:
    std::function<void(Touch*,touchType)> callBack;
};




#endif /* ScribbleSauce_hpp */
