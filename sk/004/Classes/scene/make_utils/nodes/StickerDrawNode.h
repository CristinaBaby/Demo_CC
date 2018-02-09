//
//  StickerDrawNode.h
//  ccats1061
//
//  Created by liji on 16/8/23.
//
//

#ifndef StickerDrawNode_h
#define StickerDrawNode_h

#include <stdio.h>
#include "MakeBaseNode.h"
#include "ScribbleSauce.h"
class StickerDrawNode : public MakeBaseNode
{
public:
    StickerDrawNode():
    _sticker(nullptr),
    _needDraw(false),
    _drawNode(nullptr),
    _pen(nullptr),
    _closeBtnClicked(nullptr),
    _enable(true)
    {
        _colorVec =
        {
            Color3B::BLUE,
            Color3B(180,0,242),
            Color3B::RED,
            Color3B(104,0,4),
            Color3B::GREEN,
            Color3B::YELLOW,
        };
    };
    virtual ~StickerDrawNode(){};

    CREATE_FUNC(StickerDrawNode);
    bool init();

    function<void()> _closeBtnClicked = nullptr;
    function<void()> _confirmBtnClicked = nullptr;
    string _screenName = "paint_juice.png";

protected:
    Sprite* _sticker;
    bool _needDraw;
    ScribbleSauce* _drawNode;
    Sprite* _pen;
    vector<Color3B> _colorVec;
    vector<Button*> _btnVec;
    bool _enable;

private:
    void onEnter();
    void onExit();
    
};
#endif /* StickerDrawNode_h */
