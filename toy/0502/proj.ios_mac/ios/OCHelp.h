//
//  OCHelp.h
//  KidsBook
//
//  Created by lidong on 14-10-10.
//
//

#ifndef __KidsBook__OCHelp__
#define __KidsBook__OCHelp__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class OCHelp {
    
public:
    
    static void loadSupport();
    static void loadTermsOfUse();
    static void loadRateUs(const char* appleID,const char* info);
};

#endif /* defined(__KidsBook__OCHelp__) */
