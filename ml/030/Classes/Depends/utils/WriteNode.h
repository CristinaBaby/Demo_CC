//
//  DrawNode.h
//  DrawNode
//
//  Created by ren hong on 14-2-20.
//
//

#ifndef __DrawNode__
#define __DrawNode__

#include <iostream>
#include <vector>
#include <cocos2d.h>
using namespace cocos2d;
using namespace std;

class WriteNode : public Node {
public:
    enum class PenType{
        eUnknow,
        ePencil = 0x1,
        eHighlighter,
        eTwoColor,
        eChalk,
        eColorPen,
        eEraser,
        eMagicPen
    };
public:
    static WriteNode *create(Size pCanvasSize);
    virtual bool init(Size pCanvasSize);
    
    void touchBegan(Point pPoint);
    void touchMoved(Point pStartPoint, Point pEndPoint);
    void touchEnded(Point pPoint);
public:
    WriteNode();
    ~WriteNode();
     void setPenMagicSprite(Sprite* pMagic){_magic = pMagic; _magic->retain();}
    void setPenType(PenType pType);
    PenType getPenType(){return _penType;}
    void setPenColor(Color4B pColor){_penColor = pColor;}
    void setPenColorOutline(Color4B pColor){_outlineColor = pColor;}
    void setPenStrokeColor(Color4B pColor){_strokeColor = pColor;}
    void reset();
    
    void setPenScale(float s);
    
    void setPenSprite(const string& path);
    
    inline void setDrawing(bool pDrawing){_drawing = pDrawing;}
    inline bool getDrawing(){return _drawing;}
private:
    void addCanvas();
    
    void draw(Sprite *pBrush, RenderTexture *pCanvas, vector<Point> pPositions);
    void draw(Sprite *pBrush, RenderTexture *pCanvas, Point pStartPosition, Point pEndPosition);
    void draw(Sprite *pBrush, RenderTexture *pCanvas, Point pPosition);
    
private:
    PenType _penType;
    Sprite *_brushFront;
    Sprite *_brushMiddle;
    Sprite *_brushBack;
    Sprite *_earser;
    RenderTexture *_canvasPreviewFront;
    RenderTexture *_canvasPreviewMiddle;
    RenderTexture *_canvasPreviewBack;
    RenderTexture *_canvas;
    bool _drawing;
    int count;
    Sprite*  _magic;
    Color4B _penColor;//画笔内部颜色
    Color4B _outlineColor;//画笔外沿颜色
    Color4B _strokeColor;//描边颜色
    Size _size;
};
#endif /* defined(__DrawNode__) */

