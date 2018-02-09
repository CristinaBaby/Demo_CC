//
//  MyPaintSprite.h
//  MyCppGame
//
//  Created by luotianqiang1 on 9/12/14.
//
//

#ifndef __MyCppGame__MyPaintSprite__
#define __MyCppGame__MyPaintSprite__

#include <iostream>
#include "ScribbleNode.h"

class MyPaintSprite : public ScribbleNode{
public:
    static MyPaintSprite *create(int pCanvasWidth, int pCanvasHeight);
    long calcute(Image *pImage);
    bool useTarget(string pTargetFile);
    float getPecent();
    inline  RenderTexture * getRender(){return _canvas;}
    
protected:
    long restorePixes;
    
};

#endif /* defined(__MyCppGame__MyPaintSprite__) */
