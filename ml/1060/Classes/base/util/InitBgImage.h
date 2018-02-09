//
//  InitBgImage.h
//  MyCppGame
//
//  Created by luotianqiang1 on 11/3/14.
//
//

#ifndef MyCppGame_InitBgImage_h
#define MyCppGame_InitBgImage_h
#include<iostream>
#include "cocos2d.h"
using namespace cocos2d;
class InitBgImage {
public:
    static InitBgImage* getInstance() {
        static InitBgImage a;
        return &a;
    }
     Texture2D* crateBgTexNoCache(const char*);
    Texture2D* addBgImage(const char *);
//private:
//     std::unordered_map<std::string, Texture2D*> _textures;
};


#endif
