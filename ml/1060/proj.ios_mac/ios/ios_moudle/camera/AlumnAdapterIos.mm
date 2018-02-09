//
//  AlumnAdapter.cpp
//  Doctor2
//
//  Created by tanshoumei on 13-9-18.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "AlumnAdapter.h"
#include "ImageUtil.h"
#import "AlumnImpIos.h"
#import "AppController.h"
#import "RootViewController.h"
void AlumnAdapter::saveToAlumn(cocos2d::Image *apCCImg)
{
    [[AlumnImpIos shared] saveToAlumn:ImageUtil::CCImageToUIImage(apCCImg)];
}

void AlumnAdapter::openAlumn() {
    [[AlumnImpIos shared] openAlumn];
}



#endif