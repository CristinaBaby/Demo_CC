//
//  SVGSprite.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/24/15.
//
//

#ifndef __ScribbleDemo_3_2__SVGSprite__
#define __ScribbleDemo_3_2__SVGSprite__

#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include "SVGSource.h"
#include "SVGFill.h"
using namespace cocos2d;
using namespace std;

class SVGSprite : public Node {
public:
    static SVGSprite *create(string pFile, SVGFill::FillState pState);
    static SVGSprite *create(SVGSource *pSource);
    SVGSprite();
    ~SVGSprite();
    bool init(string pFile, SVGFill::FillState pState);
    bool init(SVGSource *pSource);
    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    void onDraw(const Mat4 &transform, uint32_t flags);
public:
    CC_SYNTHESIZE_RETAIN(SVGSource *, source_, Source);
    CC_SYNTHESIZE(BlendFunc, blendFunc_, BlendFunc);
    CustomCommand _customCommand;
};
#endif /* defined(__ScribbleDemo_3_2__SVGSprite__) */
