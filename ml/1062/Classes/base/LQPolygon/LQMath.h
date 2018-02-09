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
class LQMath {
public:
    static Vec2 LQgetShadow(Vec2 src,Vec2 direction ){
        return (src.dot(direction)/direction.getLengthSq())*direction;
    }
    
    static bool C1IntersectC2(LQCircle* c1,LQCircle* c2){
        return c1->_center.distanceSquared(c2->_center)<=((c1->_R+c2->_R)*(c1->_R+c2->_R));
    }
    
    static int getOpacityNum(Image* pImage){
        long lCounter = 0;
        for (long i = 0; i < pImage->getHeight(); ++i) {
            for (long j = 0; j < pImage->getWidth(); ++j) {
                unsigned char *lData = pImage->getData();
                long lPixcelIndex = i * pImage->getWidth() + j;
                unsigned char lRed = lData[lPixcelIndex * 4];
                unsigned char lGreen = lData[lPixcelIndex * 4 + 1];
                unsigned char lBlue = lData[lPixcelIndex * 4 + 2];
                unsigned char lAlpha = lData[lPixcelIndex * 4 + 3];
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
