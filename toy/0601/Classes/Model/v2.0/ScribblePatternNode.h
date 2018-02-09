//
//  ScribblePatternNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/26/14.
//
//

#ifndef __ScribbleDemo_3_2__ScribblePatternNode__
#define __ScribbleDemo_3_2__ScribblePatternNode__

#include <stdio.h>
#include "ScribbleNode.h"
#include "ScribblePattern.h"

class ScribblePatternNode : public ScribbleNode {
public:
    static ScribblePatternNode *create(Size pCanvasSize);
    ScribblePatternNode();
    ~ScribblePatternNode();
    bool setPattern(string pShapeFile, Point pPosition);
    bool setPattern(string pShapeFile);
    bool useTarget(Color4F pTargetColor);
    bool useTarget(Sprite *pTarget);
    bool setPatternColor(Color4F pColor);
protected:
    ScribblePattern *getScribble(){return static_cast<ScribblePattern *>(_scribble);}
    virtual bool init(Size pCanvasSize);
    inline Image *getPatternImage(){return _patternImage;}
protected:
    Image *_patternImage;
};

#endif /* defined(__ScribbleDemo_3_2__ScribblePatternNode__) */
