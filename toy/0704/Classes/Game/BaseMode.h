//
//  BaseMode.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__BaseMode__
#define __ColorBook__BaseMode__

#include "cocos2d.h"

class GameScene;

using std::vector;
using std::string;
using namespace cocos2d;

class BaseMode : public Node
{
public:
    BaseMode(GameScene * layer);
    virtual ~BaseMode();
    
    virtual void onTouchesBegan(const vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesCancelled(const vector<Touch*>& touches, Event *unused_event);
    virtual void start();
    virtual void stop();
    
    void checkBoundingBox();
    
public:
    enum class Mode
    {
        NONE,
        PAINT,
        ZOOM,
        WAIT,
        MOVE
    };
    
protected:
    GameScene * _gameScene;
    Layer     * _colouringLayer;
    Rect        _original;
};

#endif /* defined(__ColorBook__BaseMode__) */
