//
//  PicHelp.h
//  IceCreamSundae
//
//  Created by luotianqiang on 16/5/29.
//
//

#ifndef PicHelp_hpp
#define PicHelp_hpp

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

class PicHelp {
public:
    static void caluteImage(RenderTexture* render,vector<int>& maxs,vector<int> &mins);
};

#endif /* PicHelp_hpp */
