//
//  SVGSource.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/21/15.
//
//

#ifndef __ScribbleDemo_3_2__SVGSource__
#define __ScribbleDemo_3_2__SVGSource__

#include <stdio.h>
#include <string>
#include <MonkSVG/mkOpenVG_SVG.h>
#include <MonkVG/openvg.h>
#include <MonkVG/vgext.h>
#include "cocos2d.h"
#include "SVGFill.h"
using namespace cocos2d;
using namespace std;
class SVGSource : public Ref{
public:
    static void setTessellationIterations(unsigned int pNumberOfTesselationIterations);//设置迭代次数，数字越大，矢量图越平滑，效率越低。
    SVGSource *initWithData(Data *pData, SVGFill::FillState pState);
    SVGSource *initWithFile(string pFile, SVGFill::FillState pState);
    void draw();
    void optimize();
#pragma - PROPERTYs
    CC_SYNTHESIZE(Rect, contentRect_, ContentRect);
    CC_SYNTHESIZE(Size, contentSize_, ContentSize);
    CC_SYNTHESIZE_READONLY(bool, _hasTransparentColors, HasTransparentColors);
    
protected:
    virtual bool init();
    
private:
    CC_SYNTHESIZE(bool, isOptimized_, IsOptimized);
    CC_SYNTHESIZE(MonkSVG::OpenVG_SVGHandler::SmartPtr, svg_, Svg);
};
#endif /* defined(__ScribbleDemo_3_2__SVGSource__) */
