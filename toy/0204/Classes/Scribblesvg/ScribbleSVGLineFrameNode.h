//
//  ScribbleSVGLineFrameNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/9/8.
//
//

#ifndef __ScribbleDemo_3_2__ScribbleSVGLineFrameNode__
#define __ScribbleDemo_3_2__ScribbleSVGLineFrameNode__

#include <stdio.h>
#include "ScribblePatternNode.h"
#include "ScribbleUtil.h"
#include "SVGSprite.h"
#include "SVGFill.h"

class ScribbleSVGLineFrameNode : public ScribblePatternNode {
public:
    static ScribbleSVGLineFrameNode *create(string pLineArtFile);
    ScribbleSVGLineFrameNode();
    virtual ~ScribbleSVGLineFrameNode();
    bool init(string pLineArtFile);
    virtual bool setPattern(string pSVGFile, SVGFill::FillState pState = SVGFill::FillState::eAuto);
    void paint(Point pPosition);
    void paint(Point pStartPosition, Point pEndPosition);
    Sprite *getPattern(){return _patternNode;}
    void setLineFrame(Node *pLineFrame, Vec2 pPosition = Vec2::ZERO);
    void debugPattern(bool pDebug = true);
    
protected:
    Sprite *_patternNode;
    Node *_lineFrameNode;
    bool _debugPattern;
};
#endif /* defined(__ScribbleDemo_3_2__ScribbleSVGLineFrameNode__) */
