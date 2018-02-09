//
//  EventMotionCotrll.cpp
//  Milkshake
//
//  Created by luotianqiang1 on 4/15/15.
//
//

#include "EventMotionCotrol.h"
#import "AppController.h"
#import "RootViewController.h"

void EventMotionCotrol::setMultTouch(bool flag) {
    RootViewController *root = [AppController sharedAppController].viewController;
    [root.view setMultipleTouchEnabled:flag];
}