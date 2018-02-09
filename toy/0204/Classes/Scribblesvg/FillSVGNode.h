//
//  FillSVGNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/11/3.
//
//

#ifndef __ScribbleDemo_3_2__FillSVGNode__
#define __ScribbleDemo_3_2__FillSVGNode__

#include <stdio.h>
#include "ScribbleSVGLineFrameNode.h"
#include "ColoringSVGNode.h"
class FillSVGNode : public ScribbleSVGLineFrameNode{
public:
    static FillSVGNode *create(string pLineArtFile);
    bool init(string pLineArtFile);
    ~FillSVGNode();
    virtual bool setPattern(string pSVGFile, SVGFill::FillState pState = SVGFill::FillState::eAuto);
    inline ColoringSVGNode *getColoringSVGNode(){return _coloringSVGNode;}
private:
    ColoringSVGNode *_coloringSVGNode;
};
#endif /* defined(__ScribbleDemo_3_2__FillSVGNode__) */
