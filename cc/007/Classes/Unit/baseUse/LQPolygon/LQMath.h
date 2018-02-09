//
//  LQMath.h
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef LQMath_h
#define LQMath_h
#include "cocos2d.h"
USING_NS_CC;
#include "LQPolygon.h"
#include "LQCircle.h"
#include "LQRect.h"
#include "LQEllipse.h"
#include "PolygonHelp.h"
class LQMath {
public:
    static Vec2 LQgetShadow(Vec2 src,Vec2 direction ){
        return (src.dot(direction)/direction.getLengthSq())*Vec2(direction.x,direction.y);
    }
    
    static bool C1IntersectC2(LQCircle* c1,LQCircle* c2){
        return c1->_center.distanceSquared(c2->_center)<=((c1->_R+c2->_R)*(c1->_R+c2->_R));
    }
    
    static int getOpacityNum(Image* pImage){
        int lCounter = 0;
        for (int i = 0; i < pImage->getHeight(); ++i) {
            for (int j = 0; j < pImage->getWidth(); ++j) {
                auto *lData = pImage->getData();
                auto lPixcelIndex = i * pImage->getWidth() + j;
                auto lRed = lData[lPixcelIndex * 4];
                auto lGreen = lData[lPixcelIndex * 4 + 1];
                auto lBlue = lData[lPixcelIndex * 4 + 2];
                auto lAlpha = lData[lPixcelIndex * 4 + 3];
                if (lAlpha > 10) {
                    if (lRed > 0 || lGreen > 0 || lBlue > 0) {
                        ++lCounter;
                    }
                }
            }
        }
        return lCounter;
        
    }
};


#endif /* LQMath_h */
